////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt5 framework.
// Copyright (C) 2017 Nicolas Kogler
//
// Cranberry is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Cranberry is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Cranberry. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////


// Cranberry headers
#include <Cranberry/Graphics/RawAnimation.hpp>
#include <Cranberry/Graphics/Sprite.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - File %2 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Invalid version: %2.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Sheet could not be loaded.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Invalid movement.")
CRANBERRY_CONST_VAR(QString, e_05, "%0 [%1] - Invalid frame.")
CRANBERRY_CONST_VAR(QString, e_06, "%0 [%1] - Movement %2 does not exist.")


QRectF getJsonRect(QJsonObject& obj)
{
    if (obj.isEmpty()) return QRectF();

    QJsonValue x = obj.value("x");
    QJsonValue y = obj.value("y");
    QJsonValue w = obj.value("w");
    QJsonValue h = obj.value("h");

    return QRectF(x.toDouble(), y.toDouble(), w.toDouble(), h.toDouble());
}


Sprite::Movement::~Movement()
{
    delete anim;
}


Sprite::Sprite()
    : RenderBase()
    , TransformBase()
    , m_currentMove(nullptr)
    , m_isRunning(false)
    , m_isBlocking(false)
{
    m_receiver.setSprite(this);

    // Movement should stop as soon as tile-based stuff is finished.
    QObject::connect(
            transformableEmitter(),
            SIGNAL(stoppedMoving()),
            &m_receiver,
            SLOT(stoppedRunning())
            );
}


Sprite::~Sprite()
{
    destroy();
}


bool Sprite::isNull() const
{
    return RenderBase::isNull() ||
           m_movements.isEmpty();
}


bool Sprite::isBlocking() const
{
    return m_isBlocking;
}


bool Sprite::isRunning() const
{
    return m_isRunning;
}


bool Sprite::create(const QString& path, Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    // Attempts to load the file.
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        return cranError(ERRARG_1(e_01, path));
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    file.close();

    // Reads the version attribute.
    QJsonObject top = json.object();
    QJsonValue version = top.value("version");

    int v = version.toInt();
    if (version.isNull() || v <= 0 || v >= 2)
    {
        return cranError(ERRARG_1(e_02, QString::number(v)));
    }
    else if (v == 1)
    {
        // Loads the spritesheet.
        QJsonValue sheet = top.value("sheet");
        QImage img(cranResourcePath(sheet.toString()));

        if (sheet.isNull() || img.isNull())
        {
            return cranError(ERRARG(e_03));
        }

        // Loads the movements.
        QJsonArray movements = top.value("movements").toArray();
        Q_FOREACH (QJsonValue m, movements)
        {
            QJsonObject obj = m.toObject();
            QJsonValue nme = obj.value("name");
            QJsonValue mode = obj.value("mode");
            QJsonValue advanceX = obj.value("advanceX");
            QJsonValue advanceY = obj.value("advanceY");
            QJsonObject idle = obj.value("idle").toObject();
            QJsonArray frames = obj.value("frames").toArray();

            if (nme.isNull() || advanceX.isNull() || advanceY.isNull() || frames.isEmpty())
            {
                return cranError(ERRARG(e_04));
            }

            auto* move = new Movement;
            move->name = nme.toString();
            move->mode = (mode.toString() == "tile") ? MovementTile : MovementDefault;
            move->totalTime = 0.0;
            move->advanceX = advanceX.toDouble();
            move->advanceY = advanceY.toDouble();
            move->idle = getJsonRect(idle);
            move->anim = new RawAnimation;

            QVector<AnimationFrame> animFrames;
            qint32 currentFrame = 0;

            // Loads the frames.
            Q_FOREACH (QJsonValue frame, frames)
            {
                QJsonObject fobj = frame.toObject();
                QJsonValue duration = fobj.value("duration");
                QJsonObject rect = fobj.value("rect").toObject();

                if (duration.isNull() || rect.isEmpty())
                {
                    return cranError(ERRARG(e_05));
                }

                AnimationFrame animFrame;
                animFrame.setAtlasId(0);
                animFrame.setDuration(duration.toDouble() / 1000.0);
                animFrame.setFrameId(currentFrame);
                animFrame.setRectangle(getJsonRect(rect));

                move->totalTime += animFrame.duration();
                animFrames.append(animFrame);
                currentFrame++;
            }

            if(!move->anim->createRawAnimation({ img }, animFrames, renderTarget()))
            {
                return false;
            }

            move->anim->setIdleFrame(0, move->idle);
            m_movements.insert(move->name, move);
        }

        return true;
    }

    return false;
}


void Sprite::destroy()
{
    for (auto* const m : m_movements.values())
    {
        delete m;
    }

    m_movements.clear();
    m_currentMove = nullptr;
    m_isBlocking = false;
    m_isRunning = false;

    RenderBase::destroy();
}


void Sprite::runMovement(const QString& n)
{
    if (m_isBlocking) return;
    if (m_isRunning && m_currentMove)
    {
        startMovingBy(m_currentMove->advanceX, m_currentMove->advanceY);
        resumeMovement();
        return;
    }

    // Tries to find the movement.
    auto* m = m_movements.value(n, nullptr);
    if (m == nullptr)
    {
        cranError(ERRARG_1(e_06, n));
        return;
    }

    // Runs the animation.
    m->anim->startAnimation(AnimateForever);
    if (m->mode == MovementTile)
    {
        float mx = m->advanceX / m->totalTime;
        float my = m->advanceY / m->totalTime;

        setMoveSpeed(mx, my);
        startMovingBy(m->advanceX, m->advanceY);

        m_isBlocking = true;
    }

    m_isRunning = true;
    m_currentMove = m;
}


void Sprite::runIdle(const QString& n)
{
    if (m_isBlocking) return;

    // Tries to find the movement.
    auto* m = m_movements.value(n, nullptr);
    if (m == nullptr)
    {
        cranError(ERRARG_1(e_06, n));
        return;
    }

    // Starts the idle mode.
    m->anim->startIdle();
    m_currentMove = m;
    m_isRunning = false;
}


void Sprite::resumeMovement()
{
    if (m_isBlocking || !m_currentMove) return;

    m_currentMove->anim->resumeAnimation();
    m_isRunning = true;
}


void Sprite::stopMovement()
{
    if (!m_currentMove) return;

    m_currentMove->anim->stopAnimation();
    m_currentMove->anim->stopMoving();
    m_currentMove->anim->startIdle();

    m_isBlocking = false;
    m_isRunning = false;
}


void Sprite::update(const GameTime& time)
{
    if (!m_currentMove) return;

    m_currentMove->anim->update(time);
    updateTransform(time);
}


void Sprite::render()
{
    if (!m_currentMove) return;

    m_currentMove->anim->setShaderProgram(shaderProgram());
    m_currentMove->anim->setPosition(pos());
    m_currentMove->anim->setAngle(angle());
    m_currentMove->anim->setOpacity(opacity());
    m_currentMove->anim->setOrigin(origin().toVector2D());
    m_currentMove->anim->setScale(scaleX(), scaleY());
    m_currentMove->anim->render();
}


void Sprite::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void Sprite::setBlendColor(
        const QColor &tl,
        const QColor &tr,
        const QColor &br,
        const QColor &bl
        )
{
    for (auto* const m : m_movements.values())
    {
        m->anim->setBlendColor(tl, tr, br, bl);
    }
}


void Sprite::setBlendMode(BlendModes modes)
{
    for (auto* const m : m_movements.values())
    {
        m->anim->setBlendMode(modes);
    }
}


void Sprite::setEffect(Effect effect)
{
    for (auto* const m : m_movements.values())
    {
        m->anim->setEffect(effect);
    }
}

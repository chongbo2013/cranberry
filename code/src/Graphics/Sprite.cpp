////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#include <Cranberry/System/Models/TreeModel.hpp>

// Qt headers
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - File %2 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Invalid version: %2.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Sheet could not be loaded.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Invalid movement.")
CRANBERRY_CONST_VAR(QString, e_05, "%0 [%1] - Invalid frame.")
CRANBERRY_CONST_VAR(QString, e_06, "%0 [%1] - Movement %2 does not exist.")


CRANBERRY_USING_NAMESPACE


QRectF getJsonRect(QJsonObject& obj)
{
    if (obj.isEmpty()) return QRectF();

    QJsonValue x = obj.value("x");
    QJsonValue y = obj.value("y");
    QJsonValue w = obj.value("w");
    QJsonValue h = obj.value("h");

    return QRectF(x.toDouble(), y.toDouble(), w.toDouble(), h.toDouble());
}


MovementMode getJsonMoveMode(QJsonValue& val)
{
    return val.toString() == "tile" ? MovementTile : MovementDefault;
}


Sprite::Sprite()
    : RenderBase()
    , m_currentMove(nullptr)
    , m_isRunning(false)
    , m_isBlocking(false)
{
    m_receiver.setSprite(this);

    // Movement should stop as soon as tile-based stuff is finished.
    QObject::connect(
        signals(),
        &TransformBaseEmitter::stoppedMoving,
        &m_receiver,
        &SpriteReceiver::stoppedRunning
        );
}


Sprite::~Sprite()
{
    destroy();
}


bool Sprite::isNull() const
{
    return RenderBase::isNull() || m_movements.isEmpty();
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
            QJsonValue valName = obj.value("name");
            QJsonValue valMode = obj.value("mode");
            QJsonValue valAdvX = obj.value("advanceX");
            QJsonValue valAdvY = obj.value("advanceY");
            QJsonObject objIdle = obj.value("idle").toObject();
            QJsonArray arrFrames = obj.value("frames").toArray();

            if (valName.isNull() ||
                valAdvY.isNull() ||
                valAdvY.isNull() ||
                arrFrames.isEmpty())
            {
                return cranError(ERRARG(e_04));
            }

            SpriteMovement* move = new SpriteMovement;
            move->setName(valName.toString());
            move->setMovementMode(getJsonMoveMode(valMode));
            move->setHorizontalAdvance(valAdvX.toDouble());
            move->setVerticalAdvance(valAdvX.toDouble());
            move->setIdleFrame(getJsonRect(objIdle));
            move->setRawAnimation(new RawAnimation);

            QVector<AnimationFrame> animFrames;
            qint32 currentFrame = 0;

            // Loads the frames.
            Q_FOREACH (QJsonValue frame, arrFrames)
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

                move->setTotalDuration(move->totalDuration() + animFrame.duration());
                animFrames.append(animFrame);
                currentFrame++;
            }

            if(!move->animation()->createRawAnimation({ img }, animFrames, renderTarget()))
            {
                return false;
            }

            move->animation()->setIdleFrame(0, move->idleFrame());
            m_movements.insert(move->name(), move);
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


void Sprite::beginMove(const QString& n)
{
    if (m_isBlocking)
    {
        return;
    }
    else if (m_isRunning && m_currentMove)
    {
        moveBy(m_currentMove->horizontalAdvance(), m_currentMove->verticalAdvance());
        resumeMove();
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
    m->animation()->startAnimation(AnimateForever);
    if (m->mode() == MovementTile)
    {
        float mx = m->horizontalAdvance() / m->totalDuration();
        float my = m->verticalAdvance()   / m->totalDuration();

        setMoveSpeed(mx, my);
        moveBy(m->horizontalAdvance(), m->verticalAdvance());

        m_isBlocking = true;
    }

    m_isRunning = true;
    m_currentMove = m;
}


void Sprite::beginIdle(const QString& n)
{
    if (m_isBlocking)
    {
        return;
    }

    // Tries to find the movement.
    auto* m = m_movements.value(n, nullptr);
    if (m == nullptr)
    {
        cranError(ERRARG_1(e_06, n));
        return;
    }

    // Starts the idle mode.
    m->animation()->startIdle();
    m_currentMove = m;
    m_isRunning = false;
}


void Sprite::resumeMove()
{
    if (m_isBlocking || !m_currentMove)
    {
        return;
    }

    m_currentMove->animation()->resumeAnimation();
    m_isRunning = true;
}


void Sprite::endMove()
{
    if (!m_currentMove)
    {
        return;
    }

    m_currentMove->animation()->stopAnimation();
    m_currentMove->animation()->endMove();
    m_currentMove->animation()->startIdle();

    m_isBlocking = false;
    m_isRunning = false;
}


void Sprite::update(const GameTime& time)
{
    if (!m_currentMove)
    {
        return;
    }

    m_currentMove->animation()->update(time);
    updateTransform(time);
}


void Sprite::render()
{
    if (!RenderBase::prepareRendering())
    {
        return;
    }

    m_currentMove->animation()->setShaderProgram(shaderProgram());
    m_currentMove->animation()->copyTransform(this, m_currentMove->animation());
    m_currentMove->animation()->render();
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
        m->animation()->setBlendColor(tl, tr, br, bl);
    }
}


void Sprite::setBlendMode(BlendModes modes)
{
    for (auto* const m : m_movements.values())
    {
        m->animation()->setBlendMode(modes);
    }
}


void Sprite::setEffect(Effect effect)
{
    for (auto* const m : m_movements.values())
    {
        m->animation()->setEffect(effect);
    }
}


QString getSpriteModeString(MovementMode mm)
{
    switch (mm)
    {
    case MovementDefault: return "Default";
    case MovementTile:    return "Tile";
    default:              return "Unknown";
    }
}


TreeModelItem* Sprite::rootModelItem()
{
    return m_rootModelItem;
}


void Sprite::createProperties(TreeModel* model)
{
    if (isNull())
    {
        m_rootModelItem = new TreeModelItem("This object is not valid.");
        model->addItem(m_rootModelItem);
        return;
    }
    else
    {
        m_currentMove->animation()->createProperties(nullptr);
    }

    TreeModelItem* tmiRunn = new TreeModelItem("Is running?", m_isRunning);
    TreeModelItem* tmiBloc = new TreeModelItem("Is blocking?", m_isBlocking);
    TreeModelItem* tmiMove = new TreeModelItem("Movements");
    TreeModelItem* tmiCurr = new TreeModelItem("Current movement");
    TreeModelItem* tmiCurN = new TreeModelItem("Name", m_currentMove->name());
    TreeModelItem* tmiCurR = new TreeModelItem("Idle frame");
    TreeModelItem* tmiCuRx = new TreeModelItem("x", m_currentMove->idleFrame().x());
    TreeModelItem* tmiCuRy = new TreeModelItem("y", m_currentMove->idleFrame().y());
    TreeModelItem* tmiCuRw = new TreeModelItem("w", m_currentMove->idleFrame().width());
    TreeModelItem* tmiCuRh = new TreeModelItem("h", m_currentMove->idleFrame().height());
    TreeModelItem* tmiAdvX = new TreeModelItem("Advance X", m_currentMove->horizontalAdvance());
    TreeModelItem* tmiAdvY = new TreeModelItem("Advance Y", m_currentMove->verticalAdvance());
    TreeModelItem* tmiMode = new TreeModelItem("Mode", getSpriteModeString(m_currentMove->mode()));

    m_rootModelItem = new TreeModelItem("Sprite");
    m_rootModelItem->appendChild(tmiRunn);
    m_rootModelItem->appendChild(tmiBloc);
    m_rootModelItem->appendChild(tmiMove);
    m_rootModelItem->appendChild(tmiCurr);

    Q_FOREACH (QString name, m_movements.keys())
    {
        tmiMove->appendChild(new TreeModelItem("Name", name));
    }

    tmiCurr->appendChild(tmiCurN);
    tmiCurr->appendChild(tmiCurR);
    tmiCurr->appendChild(tmiAdvX);
    tmiCurr->appendChild(tmiAdvY);
    tmiCurr->appendChild(tmiMode);
    tmiCurR->appendChild(tmiCuRx);
    tmiCurR->appendChild(tmiCuRy);
    tmiCurR->appendChild(tmiCuRw);
    tmiCurR->appendChild(tmiCuRh);
    tmiCurr->appendChild(m_currentMove->animation()->rootModelItem());

    model->addItem(m_rootModelItem);

    RenderBase::createProperties(model);
}


void Sprite::updateProperties()
{
    if (!isNull())
    {
        // Has current move changed?
        if (m_rootModelItem->childAt(3)->childAt(0)->value().toString() != m_currentMove->name())
        {
            m_currentMove->animation()->createProperties(nullptr);
            m_rootModelItem->childAt(3)->removeChild(m_rootModelItem->childAt(3)->childCount() - 1);
            m_rootModelItem->childAt(3)->appendChild(m_currentMove->animation()->rootModelItem());
        }
        else
        {
            m_currentMove->animation()->updateProperties();
        }

        m_rootModelItem->childAt(0)->setValue(m_isRunning);
        m_rootModelItem->childAt(1)->setValue(m_isBlocking);
        m_rootModelItem->childAt(3)->childAt(0)->setValue(m_currentMove->name());
        m_rootModelItem->childAt(3)->childAt(1)->childAt(0)->setValue(m_currentMove->idleFrame().x());
        m_rootModelItem->childAt(3)->childAt(1)->childAt(1)->setValue(m_currentMove->idleFrame().y());
        m_rootModelItem->childAt(3)->childAt(1)->childAt(2)->setValue(m_currentMove->idleFrame().width());
        m_rootModelItem->childAt(3)->childAt(1)->childAt(3)->setValue(m_currentMove->idleFrame().height());
        m_rootModelItem->childAt(3)->childAt(2)->setValue(m_currentMove->horizontalAdvance());
        m_rootModelItem->childAt(3)->childAt(3)->setValue(m_currentMove->verticalAdvance());
        m_rootModelItem->childAt(3)->childAt(4)->setValue(getSpriteModeString(m_currentMove->mode()));

        RenderBase::updateProperties();
    }
}

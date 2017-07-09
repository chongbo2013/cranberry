﻿////////////////////////////////////////////////////////////////////////////////
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
#include <Cranberry/Graphics/Sprite.hpp>
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


Sprite::Sprite()
    : IRenderable()
    , ITransformable()
    , m_currentMove(nullptr)
    , m_isRunning(false)
    , m_isBlocking(false)
{
}


Sprite::~Sprite()
{
    destroy();
}


bool Sprite::isNull() const
{
    return IRenderable::isNull() ||
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
    if (!IRenderable::create(rt)) return false;

    // Attempts to load the file.
    QFile file(path);
    if (file.open(QFile::ReadOnly))
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
        foreach (QJsonValue move, movements)
        {
            QJsonObject obj = move.toObject();
            QJsonValue name = obj.value("name");
            QJsonValue mode = obj.value("mode");
            QJsonValue advanceX = obj.value("advanceX");
            QJsonValue advanceY = obj.value("advanceY");
            QJsonObject idle = obj.value("idle").toObject();
            QJsonArray frames = obj.value("frames").toArray();

            if (name.isNull() || ((!mode.isNull() && mode.toString() == "tile") &&
               (advanceX.isNull() || advanceY.isNull())) || frames.isEmpty())
            {
                return cranError(ERRARG(e_04));
            }

            auto move = new Movement;
            move->name = name.toString();
            move->mode = (mode.toString() == "tile") ? MovementTile : MovementDefault;
            move->totalTime = 0.0;
            move->advanceX = advanceX.toDouble();
            move->advanceY = advanceY.toDouble();
            move->idle = getJsonRect(idle);
            move->anim = new IAnimation;

            QVector<IAnimation::Frame> animFrames;
            qint32 currentFrame = 0;

            // Loads the frames.
            foreach (QJsonValue frame, frames)
            {
                QJsonObject fobj = frame.toObject();
                QJsonValue duration = fobj.value("duration");
                QJsonObject rect = fobj.value("rect").toObject();

                if (duration.isNull() || rect.isEmpty())
                {
                    return cranError(ERRARG(e_05));
                }

                IAnimation::Frame f;
                f.atlas = 0;
                f.duration = duration / 1000.0;
                f.frame = currentFrame;
                f.rect = getJsonRect(rect);

                move->totalTime += f.duration;

                animFrames.append(f);
                currentFrame++;
            }

            if(!move->anim->createRawAnimation({ img }, animFrames, renderTarget())) return false;

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

    IRenderable::destroy();
}


void Sprite::runMovement(const QString& name)
{
    // Tries to find the movement.
    auto* m = m_movements.value(name, nullptr);
    if (m == nullptr)
    {
        cranError(ERRARG_1(e_06, name));
        return;
    }

    // Runs the animation.
    m->anim->startAnimation(AnimateForever);
    if (m->mode == MovementTile)
    {
        m->anim->startMovingBy(m->advanceX, m->advanceY);
        m_isBlocking = true;
    }

    m_isRunning = true;
    m_currentMove = m->anim;
}


void Sprite::runIdle(const QString& name)
{
    // Tries to find the movement.
    auto* m = m_movements.value(name, nullptr);
    if (m == nullptr)
    {
        cranError(ERRARG_1(e_06, name));
        return;
    }

    // Starts the idle mode.
    m->anim->startIdle();
    m_currentMove = m->anim;
    m_isBlocking = false;
    m_isRunning = false;
}


void Sprite::resumeMovement()
{
    if (!m_currentMove) return;

    m_currentMove->resumeAnimation();
    m_isRunning = true;
}


void Sprite::stopMovement()
{
    if (!m_currentMove) return;

    m_currentMove->stopAnimation();
    m_currentMove->stopMoving();
    m_currentMove = nullptr;
}


void Sprite::update(const GameTime& time)
{
    if (!m_currentMove) return;

    m_currentMove->update(time);
}


void Sprite::render()
{
    if (!m_currentMove) return;

    m_currentMove->setShaderProgram(shaderProgram());
    m_currentMove->setPosition(pos());
    m_currentMove->setAngle(angle());
    m_currentMove->setOpacity(opacity());
    m_currentMove->setScale(scaleX(), scaleY());
    m_currentMove->render();
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

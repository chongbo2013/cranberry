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
#include <Cranberry/Graphics/Base/ITransformable.hpp>

// Qt headers
#include <QTransform>
#include <QVector3D>


CRANBERRY_USING_NAMESPACE


ITransformable::ITransformable()
    : m_moveDir(MoveNone)
    , m_rotateDirX(RotateNone)
    , m_rotateDirY(RotateNone)
    , m_rotateDirZ(RotateNone)
    , m_scaleDirX(ScaleNone)
    , m_scaleDirY(ScaleNone)
    , m_fadeDir(FadeNone)
    , m_rotateAxes(AxisNone)
    , m_isMovingX(false)
    , m_isMovingY(false)
    , m_isRotating(false)
    , m_isScalingX(false)
    , m_isScalingY(false)
    , m_isFading(false)
    , m_speedMoveX(50.f)
    , m_speedMoveY(50.f)
    , m_speedRotateX(50.f)
    , m_speedRotateY(50.f)
    , m_speedRotateZ(50.f)
    , m_speedScaleX(50.f)
    , m_speedScaleY(50.f)
    , m_speedFade(1.f)
    , m_targetMoveX(0.f)
    , m_targetMoveY(0.f)
    , m_targetRotateX(0.f)
    , m_targetRotateY(0.f)
    , m_targetRotateZ(0.f)
    , m_targetScaleX(0.f)
    , m_targetScaleY(0.f)
    , m_targetOpacity(0.f)
    , m_x(0.f)
    , m_y(0.f)
    , m_width(0.f)
    , m_height(0.f)
    , m_angleX(0.f)
    , m_angleY(0.f)
    , m_angleZ(0.f)
    , m_scaleX(1.f)
    , m_scaleY(1.f)
    , m_opacity(1.f)
    , m_originX(0.f)
    , m_originY(0.f)
{
}


bool ITransformable::isMoving() const
{
    return m_isMovingX && m_isMovingY;
}


bool ITransformable::isRotating() const
{
    return m_isRotating;
}


bool ITransformable::isScaling() const
{
    return m_isScalingX && m_isScalingY;
}


bool ITransformable::isFading() const
{
    return m_isFading;
}


float ITransformable::x() const
{
    return m_x;
}


float ITransformable::y() const
{
    return m_y;
}


float ITransformable::angle() const
{
    return m_angleZ;
}


float ITransformable::angleX() const
{
    return m_angleX;
}


float ITransformable::angleY() const
{
    return m_angleY;
}


float ITransformable::angleZ() const
{
    return m_angleZ;
}



float ITransformable::scaleX() const
{
    return m_scaleX;
}


float ITransformable::scaleY() const
{
    return m_scaleY;
}


float ITransformable::width() const
{
    return m_width;
}


float ITransformable::height() const
{
    return m_height;
}


float ITransformable::opacity() const
{
    return m_opacity;
}


MoveDirections ITransformable::moveDirection() const
{
    return m_moveDir;
}


RotateDirection ITransformable::rotateDirectionX() const
{
    return m_rotateDirX;
}


RotateDirection ITransformable::rotateDirectionY() const
{
    return m_rotateDirY;
}


RotateDirection ITransformable::rotateDirectionZ() const
{
    return m_rotateDirZ;
}


ScaleDirection ITransformable::scaleDirectionX() const
{
    return m_scaleDirX;
}


ScaleDirection ITransformable::scaleDirectionY() const
{
    return m_scaleDirY;
}


FadeDirection ITransformable::fadeDirection() const
{
    return m_fadeDir;
}


QVector2D ITransformable::pos() const
{
    return QVector2D(m_x, m_y);
}


QVector3D ITransformable::origin() const
{
    return QVector3D(m_originX, m_originY, 0);
}


QVector3D ITransformable::rotateAxes() const
{
    QVector3D axes;

    if ((m_rotateAxes & AxisX) != 0) axes.setX(1);
    if ((m_rotateAxes & AxisY) != 0) axes.setY(1);
    if ((m_rotateAxes & AxisZ) != 0) axes.setZ(1);

    return axes;
}


QPainterPath ITransformable::bounds() const
{
    QPainterPath path;
    QTransform trans;

    if ((m_rotateAxes & AxisX) != 0) trans.rotate(m_angleX, Qt::XAxis);
    if ((m_rotateAxes & AxisY) != 0) trans.rotate(m_angleY, Qt::YAxis);
    if ((m_rotateAxes & AxisZ) != 0) trans.rotate(m_angleZ, Qt::ZAxis);

    trans.scale(m_scaleX, m_scaleY);
    path.addRect(m_x, m_y, m_width, m_height);

    return trans.map(path);
}


TransformableEmitter* ITransformable::transformableEmitter()
{
    return &m_emitter;
}


void ITransformable::setMoveSpeed(float speedX, float speedY)
{
    m_speedMoveX = speedX;
    m_speedMoveY = speedY;
}


void ITransformable::setMoveDirection(MoveDirections dir)
{
    m_moveDir = dir;
}


void ITransformable::setRotateSpeed(float speedX, float speedY, float speedZ)
{
    m_speedRotateX = speedX;
    m_speedRotateY = speedY;
    m_speedRotateZ = speedZ;
}


void ITransformable::setRotateDirection(RotateDirection dir)
{
    setRotateDirection(RotateNone, RotateNone, dir);
}


void ITransformable::setRotateDirection(
        RotateDirection dirX,
        RotateDirection dirY,
        RotateDirection dirZ
        )
{
    m_rotateDirX = dirX;
    m_rotateDirY = dirY;
    m_rotateDirZ = dirZ;
}


void ITransformable::setRotateAxes(RotateAxes axes)
{
    m_rotateAxes = axes;
}


void ITransformable::setScaleSpeed(float speedX, float speedY)
{
    m_speedScaleX = speedX;
    m_speedScaleY = speedY;
}


void ITransformable::setX(float x)
{
    m_x = x;
}


void ITransformable::setY(float y)
{
    m_y = y;
}


void ITransformable::setAngle(float degrees)
{
    setAngle(0, 0, degrees);
}


void ITransformable::setAngle(float x, float y, float z)
{
    m_angleX = x;
    m_angleY = y;
    m_angleZ = z;
}


void ITransformable::setScale(float scaleX, float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;
}


void ITransformable::setOpacity(float opacity)
{
    m_opacity = opacity;
}


void ITransformable::setPosition(const QVector2D& pos)
{
    m_x = pos.x();
    m_y = pos.y();
}


void ITransformable::setOrigin(const QVector2D& origin)
{
    m_originX = origin.x();
    m_originY = origin.y();
}


void ITransformable::startMoving(float advanceX, float advanceY)
{
    // Do not accept negative values. Use ITransformable::setMoveDirections
    // to specify the directions to move object in.
    advanceX = qAbs(advanceX);
    advanceY = qAbs(advanceY);

    // Calculates the target position.
    if ((m_moveDir & MoveEast) != 0 && (m_moveDir & MoveWest) == 0) m_targetMoveX = m_x + advanceX;
    else if ((m_moveDir & MoveEast) == 0 && (m_moveDir & MoveWest) != 0) m_targetMoveX = m_x - advanceX;
    if ((m_moveDir & MoveSouth) != 0 && (m_moveDir & MoveNorth) == 0) m_targetMoveY = m_y + advanceY;
    else if ((m_moveDir & MoveSouth) == 0 && (m_moveDir & MoveNorth) != 0) m_targetMoveY = m_y - advanceY;

    if ((m_moveDir & MoveEast)  || (m_moveDir & MoveWest))  m_isMovingX = true;
    if ((m_moveDir & MoveSouth) || (m_moveDir & MoveNorth)) m_isMovingY = true;
}


void ITransformable::startRotating(float advance)
{
    startRotating(0, 0, advance);
}


void ITransformable::startRotating(float advanceX, float advanceY, float advanceZ)
{
    // Do not accept negative values. Use ITransformable::setRotateDirection
    // to specify the direction to move object in.
    advanceX = qAbs(advanceX);
    advanceY = qAbs(advanceY);
    advanceZ = qAbs(advanceZ);

    // Calculates the target rotation.
    if (m_rotateDirX & RotateCW) m_targetRotateX = m_angleX + advanceX;
    else if (m_rotateDirY & RotateCCW) m_targetRotateX = m_angleX - advanceX;
    if (m_rotateDirY & RotateCW) m_targetRotateY = m_angleY + advanceY;
    else if (m_rotateDirY & RotateCCW) m_targetRotateY = m_angleY - advanceY;
    if (m_rotateDirZ & RotateCW) m_targetRotateZ = m_angleZ + advanceZ;
    else if (m_rotateDirZ & RotateCCW) m_targetRotateZ = m_angleZ - advanceZ;

    if (m_rotateDirX != RotateNone ||
        m_rotateDirY != RotateNone ||
        m_rotateDirZ != RotateNone)
    {
        m_isRotating = true;
    }
}


void ITransformable::startScaling(float scaleX, float scaleY)
{
    // Do not accept negative values.
    scaleX = qAbs(scaleX);
    scaleY = qAbs(scaleY);

    // Calculates the target scale.
    if (scaleX <= m_scaleX) m_scaleDirX = ScaleDown;
    else m_scaleDirX = ScaleUp;
    if (scaleY <= m_scaleY) m_scaleDirY = ScaleDown;
    else m_scaleDirY = ScaleUp;

    m_targetScaleX = scaleX;
    m_targetScaleY = scaleY;

    if (scaleX != 0) m_isScalingX = true;
    if (scaleY != 0) m_isScalingY = true;
}


void ITransformable::startFading(float target)
{
    // Do not accept negative values.
    target = qAbs(target);

    // Calculates the target opacity.
    if (target <= m_opacity) m_fadeDir = FadeOut;
    else m_fadeDir = FadeIn;

    m_targetOpacity = target;
    m_isFading = true;
}


void ITransformable::stopMoving()
{
    m_isMovingX = false;
    m_isMovingY = false;
    m_emitter.emitStoppedMoving();
}


void ITransformable::stopRotating()
{
    m_isRotating = false;
    m_emitter.emitStoppedRotating();
}


void ITransformable::stopScaling()
{
    m_isScalingX = false;
    m_isScalingY = false;
    m_emitter.emitStoppedScaling();
}


void ITransformable::stopFading()
{
    m_isFading = false;
    m_emitter.emitStoppedFading();
}


void ITransformable::updateTransform(const GameTime& time)
{
    if (isMoving()) updateMove(time.deltaTime());
    if (isRotating()) updateRotate(time.deltaTime());
    if (isScaling()) updateScale(time.deltaTime());
    if (isFading()) updateFade(time.deltaTime());
}


void ITransformable::setSize(float width, float height)
{
    m_width = width;
    m_height = height;
}


void ITransformable::updateMove(double delta)
{
    if ((m_moveDir & MoveEast) != 0 && (m_moveDir & MoveWest) == 0)
    {
        m_x += (m_speedMoveX * delta);
        if (m_x >= m_targetMoveX)
        {
            m_x = m_targetMoveX;
            m_isMovingX = false;
            checkMove();
        }
    }
    else if ((m_moveDir & MoveEast) == 0 && (m_moveDir & MoveWest) != 0)
    {
        m_x -= (m_speedMoveX * delta);
        if (m_x <= m_targetMoveX)
        {
            m_x = m_targetMoveX;
            m_isMovingX = false;
            checkMove();
        }
    }
    if ((m_moveDir & MoveSouth) != 0 && (m_moveDir & MoveNorth) == 0)
    {
        m_y += (m_speedMoveY * delta);
        if (m_y >= m_targetMoveY)
        {
            m_y = m_targetMoveY;
            m_isMovingY = false;
            checkMove();
        }
    }
    else if ((m_moveDir & MoveSouth) == 0 && (m_moveDir & MoveNorth) != 0)
    {
        m_y -= (m_speedMoveY * delta);
        if (m_y <= m_targetMoveY)
        {
            m_y = m_targetMoveY;
            m_isMovingY = false;
            checkMove();
        }
    }
}


void ITransformable::updateRotate(double delta)
{
    if (m_rotateDirX != RotateNone)
    {
        if (m_rotateDirX == RotateCW)
        {
            m_angleX += (m_speedRotateX * delta);
            if (m_angleX >= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                stopRotating();
            }
        }
        else
        {
            m_angleX -= (m_speedRotateX * delta);
            if (m_angleX <= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                stopRotating();
            }
        }
    }
    else if (m_rotateDirY != RotateNone)
    {
        if (m_rotateDirY == RotateCW)
        {
            m_angleY += (m_speedRotateY * delta);
            if (m_angleY >= m_targetRotateY)
            {
                m_angleY = m_targetRotateY;
                stopRotating();
            }
        }
        else
        {
            m_angleY -= (m_speedRotateY * delta);
            if (m_angleY <= m_targetRotateY)
            {
                m_angleY = m_targetRotateY;
                stopRotating();
            }
        }
    }
    else if (m_rotateDirZ != RotateNone)
    {
        if (m_rotateDirZ == RotateCW)
        {
            m_angleZ += (m_speedRotateZ * delta);
            if (m_angleZ >= m_targetRotateZ)
            {
                m_angleZ = m_targetRotateZ;
                stopRotating();
            }
        }
        else
        {
            m_angleZ -= (m_speedRotateZ * delta);
            if (m_angleZ <= m_targetRotateZ)
            {
                m_angleZ = m_targetRotateZ;
                stopRotating();
            }
        }
    }
}


void ITransformable::updateScale(double delta)
{
    if (m_scaleDirX == ScaleUp)
    {
        m_scaleX += (m_speedScaleX * delta);
        if (m_scaleX >= m_targetScaleX)
        {
            m_scaleX = m_targetScaleX;
            m_isScalingX = false;
            checkScale();
        }
    }
    else
    {
        m_scaleX -= (m_speedScaleX * delta);
        if (m_scaleX <= m_targetScaleX)
        {
            m_scaleX = m_targetScaleX;
            m_isScalingX = false;
            checkScale();
        }
    }

    if (m_scaleDirY == ScaleUp)
    {
        m_scaleY += (m_speedScaleY * delta);
        if (m_scaleY >= m_targetScaleY)
        {
            m_scaleY = m_targetScaleY;
            m_isScalingY = false;
            checkScale();
        }
    }
    else
    {
        m_scaleY -= (m_speedScaleY * delta);
        if (m_scaleY <= m_targetScaleY)
        {
            m_scaleY = m_targetScaleY;
            m_isScalingY = false;
            checkScale();
        }
    }
}


void ITransformable::updateFade(double delta)
{
    if (m_fadeDir == FadeIn)
    {
        m_opacity += (m_speedFade * delta);
        if (m_opacity >= m_targetOpacity)
        {
            m_opacity = m_targetOpacity;
            stopFading();
        }
    }
    else
    {
        m_opacity -= (m_speedFade * delta);
        if (m_opacity <= m_targetOpacity)
        {
            m_opacity = m_targetOpacity;
            stopFading();
        }
    }
}


void ITransformable::checkMove()
{
    if (!m_isMovingX && !m_isMovingY) stopMoving();
}


void ITransformable::checkScale()
{
    if (!m_isScalingX && !m_isScalingY) stopScaling();
}

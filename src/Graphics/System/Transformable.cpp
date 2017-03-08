//
//  cranberry: C++ game engine using the Qt framework and OpenGL/ES.
//  Copyright (C) 2017 Nicolas Kogler
//
//  Cranberry is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cranberry is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with cranberry. If not, see <http://www.gnu.org/licenses/>.
//


// Cranberry headers
#include <Cranberry/Graphics/System/Transformable.hpp>

// Qt headers
#include <QVector3D>


CRANBERRY_BEGIN_NAMESPACE


Transformable::Transformable()
    : m_isMoving(false)
    , m_isRotating(false)
    , m_isScaling(false)
    , m_rotatesX(false)
    , m_rotatesY(false)
    , m_rotatesZ(true)
    , m_dirMovement(0)
    , m_dirRotation(0)
    , m_dirScale(0)
    , m_speedMoveX(50.f)
    , m_speedMoveY(50.f)
    , m_speedRotate(50.f)
    , m_speedScale(50.f)
    , m_targetX(0.f)
    , m_targetY(0.f)
    , m_targetRotation(0.f)
    , m_targetScale(0.f)
    , m_x(0.f)
    , m_y(0.f)
    , m_angle(0.f)
    , m_scale(1.f)
    , m_originX(0.f)
    , m_originY(0.f)
{
}


bool Transformable::isMoving() const
{
    return m_isMoving;
}


bool Transformable::isRotating() const
{
    return m_isRotating;
}


bool Transformable::isScaling() const
{
    return m_isScaling;
}


float Transformable::x() const
{
    return m_x;
}


float Transformable::y() const
{
    return m_y;
}


float Transformable::angle() const
{
    return m_angle;
}


float Transformable::scale() const
{
    return m_scale;
}


MoveDirections Transformable::movementDirection() const
{
    return static_cast<MoveDirections>(m_dirMovement);
}


RotateDirection Transformable::rotationDirection() const
{
    return static_cast<RotateDirection>(m_dirRotation);
}


ScaleDirection Transformable::scaleDirection() const
{
    return static_cast<ScaleDirection>(m_dirScale);
}


QVector2D Transformable::pos() const
{
    return QVector2D(m_x, m_y);
}


QVector3D Transformable::origin() const
{
    return QVector3D(m_originX, m_originY, 0);
}


QVector3D Transformable::rotationAxis() const
{
    QVector3D axes;
    if (m_rotatesX)
        axes.setX(1);
    if (m_rotatesY)
        axes.setY(1);
    if (m_rotatesZ)
        axes.setZ(1);

    return axes;
}


void Transformable::setMovementSpeed(float speedX, float speedY)
{
    m_speedMoveX = speedX;
    m_speedMoveY = speedY;
}


void Transformable::setMovementDirection(MoveDirections dir)
{
    m_dirMovement = static_cast<int>(dir);
}


void Transformable::setRotationSpeed(float speed)
{
    m_speedRotate = speed;
}


void Transformable::setRotationDirection(RotateDirection dir)
{
    m_dirRotation = static_cast<int>(dir);
}


void Transformable::setRotationAxis(RotateAxes axes)
{
    m_rotatesX = (axes & AxisX);
    m_rotatesY = (axes & AxisY);
    m_rotatesZ = (axes & AxisZ);
}


void Transformable::setX(float x)
{
    m_x = x;
}


void Transformable::setY(float y)
{
    m_y = y;
}


void Transformable::setAngle(float degrees)
{
    m_angle = degrees;
}


void Transformable::setScale(float scale)
{
    m_scale = scale;
}


void Transformable::setPosition(const QVector2D& pos)
{
    m_x = pos.x();
    m_y = pos.y();
}


void Transformable::setOrigin(const QVector2D& origin)
{
    m_originX = origin.x();
    m_originY = origin.y();
}


void Transformable::startMovement(float byX, float byY)
{
    // Does not accept negative values. Directions are
    // specified in function setMovementDirection().
    byX = qAbs(byX);
    byY = qAbs(byY);

    // Calculates the target position.
    if ((m_dirMovement & MoveEast) && (m_dirMovement & MoveWest) == 0)
        m_targetX = m_x + byX;
    else if ((m_dirMovement & MoveWest) && (m_dirMovement & MoveEast) == 0)
        m_targetX = m_x - byX;
    if ((m_dirMovement & MoveSouth) && (m_dirMovement & MoveNorth) == 0)
        m_targetY = m_y + byY;
    else if ((m_dirMovement & MoveNorth) && (m_dirMovement & MoveSouth) == 0)
        m_targetY = m_y - byY;

    if (m_dirMovement != MoveNone)
        m_isMoving = true;
}


void Transformable::startRotation(float byDegrees)
{
    // Does not accept negative values. Direction
    // is specified in function setRotationDirection().
    byDegrees = qAbs(byDegrees);

    // Calculates the target rotation.
    if (m_dirRotation == RotateCW)
        m_targetRotation = m_angle + byDegrees;
    else
        m_targetRotation = m_angle - byDegrees;

    m_isRotating = true;
}


void Transformable::startScale(float byFactor)
{
    // Does not accept negative values. Direction
    // is specified in function setScaleDirection().
    byFactor = qAbs(byFactor);

    // Calculates the target scale.
    if (m_dirScale == ScaleUp)
        m_targetScale = m_scale * byFactor;
    else
        m_targetScale = m_scale / byFactor;

    m_isScaling = true;
}


void Transformable::updateTransform(const GameTime& time)
{
    // Updates the object's position.
    if (m_isMoving)
    {
        if ((m_dirMovement & MoveEast) && (m_dirMovement & MoveWest) == 0)
        {
            m_x += (m_speedMoveX * time.deltaTime());
            if (m_x >= m_targetX)
            {
                m_x = m_targetX;
                m_isMoving = false;
            }
        }
        else if ((m_dirMovement & MoveWest) && (m_dirMovement & MoveEast) == 0)
        {
            m_x -= (m_speedMoveX * time.deltaTime());
            if (m_x <= m_targetX)
            {
                m_x = m_targetX;
                m_isMoving = false;
            }
        }
        if ((m_dirMovement & MoveSouth) && (m_dirMovement & MoveNorth) == 0)
        {
            m_y += (m_speedMoveY * time.deltaTime());
            if (m_y >= m_targetY)
            {
                m_y = m_targetY;
                m_isMoving = false;
            }
        }
        else if ((m_dirMovement & MoveNorth) && (m_dirMovement & MoveSouth) == 0)
        {
            m_y -= (m_speedMoveY * time.deltaTime());
            if (m_y <= m_targetY)
            {
                m_y = m_targetY;
                m_isMoving = false;
            }
        }
    }

    // Updates the object's rotation.
    if (m_isRotating)
    {
        if (m_dirRotation == RotateCW)
        {
            m_angle += (m_speedRotate * time.deltaTime());
            if (m_angle >= m_targetRotation)
            {
                m_angle = m_targetRotation;
                m_isRotating = false;
            }
        }
        else
        {
            m_angle -= (m_speedRotate * time.deltaTime());
            if (m_angle <= m_targetRotation)
            {
                m_angle = m_targetRotation;
                m_isRotating = false;
            }
        }
    }

    // Updates the object's scale.
    if (m_isScaling)
    {
        if (m_dirRotation == ScaleUp)
        {
            m_scale += (m_speedScale * time.deltaTime());
            if (m_scale >= m_targetScale)
            {
                m_scale = m_targetScale;
                m_isScaling = false;
            }
        }
        else
        {
            m_scale -= (m_speedScale * time.deltaTime());
            if (m_scale <= m_targetScale)
            {
                m_scale = m_targetScale;
                m_isScaling = false;
            }
        }
    }
}


CRANBERRY_END_NAMESPACE

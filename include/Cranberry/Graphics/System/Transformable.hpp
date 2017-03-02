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


#pragma once
#ifndef CRANBERRY_TRANSFORMABLE_HPP
#define CRANBERRY_TRANSFORMABLE_HPP


// Cranberry headers
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/Graphics/System/GraphicsEnums.hpp>

// Qt headers
#include <QVector2D>


CRANBERRY_BEGIN_NAMESPACE


///
/// Provides functions for rotating, scaling
/// or moving the sprite on the render target.
///
/// \class Transformable
/// \author Nicolas Kogler
/// \date Feburary 16, 2017
///
class CRANBERRY_EXPORT Transformable
{
public:

    CRANBERRY_DEFAULT_DTOR(Transformable)
    CRANBERRY_DEFAULT_COPY(Transformable)
    CRANBERRY_DISABLE_MOVE(Transformable)

    ///
    /// Initializes a new instance of Transformable.
    ///
    Transformable();


    ///
    /// Determines whether this object is moving.
    ///
    /// \returns true if moving.
    ///
    bool isMoving() const;

    ///
    /// Determines whether this object is rotating.
    ///
    /// \returns true if rotating.
    ///
    bool isRotating() const;

    ///
    /// Determines whether this object is scaling.
    ///
    /// \returns true if scaling.
    ///
    bool isScaling() const;

    ///
    /// Retrieves the X-position of the object.
    ///
    /// \returns the X-position.
    ///
    float x() const;

    ///
    /// Retrieves the Y-position of the object.
    ///
    /// \returns the Y-position.
    ///
    float y() const;

    ///
    /// Retrieves the rotation of the object.
    ///
    /// \returns the rotation.
    ///
    float angle() const;

    ///
    /// Retrieves the scale of the object.
    ///
    /// \returns the scale.
    ///
    float scale() const;

    ///
    /// Retrieves the movement direction.
    ///
    /// \returns the movement direction.
    ///
    MoveDirections movementDirection() const;

    ///
    /// Retrieves the rotation direction.
    ///
    /// \returns the rotation direction.
    ///
    RotateDirection rotationDirection() const;

    ///
    /// Retrieves the scale direction.
    ///
    /// \returns the scale direction.
    ///
    ScaleDirection scaleDirection() const;

    ///
    /// Retrieves the position as vector.
    ///
    /// \returns the position.
    ///
    QVector2D pos() const;

    ///
    /// Retrieves the origin position.
    ///
    /// \returns the origin.
    ///
    QVector3D origin() const;

    ///
    /// Returns the rotation axes for this object.
    ///
    /// \returns one or multiple rotation axes.
    ///
    QVector3D rotationAxis() const;


    ///
    /// Specifies the movement speed of the object.
    ///
    /// \param speedX Horizontal speed.
    /// \param speedY Vertical speed.
    ///
    void setMovementSpeed(float speedX, float speedY);

    ///
    /// Specifies the movement direction. Multiple
    /// movement directions may be combined.
    ///
    /// \param dir MovementDirection enum value(s).
    ///
    void setMovementDirection(MoveDirections dir);

    ///
    /// Specifies the rotation speed of the object.
    ///
    /// \param speed Rotation speed.
    ///
    void setRotationSpeed(float speed);

    ///
    /// Specifies the rotation direction.
    ///
    /// \param dir RotationDirection enum value.
    ///
    void setRotationDirection(RotateDirection dir);

    ///
    /// Specifies the rotation axis. Multiple
    /// rotation axes may be combined.
    ///
    /// \param axes RotateAxis enum value(s).
    ///
    void setRotationAxis(RotateAxes axes);

    ///
    /// Specifies the scaling speed.
    ///
    /// \param speed Scaling speed.
    ///
    void setScaleSpeed(float speed);

    ///
    /// Specifies the scaling direction.
    ///
    /// \param dir ScaleDirection enum value.
    ///
    void setScaleDirection(ScaleDirection dir);

    ///
    /// Specifies the X-position of the object.
    ///
    /// \param x Object X-position.
    ///
    void setX(float x);

    ///
    /// Specifies the Y-position of the object.
    ///
    /// \param y Object Y-position.
    ///
    void setY(float y);

    ///
    /// Specifies the rotation angle of the object.
    ///
    /// \param degrees Angle in degrees.
    ///
    void setAngle(float degrees);

    ///
    /// Specifies the scale factor of the object.
    ///
    /// \param scale Scale factor.
    ///
    void setScale(float scale);

    ///
    /// Specifies the position of the object.
    ///
    /// \param pos Vector containing X and Y.
    ///
    void setPosition(const QVector2D& pos);

    ///
    /// Specifies the origin of the object.
    ///
    /// \param origin Vector containing X and Y.
    ///
    void setOrigin(const QVector2D& origin);


    ///
    /// Starts moving the object.
    ///
    /// \param byX Horizontal pixels to move the object by.
    /// \param byY Vertical pixels to move the object by.
    ///
    void startMovement(float byX, float byY);

    ///
    /// Starts rotating the object.
    ///
    /// \param byDegrees Degrees to rotate object by.
    ///
    void startRotation(float byDegrees);

    ///
    /// Starts scaling the object.
    ///
    /// \param byFactor Factor to scale object with.
    ///
    void startScale(float byFactor);


protected:

    ///
    /// Updates scale, rotation and movement.
    ///
    /// \param time Holds the delta time.
    ///
    void updateTransform(const GameTime& time);


private:

    // Members
    bool    m_isMoving;
    bool    m_isRotating;
    bool    m_isScaling;
    bool    m_rotatesX;
    bool    m_rotatesY;
    bool    m_rotatesZ;
    int     m_dirMovement;
    int     m_dirRotation;
    int     m_dirScale;
    float   m_speedMoveX;
    float   m_speedMoveY;
    float   m_speedRotate;
    float   m_speedScale;
    float   m_targetX;
    float   m_targetY;
    float   m_targetRotation;
    float   m_targetScale;
    float   m_x;
    float   m_y;
    float   m_angle;
    float   m_scale;
    float   m_originX;
    float   m_originY;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_TRANSFORMABLE_HPP

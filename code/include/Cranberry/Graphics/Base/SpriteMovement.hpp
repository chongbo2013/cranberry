﻿////////////////////////////////////////////////////////////////////////////////
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


#pragma once
#ifndef CRANBERRY_GRAPHICS_BASE_SPRITEMOVEMENT_HPP
#define CRANBERRY_GRAPHICS_BASE_SPRITEMOVEMENT_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>

// Qt headers
#include <QRectF>
#include <QString>

// Forward declarations
CRANBERRY_FORWARD_C(RawAnimation)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines one movement consisting of multiple frames for a sprite.
///
/// \class SpriteMovement
/// \author Nicolas Kogler
/// \date July 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT SpriteMovement final
{
public:

    CRANBERRY_DECLARE_CTOR(SpriteMovement)
    CRANBERRY_DECLARE_DTOR(SpriteMovement)
    CRANBERRY_DEFAULT_COPY(SpriteMovement)
    CRANBERRY_DEFAULT_MOVE(SpriteMovement)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this movement.
    ///
    /// \returns the movement's name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the idle frame rectangle.
    ///
    /// \returns the idle frame.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QRectF& idleFrame() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the horizontal advance, in pixels.
    ///
    /// \returns the horizontal advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    qreal horizontalAdvance() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the vertical advance, in pixels.
    ///
    /// \returns the vertical advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    qreal verticalAdvance() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the total movement duration.
    ///
    /// \returns the total duration.
    ///
    ////////////////////////////////////////////////////////////////////////////
    qreal totalDuration() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the movement mode.
    ///
    /// \returns the movement mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MovementMode mode() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the underlying raw animation.
    ///
    /// \returns the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RawAnimation* animation() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the movement name.
    ///
    /// \param name New name of movement.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the idle frame rectangle.
    ///
    /// \param frame Idle frame.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setIdleFrame(const QRectF& frame);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the idle frame rectangle.
    ///
    /// \param x X-position.
    /// \param y Y-position.
    /// \param width Frame width.
    /// \param height Frame height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setIdleFrame(qreal x, qreal y, qreal width, qreal height);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the horizontal advance, in pixels.
    ///
    /// \param amount Horizontal advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setHorizontalAdvance(qreal amount);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the vertical advance, in pixels.
    ///
    /// \param amount Vertical advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setVerticalAdvance(qreal amount);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the horizontal advance, in seconds.
    ///
    /// \param total Seconds the movement persists.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTotalDuration(qreal total);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the movement mode.
    ///
    /// \param mode Movement mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMovementMode(MovementMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the raw animation. Takes ownership of the animation and
    /// destroys it upon disposal.
    ///
    /// \param anim Animation to set.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRawAnimation(RawAnimation* anim);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QString       m_name;
    QRectF        m_rect;
    qreal         m_advanceX;
    qreal         m_advanceY;
    qreal         m_duration;
    MovementMode  m_mode;
    RawAnimation* m_anim;
};


CRANBERRY_END_NAMESPACE


#endif

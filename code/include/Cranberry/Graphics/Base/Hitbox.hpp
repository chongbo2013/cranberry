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


#pragma once
#ifndef CRANBERRY_HITBOX_HPP
#define CRANBERRY_HITBOX_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Forward declarations
CRANBERRY_FORWARD_Q(QPainterPath)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a hitbox for a graphics object.
///
/// \class Hitbox
/// \author Nicolas Kogler
/// \date July 23, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Hitbox
{
public:

    CRANBERRY_DECLARE_CTOR(Hitbox)
    CRANBERRY_DECLARE_DTOR(Hitbox)
    CRANBERRY_DISABLE_COPY(Hitbox)
    CRANBERRY_DISABLE_MOVE(Hitbox)

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the QPainterPath for this hitbox. This method is only used
    /// internally by cranberry in order to reuse hitbox objects.
    ///
    /// \param pp Painter path describing the bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setHitbox(QPainterPath* pp);

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this and the other hitbox intersect with each other.
    ///
    /// \param other Other hitbox.
    /// \returns true if they intersect.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool intersectsWith(const Hitbox& other) const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QPainterPath* m_pp; ///< QtGui painter path object
};


////////////////////////////////////////////////////////////////////////////////
/// \class Hitbox
/// \ingroup Graphics
///
/// You will need this class in order to detect collisions between objects.
///
/// \code
/// // Both devire from TransformBase.
/// Polygon* m_polygon;
/// Sprite* m_sprite;
///
/// ...
///
/// // Warning: We create hitboxes on the fly, since it is hard to track changes
/// // to it, because literally everything can alter it. If you need one hitbox
/// // multiple times, store it
/// if (m_polygon->hitbox().intersectsWith(m_sprite->hitbox())
/// {
///     // some game logic
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

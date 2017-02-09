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
#ifndef CRANBERRY_MOUSEMOVEEVENT_HPP
#define CRANBERRY_MOUSEMOVEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds information about a mouse-move event.
///
/// \class MouseMoveEvent
/// \author Nicolas Kogler
/// \date February 9, 2017
///
class CRANBERRY_EXPORT MouseMoveEvent
{
public:

    CRANBERRY_DISABLE_COPY(MouseMoveEvent)
    CRANBERRY_DISABLE_MOVE(MouseMoveEvent)
    CRANBERRY_DEFAULT_DTOR(MouseMoveEvent)

    ///
    /// Initializes a new instance of MouseMoveEvent.
    ///
    /// \param posOld The previous cursor position.
    /// \param posNew The new cursor position.
    ///
    MouseMoveEvent(const QPoint& posOld, const QPoint& posNew);


    ///
    /// Retrieves the current cursor position.
    ///
    /// \returns the cursor position.
    ///
    const QPoint& pos() const;

    ///
    /// Retrieves the difference between the old and new position.
    ///
    /// \returns the amount of pixels the cursor moved.
    ///
    const QPoint& delta() const;


private:

    // Members
    QPoint m_pos;
    QPoint m_delta;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_MOUSEMOVEEVENT_HPP

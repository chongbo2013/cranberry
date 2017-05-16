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
#ifndef CRANBERRY_MOUSERELEASEEVENT_HPP
#define CRANBERRY_MOUSERELEASEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>
#include <qnamespace.h>


CRANBERRY_BEGIN_NAMESPACE


///
/// This is the description of the class.
///
/// \class MouseReleaseEvent
/// \author Nicolas Kogler
/// \date February 8, 2017
///
class CRANBERRY_EXPORT MouseReleaseEvent
{
public:

    CRANBERRY_DISABLE_COPY(MouseReleaseEvent)
    CRANBERRY_DISABLE_MOVE(MouseReleaseEvent)
    CRANBERRY_DEFAULT_DTOR(MouseReleaseEvent)

    ///
    /// Initializes a new instance of MouseReleaseEvent.
    ///
    /// \param pos Current mouse position.
    /// \param btns The released buttons.
    ///
    MouseReleaseEvent(const QPoint& pos, Qt::MouseButtons btns);


    ///
    /// Retrieves the current cursor position.
    ///
    /// \returns the cursor position.
    ///
    const QPoint& pos() const;

    ///
    /// Returns a combination of the affected mouse buttons. In
    /// order to easily determine whether a specific button is
    /// affected, use the hasButton() function.
    ///
    /// \returns a combination of mouse buttons.
    ///
    Qt::MouseButtons buttons() const;

    ///
    /// Determines whether the given button was released.
    ///
    /// \returns true if it was just released.
    ///
    bool hasButton(Qt::MouseButton button) const;


private:

    // Members
    QPoint m_pos;
    Qt::MouseButtons m_btns;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_MOUSERELEASEEVENT_HPP

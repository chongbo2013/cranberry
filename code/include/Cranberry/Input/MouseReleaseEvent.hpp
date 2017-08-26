////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#ifndef CRANBERRY_INPUT_MOUSERELEASEEVENT_HPP
#define CRANBERRY_INPUT_MOUSERELEASEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the mouse button that was just released.
///
/// \class MouseReleaseEvent
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT MouseReleaseEvent final
{
public:

    CRANBERRY_DEFAULT_CTOR(MouseReleaseEvent)
    CRANBERRY_DEFAULT_DTOR(MouseReleaseEvent)
    CRANBERRY_DEFAULT_COPY(MouseReleaseEvent)
    CRANBERRY_DEFAULT_MOVE(MouseReleaseEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new MouseReleaseEvent with pos and buttons.
    ///
    /// \param pos Position of the mouse cursor.
    /// \param buttons Released mouse buttons.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MouseReleaseEvent(const QPoint& pos, int buttons);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current mouse cursor position.
    ///
    /// \returns the current cursor position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QPoint& pos() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this specific \p button was released or not.
    ///
    /// \param button Qt::MouseButton to check for.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool wasReleased(int button) const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QPoint m_pos;     ///< Current mouse position
    int    m_buttons; ///< One or multiple mouse buttons
};


////////////////////////////////////////////////////////////////////////////////
/// \class MouseReleaseEvent
/// \ingroup Input
///
/// An instance of this class is passed to Window::onMouseReleased(). There it
/// can be checked against any Qt::MouseButton using the function
/// MouseReleaseEvent::wasReleased().
///
/// \code
/// void onMouseReleased(const MouseReleaseEvent& event)
/// {
///     if (event.wasReleased(Qt::LeftButton))
///     {
///         // do stuff
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

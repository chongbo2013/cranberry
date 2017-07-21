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


#pragma once
#ifndef CRANBERRY_INPUT_MOUSEMOVEEVENT_HPP
#define CRANBERRY_INPUT_MOUSEMOVEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the current mouse position and the delta (current - last).
///
/// \class MouseMoveEvent
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT MouseMoveEvent
{
public:

    CRANBERRY_DEFAULT_DTOR(MouseMoveEvent)
    CRANBERRY_DEFAULT_COPY(MouseMoveEvent)
    CRANBERRY_DEFAULT_MOVE(MouseMoveEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a MouseMoveEvent with the old and the new mouse position.
    ///
    /// \param pOld Old cursor pos.
    /// \param pNew New cursor pos.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MouseMoveEvent(const QPoint& pOld, const QPoint& pNew);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current mouse cursor position.
    ///
    /// \returns the cursor pos.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QPoint& pos() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of pixels the cursor moved since last time.
    ///
    /// \returns the delta position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QPoint& delta() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QPoint m_pos;
    QPoint m_delta;
};


////////////////////////////////////////////////////////////////////////////////
/// \class MouseMoveEvent
/// \ingroup Input
///
/// An instance of this class is passed to Window::onMouseMoved(). There, the
/// mouse position can be retrieved and some magic stuff can be done.
///
/// \code
/// void onMouseMoved(const MouseMoveEvent& event)
/// {
///     movePlayer(event.pos());
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

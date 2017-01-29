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


#ifndef CRANBERRY_CBMOUSEMOVEEVENT_HPP
#define CRANBERRY_CBMOUSEMOVEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>


CRANBERRY_BEGIN_NAMESPACE


//!
//! Holds information about a mouse-move event.
//!
//! @class cbMouseMoveEvent
//! @author Nicolas Kogler
//! @date January 29th, 2017
//!
class CRANBERRY_EXPORT cbMouseMoveEvent
{
public:

    //!
    //! Constructor
    //!
    cbMouseMoveEvent(const QPoint& pOld, const QPoint& pNew);

    //!
    //! Copy constructor (disabled)
    //!
    //! @param other The other cbMouseMoveEvent instance.
    //!
    cbMouseMoveEvent(const cbMouseMoveEvent& other) = delete;

    //!
    //! Move constructor (disabled)
    //!
    //! @param other The other cbMouseMoveEvent instance.
    //!
    cbMouseMoveEvent(cbMouseMoveEvent&& other) = delete;

    //!
    //! Assignment operator (disabled)
    //!
    //! @param other The other cbMouseMoveEvent instance.
    //!
    cbMouseMoveEvent& operator=(const cbMouseMoveEvent& other) = delete;

    //!
    //! Destructor
    //!
    ~cbMouseMoveEvent() = default;


    //!
    //! Retrieves the current position of the cursor.
    //!
    //! @returns the cursor position.
    //!
    const QPoint& pos() const;

    //!
    //! Retrieves the difference between the old and new position.
    //!
    //! @returns the amount of moved pixels.
    //!
    const QPoint& delta() const;


private:

    // Members
    QPoint m_pos;
    QPoint m_delta;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBMOUSEMOVEEVENT_HPP

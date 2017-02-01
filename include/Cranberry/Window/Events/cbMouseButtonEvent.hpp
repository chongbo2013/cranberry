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


#ifndef CRANBERRY_CBMOUSEBUTTONEVENT_HPP
#define CRANBERRY_CBMOUSEBUTTONEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QPoint>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds information about a mouse-button event.
///
/// \class cbMouseButtonEvent
/// \author Nicolas Kogler
/// \date January 29th, 2017
///
class CRANBERRY_EXPORT cbMouseButtonEvent
{
public:

    ///
    /// Constructor
    ///
    cbMouseButtonEvent(const QPoint& p, Qt::MouseButtons buttons);

    ///
    /// Copy constructor
    ///
    /// \param other The other cbMouseButtonEvent instance.
    ///
    cbMouseButtonEvent(const cbMouseButtonEvent& other) = delete;

    ///
    /// Move constructor
    ///
    /// \param other The other cbMouseButtonEvent instance.
    ///
    cbMouseButtonEvent(cbMouseButtonEvent&& other) = delete;

    ///
    /// Assignment operator
    ///
    /// \param other The other cbMouseButtonEvent instance.
    ///
    cbMouseButtonEvent& operator=(const cbMouseButtonEvent& other) = delete;

    ///
    /// Destructor
    ///
    ~cbMouseButtonEvent() = default;


    ///
    /// Retrieves the current position of the cursor.
    ///
    /// \returns the cursor position.
    ///
    const QPoint& pos() const;

    ///
    /// Returns a combination of affected mouse buttons. In order
    /// to easily determine whether a specific button is affected,
    /// use the cbMouseButtonEvent::hasButton() function.
    ///
    /// \returns a combination of affected mouse buttons.
    ///
    Qt::MouseButtons buttons() const;

    ///
    /// Determines whether the given button is affected.
    ///
    /// \returns true if it is affected.
    ///
    bool hasButton(Qt::MouseButton button) const;


private:

    // Members
    QPoint m_pos;
    Qt::MouseButtons m_flags;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBMOUSEBUTTONEVENT_HPP

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


#ifndef CRANBERRY_CBKEYUPEVENT_HPP
#define CRANBERRY_CBKEYUPEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


//!
//! Holds information about a key-up event.
//!
//! @class cbKeyUpEvent
//! @author Nicolas Kogler
//! @date January 29th, 2017
//!
class CRANBERRY_EXPORT cbKeyUpEvent
{
public:

    //!
    //! Constructor
    //!
    cbKeyUpEvent(int key, Qt::KeyboardModifiers mods);

    //!
    //! Copy constructor
    //!
    //! @param other The other cbKeyUpEvent instance.
    //!
    cbKeyUpEvent(const cbKeyUpEvent& other) = delete;

    //!
    //! Move constructor
    //!
    //! @param other The other cbKeyUpEvent instance.
    //!
    cbKeyUpEvent(cbKeyUpEvent&& other) = delete;

    //!
    //! Assignment operator
    //!
    //! @param other The other cbKeyUpEvent instance.
    //!
    cbKeyUpEvent& operator=(const cbKeyUpEvent& other) = delete;

    //!
    //! Destructor
    //!
    ~cbKeyUpEvent() = default;


    //!
    //! Retrieves the released key.
    //!
    //! @param key The key that was released.
    //!
    Qt::Key key() const;

    //!
    //! Retrieves all pressed modifiers. In order to
    //! easily determine whether a specific modifier
    //! is pressed, use cbKeyUpEvent::hasModifier().
    //!
    //! @returns all pressed modifiers.
    //!
    Qt::KeyboardModifiers modifiers() const;

    //!
    //! Determines whether the given modifier is pressed.
    //!
    //! @returns true if the modifier is pressed.
    //!
    bool hasModifier(Qt::KeyboardModifier mod) const;


private:

    // Members
    Qt::Key m_key;
    Qt::KeyboardModifiers m_mods;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBKEYUPEVENT_HPP

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


#ifndef CRANBERRY_CBKEYBOARDSTATE_HPP
#define CRANBERRY_CBKEYBOARDSTATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QHash>


CRANBERRY_BEGIN_NAMESPACE


//!
//! Holds all currently pressed keys and modifiers.
//!
//! @class cbKeyboardState
//! @author Nicolas Kogler
//! @date January 29th, 2017
//!
class CRANBERRY_EXPORT cbKeyboardState
{
public:

    //!
    //! Constructor
    //!
    cbKeyboardState() = default;

    //!
    //! Copy constructor
    //!
    //! @param other The other cbKeyboardState instance.
    //!
    cbKeyboardState(const cbKeyboardState& other) = delete;

    //!
    //! Move constructor
    //!
    //! @param other The other cbKeyboardState instance.
    //!
    cbKeyboardState(cbKeyboardState&& other) = delete;

    //!
    //! Assignment operator
    //!
    //! @param other The other cbKeyboardState instance.
    //!
    cbKeyboardState& operator=(const cbKeyboardState& other) = delete;

    //!
    //! Destructor
    //!
    ~cbKeyboardState() = default;


    //!
    //! Determines whether the given modifier is pressed.
    //!
    //! @returns true if the modifier is pressed.
    //!
    bool isModifierDown(Qt::Modifier mod) const;

    //!
    //! Determines whether the given modifier is released.
    //!
    //! @returns true if the modifier is released.
    //!
    bool isModifierUp(Qt::Modifier mod) const;

    //!
    //! Determines whether the given key is pressed.
    //!
    //! @returns true if the key is pressed.
    //!
    bool isKeyDown(Qt::Key key) const;

    //!
    //! Determines whether the given key is released.
    //!
    //! @returns true if the key is released.
    //!
    bool isKeyUp(Qt::Key key) const;


private:

    // Members
    QHash<int, bool>        m_keys;
    Qt::KeyboardModifiers   m_mods;

    // Friends
    friend class cbWindow;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBKEYBOARDSTATE_HPP

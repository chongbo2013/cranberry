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
#ifndef CRANBERRY_KEYBOARDSTATE_HPP
#define CRANBERRY_KEYBOARDSTATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QHash>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds all currently pressed keys and modifiers.
///
/// \class KeyboardState
/// \author Nicolas Kogler
/// \date February 8, 2017
///
class CRANBERRY_EXPORT KeyboardState
{
public:

    CRANBERRY_DEFAULT_CTOR(KeyboardState)
    CRANBERRY_DISABLE_COPY(KeyboardState)
    CRANBERRY_DISABLE_MOVE(KeyboardState)
    CRANBERRY_DEFAULT_DTOR(KeyboardState)


    ///
    /// Determines whether the given modifier is pressed.
    ///
    /// \returns true if the modifier is pressed.
    ///
    bool isModifierDown(Qt::Modifier mod) const;

    ///
    /// Determines whether the given modifier is released.
    ///
    /// \returns true if the modifier is released.
    ///
    bool isModifierUp(Qt::Modifier mod) const;

    ///
    /// Determines whether the given key is pressed.
    ///
    /// \returns true if the key is pressed.
    ///
    bool isKeyDown(Qt::Key key) const;

    ///
    /// Determines whether the given key is released.
    ///
    /// \returns true if the key is released.
    ///
    bool isKeyUp(Qt::Key key) const;


private:

    // Members
    QHash<int, bool>        m_keys;
    Qt::KeyboardModifiers   m_mods;

    // Friends
    friend class Window;
};


CRANBERRY_END_NAMESPACE



#endif  // CRANBERRY_KEYBOARDSTATE_HPP

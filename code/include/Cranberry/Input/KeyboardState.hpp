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
#ifndef CRANBERRY_INPUT_KEYBOARDSTATE_HPP
#define CRANBERRY_INPUT_KEYBOARDSTATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QHash>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds all the keys that are currently pressed.
///
/// \class KeyboardState
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT KeyboardState
{
public:

    CRANBERRY_DEFAULT_CTOR(KeyboardState)
    CRANBERRY_DEFAULT_DTOR(KeyboardState)
    CRANBERRY_DEFAULT_COPY(KeyboardState)
    CRANBERRY_DEFAULT_MOVE(KeyboardState)


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given key is pressed.
    ///
    /// \returns true if the key is pressed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isKeyDown(int key) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given key is released.
    ///
    /// \returns true if the key is released.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isKeyUp(int key) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given modifier is pressed.
    ///
    /// \returns true if the modifier is pressed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isModifierDown(int mod) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given modifier is released.
    ///
    /// \returns true if the modifier is released.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isModifierUp(int mod) const;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the \p state for the given \p key.
    ///
    /// \param key Key to change state from.
    /// \param state State to assign to key.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setKeyState(int key, bool state);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the modifiers for this state.
    ///
    /// \param mods Currently pressed modifiers.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setModifiers(int mods);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QHash<int, bool> m_keyStates; ///< true = pressed, false = released
    int              m_modifiers; ///< One or more modifiers combined
};


////////////////////////////////////////////////////////////////////////////////
/// \class KeyboardState
/// \ingroup Input
///
/// The keyboard's key states will be stored in a hash map since Qt
/// unfortunately has very high key values. Use one of the four convenient
/// functions to determine whether a key or modifier is pressed.
///
/// \code
/// void onKeyPressed(const KeyboardState& keyboard)
/// {
///     if (keyboard.isKeyDown(Qt::Key_V) &&
///         keyboard.isModifierDown(Qt::ControlModifier))
///     {
///         // strg + v pressed!
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

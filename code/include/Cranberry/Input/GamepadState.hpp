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
#ifndef CRANBERRY_GAMEPADSTATE_HPP
#define CRANBERRY_GAMEPADSTATE_HPP


// Cranberry headers
#include <Cranberry/Input/Enumerations.hpp>

// Forward declarations
CRANBERRY_FORWARD_Q(QGamepad)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the gamepad and all its states.
///
/// \class GamepadState
/// \author Nicolas Kogler
/// \date June 10, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT GamepadState
{
public:

    CRANBERRY_DEFAULT_CTOR(GamepadState)
    CRANBERRY_DEFAULT_DTOR(GamepadState)
    CRANBERRY_DEFAULT_COPY(GamepadState)
    CRANBERRY_DEFAULT_MOVE(GamepadState)


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the associated gamepad.
    ///
    /// \returns the gamepad.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QGamepad* gamepad() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given button is pressed.
    ///
    /// \param button cran::GamepadButton to check.
    /// \returns true if the button is pressed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isButtonDown(GamepadButton button) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given button is released.
    ///
    /// \param button cran::GamepadButton to check.
    /// \returns true if the button is released.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isButtonUp(GamepadButton button) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the value of the \p axis specified.
    ///
    /// \param axis cran::GamepadAxis to check.
    /// \returns the value of the given \p axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    double axisValue(GamepadAxis axis) const;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the \p state for the given \p key.
    ///
    /// \param key Key to change state from.
    /// \param state State to assign to key.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setButtonState(GamepadButton button, bool state);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the modifiers for this state.
    ///
    /// \param mods Currently pressed modifiers.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setAxisValue(GamepadAxis axis, double value);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the associated gamepad.
    ///
    /// \param pad Gamepad to set.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setGamepad(QGamepad* pad);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QGamepad* m_pad;                            ///< Associated game pad
    bool      m_buttonStates[GamepadButtonMax]; ///< Holds all button states
    double    m_axisValues[GamepadAxisMax];     ///< Holds all axis values
};


////////////////////////////////////////////////////////////////////////////////
/// \class GamepadState
/// \ingroup Input
///
/// Use the convenient functions to determine whether a button is pressed or
/// to retrieve the values of the analogue sticks.
///
/// \code
/// void onGamepadChanged(const GamepadState& gamepad)
/// {
///     if (gamepad.isButtonDown(cran::GamepadButtonA))
///     {
///         // some shooting logic, perhaps
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

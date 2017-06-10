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
#ifndef CRANBERRY_GAMEPADRELEASEDEVENT_HPP
#define CRANBERRY_GAMEPADRELEASEDEVENT_HPP


// Cranberry headers
#include <Cranberry/Input/Enumerations.hpp>

// Forward declarations
CRANBERRY_FORWARD_Q(QGamepad)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the Gamepad plus the Gamepad-Button recently released.
///
/// \class GamepadReleasedEvent
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT GamepadReleasedEvent
{
public:

    CRANBERRY_DEFAULT_DTOR(GamepadReleasedEvent)
    CRANBERRY_DEFAULT_COPY(GamepadReleasedEvent)
    CRANBERRY_DEFAULT_MOVE(GamepadReleasedEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a GamepadReleasedEvent with the gamepad that sent it and
    /// the gamepad button that was recently released.
    ///
    /// \param pad Associated gamepad.
    /// \param button Released button.
    ///
    ////////////////////////////////////////////////////////////////////////////
    GamepadReleasedEvent(QGamepad* pad, GamepadButton button);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the associated gamepad.
    ///
    /// \returns the gamepad.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QGamepad* gamepad() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given gamepad button was released recently.
    ///
    /// \param cran::GamepadButton to check for.
    /// \returns true if it was released.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool wasReleased(GamepadButton button) const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QGamepad*     m_pad;    ///< Associated gamepad
    GamepadButton m_button; ///< Released button
};


////////////////////////////////////////////////////////////////////////////////
/// \class GamepadReleasedEvent
/// \ingroup Input
///
/// Use the convenient GamepadReleasedEvent::wasReleased() event to determine
/// whether a gamepad button was released.
///
/// \code
/// void onGamepadButtonReleased(const GamepadReleasedEvent& event)
/// {
///     if (event.wasReleased(cran::GamepadButtonA)
///     {
///         // do stuff
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

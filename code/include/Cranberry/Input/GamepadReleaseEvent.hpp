////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#ifndef CRANBERRY_INPUT_GAMEPADRELEASEDEVENT_HPP
#define CRANBERRY_INPUT_GAMEPADRELEASEDEVENT_HPP


// Cranberry headers
#include <Cranberry/Input/Enumerations.hpp>

// Forward declarations
CRANBERRY_FORWARD_Q(QGamepad)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the Gamepad plus the Gamepad-Button recently released.
///
/// \class GamepadReleaseEvent
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT GamepadReleaseEvent
{
public:

    CRANBERRY_DEFAULT_DTOR(GamepadReleaseEvent)
    CRANBERRY_DEFAULT_COPY(GamepadReleaseEvent)
    CRANBERRY_DEFAULT_MOVE(GamepadReleaseEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a GamepadReleaseEvent with the gamepad that sent it and
    /// the gamepad button that was recently released.
    ///
    /// \param pad Associated gamepad.
    /// \param button Released button.
    ///
    ////////////////////////////////////////////////////////////////////////////
    GamepadReleaseEvent(QGamepad* pad, GamepadButton button);


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
/// \class GamepadReleaseEvent
/// \ingroup Input
///
/// Use the convenient GamepadReleaseEvent::wasReleased() event to determine
/// whether a gamepad button was released.
///
/// \code
/// void onGamepadButtonRelease(const GamepadReleaseEvent& event)
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

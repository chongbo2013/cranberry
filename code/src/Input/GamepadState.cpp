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


// Cranberry headers
#include <Cranberry/Input/GamepadState.hpp>

// Qt headers
#include <QGamepad>


CRANBERRY_USING_NAMESPACE


QGamepad* GamepadState::gamepad() const
{
    return m_pad;
}


bool GamepadState::isButtonDown(GamepadButton button) const
{
    return m_buttonStates[(int) button];
}


bool GamepadState::isButtonUp(GamepadButton button) const
{
    return !m_buttonStates[(int) button];
}


double GamepadState::axisValue(GamepadAxis axis) const
{
    return m_axisValues[(int) axis];
}


void GamepadState::setButtonState(GamepadButton button, bool state)
{
    m_buttonStates[(int) button] = state;
}


void GamepadState::setAxisValue(GamepadAxis axis, double value)
{
    m_axisValues[(int) axis] = value;
}


void GamepadState::setGamepad(QGamepad* pad)
{
    m_pad = pad;
}

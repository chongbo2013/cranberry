////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#include <Cranberry/Input/GamepadReleaseEvent.hpp>

// Qt headers
#include <QGamepad>


CRANBERRY_USING_NAMESPACE


GamepadReleaseEvent::GamepadReleaseEvent(QGamepad* pad, GamepadButton button)
    : m_pad(pad)
    , m_button(button)
{
}


QGamepad* GamepadReleaseEvent::gamepad() const
{
    return m_pad;
}


bool GamepadReleaseEvent::wasReleased(GamepadButton button) const
{
    return m_button == button;
}

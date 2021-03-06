﻿////////////////////////////////////////////////////////////////////////////////
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
#include <Cranberry/Input/KeyReleaseEvent.hpp>


CRANBERRY_USING_NAMESPACE


KeyReleaseEvent::KeyReleaseEvent(int key, int mods)
    : m_key(key)
    , m_mods(mods)
{
}


bool KeyReleaseEvent::wasReleased(int key) const
{
    return m_key == key;
}


bool KeyReleaseEvent::hasModifier(int mod) const
{
    return (m_mods & mod) != 0;
}

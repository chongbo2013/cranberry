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


// Cranberry headers
#include <Cranberry/Window/Events/cbKeyUpEvent.hpp>

// Qt headers
#include <QApplication>


CRANBERRY_BEGIN_NAMESPACE


cbKeyUpEvent::
cbKeyUpEvent(int key, Qt::KeyboardModifiers mods)
    : m_key((Qt::Key) key)
    , m_mods(mods)
{
}


Qt::Key
cbKeyUpEvent::key() const
{
    return m_key;
}


Qt::KeyboardModifiers
cbKeyUpEvent::modifiers() const
{
    return m_mods;
}


bool
cbKeyUpEvent::hasModifier(Qt::KeyboardModifier mod) const
{
    return (m_mods & mod) != 0;
}


CRANBERRY_END_NAMESPACE

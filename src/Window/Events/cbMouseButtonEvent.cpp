﻿//
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
#include <Cranberry/Window/Events/cbMouseButtonEvent.hpp>


CRANBERRY_BEGIN_NAMESPACE


cbMouseButtonEvent::
cbMouseButtonEvent(const QPoint& p, Qt::MouseButtons buttons)
    : m_pos(p)
    , m_flags(buttons)
{
}


const QPoint&
cbMouseButtonEvent::pos() const
{
    return m_pos;
}


Qt::MouseButtons
cbMouseButtonEvent::buttons() const
{
    return m_flags;
}


bool
cbMouseButtonEvent::hasButton(Qt::MouseButton button) const
{
    return (m_flags & button) != 0;
}


CRANBERRY_END_NAMESPACE

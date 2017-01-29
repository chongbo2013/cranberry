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
#include <Cranberry/Window/Events/cbMouseMoveEvent.hpp>


CRANBERRY_BEGIN_NAMESPACE


cbMouseMoveEvent::
cbMouseMoveEvent(const QPoint& pOld, const QPoint& pNew)
    : m_pos(pNew)
    , m_delta(pNew - pOld)
{
}


const QPoint&
cbMouseMoveEvent::pos() const
{
    return m_pos;
}


const QPoint&
cbMouseMoveEvent::delta() const
{
    return m_delta;
}


CRANBERRY_END_NAMESPACE

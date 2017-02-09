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
#include <Cranberry/Window/Input/MouseReleaseEvent.hpp>


CRANBERRY_BEGIN_NAMESPACE


MouseReleaseEvent::MouseReleaseEvent(const QPoint& pos, Qt::MouseButtons btns)
    : m_pos(pos)
    , m_btns(btns)
{
}


const QPoint& MouseReleaseEvent::pos() const
{
    return m_pos;
}


Qt::MouseButtons MouseReleaseEvent::buttons() const
{
    return m_btns;
}


bool MouseReleaseEvent::hasButton(Qt::MouseButton button) const
{
    return (m_btns & button) != 0;
}


CRANBERRY_END_NAMESPACE

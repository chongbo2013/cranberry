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
#include <Cranberry/Graphics/Base/Hitbox.hpp>

// Qt headers
#include <QPainterPath>


CRANBERRY_USING_NAMESPACE


Hitbox::Hitbox()
    : m_pp(nullptr)
{
}


Hitbox::~Hitbox()
{
    delete m_pp;
}


void Hitbox::setHitbox(QPainterPath* pp)
{
    delete m_pp;
    m_pp = pp;
}


bool Hitbox::intersectsWith(const Hitbox& other) const
{
    return m_pp->intersects(*other.m_pp);
}

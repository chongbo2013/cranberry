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
#include <Cranberry/Game/Mapping/MapObject.hpp>

// Qt headers

// Standard headers


CRANBERRY_USING_NAMESPACE


MapObject::MapObject()
    : m_id(-1)
    , m_x(-1)
    , m_y(-1)
    , m_w(-1)
    , m_h(-1)
{
}


bool MapObject::isNull() const
{
    return m_id == -1;
}


int MapObject::id() const
{
    return m_id;
}


int MapObject::x() const
{
    return m_x;
}


int MapObject::y() const
{
    return m_y;
}


int MapObject::width() const
{
    return m_w;
}


int MapObject::height() const
{
    return m_h;
}


const QVariant& MapObject::propertyValue(const QString& property) const
{
    auto it = m_properties.find(property);
    return it.value();
}


QMap<QString, QVariant>& MapObject::properties()
{
    return m_properties;
}


void MapObject::setId(int id)
{
    m_id = id;
}


void MapObject::setX(int x)
{
    m_x = x;
}


void MapObject::setY(int y)
{
    m_y = y;
}


void MapObject::setWidth(int w)
{
    m_w = w;
}


void MapObject::setHeight(int h)
{
    m_h = h;
}

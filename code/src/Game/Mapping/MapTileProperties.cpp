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
#include <Cranberry/Game/Mapping/MapTileProperties.hpp>

// Global variables
CRANBERRY_GLOBAL_VAR(QVariant, g_default)


CRANBERRY_USING_NAMESPACE


bool MapTileProperties::isValid() const
{
    return m_properties.isEmpty() == false;
}


const QVariant& MapTileProperties::value(const QString& name) const
{
    auto it = m_properties.find(name);
    if (it == m_properties.end())
    {
        return g_default;
    }
    else
    {
        return it.value();
    }
}


QMap<QString, QVariant>& MapTileProperties::properties()
{
    return m_properties;
}


void MapTileProperties::setValue(const QString& name, const QVariant& value)
{
    m_properties[name] = value;
}


void MapTileProperties::addProperty(const QString& name, const QVariant& value)
{
    m_properties.insert(name, value);
}


void MapTileProperties::removeProperty(const QString& name)
{
    m_properties.remove(name);
}

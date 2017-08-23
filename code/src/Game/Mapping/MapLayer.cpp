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
#include <Cranberry/Game/Mapping/Map.hpp>
#include <Cranberry/Game/Mapping/MapLayer.hpp>


CRANBERRY_USING_NAMESPACE


MapLayer::MapLayer(Map* parent)
    : m_parent(parent)
    , m_opacity(1.0f)
    , m_isVisible(true)
    , m_offsetX(0)
    , m_offsetY(0)
{
}


Map* MapLayer::map() const
{
    return m_parent;
}


const QString& MapLayer::name() const
{
    return m_name;
}


float MapLayer::opacity() const
{
    return m_opacity;
}


bool MapLayer::isVisible() const
{
    return m_isVisible;
}


int MapLayer::layerId() const
{
    return m_layerId;
}


int MapLayer::offsetX() const
{
    return m_offsetX;
}


int MapLayer::offsetY() const
{
    return m_offsetY;
}


void MapLayer::setName(const QString &name)
{
    m_name = name;
}


void MapLayer::setOpacity(float opac)
{
    m_opacity = opac;
}


void MapLayer::setVisibility(bool visible)
{
    m_isVisible = visible;
}


void MapLayer::setLayerId(int id)
{
    m_layerId = id;
}


void MapLayer::setOffsetX(int x)
{
    m_offsetX = x;
}


void MapLayer::setOffsetY(int y)
{
    m_offsetY = y;
}

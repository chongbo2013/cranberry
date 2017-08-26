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
#include <Cranberry/Game/Mapping/Events/TileEvent.hpp>


CRANBERRY_USING_NAMESPACE


TileEvent::TileEvent(const MapTile& tile, const MapTileLayer* layer, const MapTileset* tileset)
    : m_tile(&tile)
    , m_layer(layer)
    , m_properties(&tileset->tileProperties(tile.tileId()))
    , m_isAccepted(true)
{
}


const MapTile& TileEvent::tile() const
{
    return *m_tile;
}


const MapTileLayer& TileEvent::layer() const
{
    return *m_layer;
}


const MapTileProperties& TileEvent::properties() const
{
    return *m_properties;
}


bool TileEvent::isAccepted() const
{
    return m_isAccepted;
}


void TileEvent::accept() const
{
    m_isAccepted = true;
}


void TileEvent::reject() const
{
    m_isAccepted = false;
}

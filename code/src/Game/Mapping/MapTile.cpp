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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Cranberry. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////


// Cranberry headers
#include <Cranberry/Game/Mapping/MapTile.hpp>


CRANBERRY_USING_NAMESPACE


MapTile::MapTile()
    : m_tileId(-1)
    , m_tilesetId(-1)
{
}


bool MapTile::isNull() const
{
    return m_tileId == -1 && m_tilesetId == -1;
}


int MapTile::tileId() const
{
    return m_tileId;
}


int MapTile::tilesetId() const
{
    return m_tilesetId;
}


void MapTile::setTileId(int id)
{
    m_tileId = id;
}


void MapTile::setTilesetId(int id)
{
    m_tilesetId = id;
}

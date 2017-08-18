﻿////////////////////////////////////////////////////////////////////////////////
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


#pragma once
#ifndef CRANBERRY_GAME_MAPPING_MAPTILE_HPP
#define CRANBERRY_GAME_MAPPING_MAPTILE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines one tile within a tile layer.
///
/// \class MapTile
/// \author Nicolas Kogler
/// \date August 15, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapTile
{
public:

    CRANBERRY_DEFAULT_DTOR(MapTile)
    CRANBERRY_DEFAULT_COPY(MapTile)
    CRANBERRY_DEFAULT_MOVE(MapTile)

    MapTile();


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the local tile ID within the current tileset.
    ///
    /// \returns the local tile ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the ID of the tileset containing this tile.
    ///
    /// \returns the tileset ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tilesetId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the local tile ID. In debug mode, checks whether the ID is
    /// actually inside the tileset.
    ///
    /// \param id Local tile ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTileId(int id);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the ID of the tileset containing this tile.
    ///
    /// \param id Tileset ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTilesetId(int id);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    int m_tileId;    ///< Local tile identifier
    int m_tilesetId; ///< Tileset identifier
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapTile
/// \ingroup Game
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

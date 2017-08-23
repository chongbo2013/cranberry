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


#pragma once
#ifndef CRANBERRY_TILEEVENT_HPP
#define CRANBERRY_TILEEVENT_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/MapLayer.hpp>
#include <Cranberry/Game/Mapping/MapTile.hpp>
#include <Cranberry/Game/Mapping/MapTileset.hpp>
#include <Cranberry/Game/Mapping/MapTileProperties.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This event is emitted when a tile is about to be stepped on, being stepped
/// on or being left.
///
/// \class TileEvent
/// \author Nicolas Kogler
/// \date August 19, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT TileEvent
{
public:

    CRANBERRY_DEFAULT_DTOR(TileEvent)
    CRANBERRY_DEFAULT_COPY(TileEvent)
    CRANBERRY_DEFAULT_MOVE(TileEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new tile event.
    ///
    /// \param tile Tile that caused the event.
    /// \param layer Layer consisting of the tile.
    /// \param tileset Tileset consisting of the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TileEvent(const MapTile& tile, MapLayer* layer, MapTileset* tileset);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the tile causing the event.
    ///
    /// \returns the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapTile& tile() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the layer of the causing tile.
    ///
    /// \returns the layer of the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapLayer& layer() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the properties of the causing tile.
    ///
    /// \returns the properties of the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapTileProperties& properties() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this event was accepted.
    ///
    /// \default By default, this value is true.
    /// \returns true if this event was accepted.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isAccepted() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Accepts the event.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void accept() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Rejects the event.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void reject() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    MapTile*           m_tile;
    MapLayer*          m_layer;
    MapTileProperties* m_properties;
    mutable bool       m_isAccepted;
};


////////////////////////////////////////////////////////////////////////////////
/// \class TileEvent
/// \ingroup Game
///
/// This class can be used to retrieve the tile and its general properties in
/// order to achieve some nice functionality on your map, e.g.:
///
/// \code
/// void onAboutStepTile(const TileEvent& e)
/// {
///     if (e.properties().value("solid").toBool())
///     {
///         e.reject(); // do not step on tile
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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
#ifndef CRANBERRY_GAME_MAPPING_MAPTILELAYER_HPP
#define CRANBERRY_GAME_MAPPING_MAPTILELAYER_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/MapLayer.hpp>
#include <Cranberry/Game/Mapping/MapTile.hpp>
#include <Cranberry/Graphics/Tilemap.hpp>

// Forward declarations
CRANBERRY_FORWARD_Q(QDomElement)
CRANBERRY_FORWARD_C(MapTileset)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines one map layer with multiple tiles.
///
/// \class MapLayer
/// \author Nicolas Kogler
/// \date August 16, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapTileLayer final : public MapLayer
{
public:

    CRANBERRY_DECLARE_DTOR(MapTileLayer)
    CRANBERRY_DISABLE_COPY(MapTileLayer)
    CRANBERRY_DISABLE_MOVE(MapTileLayer)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new MapLTileayer with the given Map as parent.
    ///
    /// \param parent The parent of this layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapTileLayer(Map* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the data encoding type for this layer.
    ///
    /// \returns the encoding type.
    ///
    ////////////////////////////////////////////////////////////////////////////
    EncodingType dataEncoding() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the data compression mode for this layer.
    ///
    /// \returns the compression mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    CompressionMode dataCompressionMode() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all tiles, starting from top-left to bottom-right.
    ///
    /// \returns the tiles.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<MapTile>& tiles() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the object to render.
    ///
    /// \returns the render object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Tilemap* renderObject() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Parses the given layer XML element.
    ///
    /// \param xmlElement Layer element to parse.
    /// \param tilesets Tilesets to use.
    /// \param layerId Index of this layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool parse(
        QDomElement* xmlElement,
        const QVector<MapTileset*>& tilesets,
        int layerId
        );


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    LayerType layerType() const override;
    void update(const GameTime& time) override;
    void render() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Tilemap*         m_tileMap;
    EncodingType     m_encoding;
    CompressionMode  m_compression;
    QVector<MapTile> m_tiles;
};


CRANBERRY_END_NAMESPACE


#endif

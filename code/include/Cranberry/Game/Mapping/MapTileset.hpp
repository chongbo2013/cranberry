﻿////////////////////////////////////////////////////////////////////////////////
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


#pragma once
#ifndef CRANBERRY_GAME_MAPPING_MAPTILESET_HPP
#define CRANBERRY_GAME_MAPPING_MAPTILESET_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/MapTileProperties.hpp>

// Qt headers
#include <QMap>
#include <QString>
#include <QVariant>

// Forward declarations
CRANBERRY_FORWARD_Q(QDomElement)
CRANBERRY_FORWARD_Q(QOpenGLTexture)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds data of one TMX tileset.
///
/// \class MapTileset
/// \author Nicolas Kogler
/// \date August 16, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapTileset final
{
public:

    CRANBERRY_DECLARE_CTOR(MapTileset)
    CRANBERRY_DECLARE_DTOR(MapTileset)
    CRANBERRY_DEFAULT_COPY(MapTileset)
    CRANBERRY_DEFAULT_MOVE(MapTileset)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the first global ID of this tileset.
    ///
    /// \returns the first global ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int globalId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the path to the tileset image.
    ///
    /// \returns the tileset image path.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& imagePath() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this tileset.
    ///
    /// \returns the tileset name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the actual texture.
    ///
    /// \returns the tileset texture.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLTexture* texture() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the width of one single tile.
    ///
    /// \returns the tile width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileWidth() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the height of one single tile.
    ///
    /// \returns the tile height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileHeight() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the spacing between each tile.
    ///
    /// \returns the tile spacing.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileSpacing() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the margin around each tile.
    ///
    /// \returns the tile margin.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileMargin() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of tiles in this tilset.
    ///
    /// \returns the tile amount.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileCount() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all tileset properties.
    ///
    /// \returns all properties.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QMap<QString, QVariant>& properties() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all tile properties of the given tile.
    ///
    /// \param tileId Id of the tile which's properties to receive.
    /// \returns all tile properties.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapTileProperties& tileProperties(int tileId) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Parses the given tileset XML element.
    ///
    /// \param xmlElement Tileset element to parse.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool parse(QDomElement* xmlElement);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    int                          m_globalId;
    QString                      m_imagePath;
    QString                      m_name;
    QOpenGLTexture*              m_texture;
    int                          m_tileWidth;
    int                          m_tileHeight;
    int                          m_tileSpacing;
    int                          m_tileMargin;
    int                          m_tileCount;
    QMap<QString, QVariant>      m_properties;
    QMap<int, MapTileProperties> m_tileProps;
};


CRANBERRY_END_NAMESPACE


#endif

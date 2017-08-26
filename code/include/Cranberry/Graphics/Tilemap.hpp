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
#ifndef CRANBERRY_TILEMAP_HPP
#define CRANBERRY_TILEMAP_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>

// Qt headers
#include <QVector>

// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLTexture)
CRANBERRY_FORWARD_Q(QOpenGLBuffer)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders multiple tiles from a tilemap with a great performance. Also
/// supports multiple tilesets.
///
/// \class Tilemap
/// \author Nicolas Kogler
/// \date August 16, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Tilemap final : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(Tilemap)
    CRANBERRY_DECLARE_DTOR(Tilemap)
    CRANBERRY_DEFAULT_COPY(Tilemap)
    CRANBERRY_DEFAULT_MOVE(Tilemap)

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the tiles in the map.
    ///
    /// \param tiles List of tile indices, paired with tileset indices.
    /// \returns false if there are more tiles than the map can actually hold.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setTiles(const QVector<QPair<int, int>>& tiles);

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the tilemap object.
    ///
    /// \param tilesets Paths to tileset images to use.
    /// \param tileSize Size of each tile, in pixels.
    /// \param mapSize Size of the entire map, in tiles.
    /// \param view The map view inside the window. A null rectangle leads to
    ///        having the entire window as view.
    /// \param renderTarget Target to render batch on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(
            const QVector<QString>& tilesets,
            const QSize& tileSize,
            const QSize& mapSize,
            const QRect& view,
            Window* renderTarget = nullptr
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a tilemap object, but with several textures to use as tilesets.
    /// This class will _not_ take ownership of the tilesets provided!
    ///
    /// \param textures Tilesets to use for this tilemap.
    /// \param tileSize Size of each tile, in pixels.
    /// \param mapSize Size of the entire map, in tiles.
    /// \param view The map view inside the window. A null rectangle leads to
    ///        having the entire window as view.
    /// \param renderTarget Target to render batch on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(
            const QVector<QOpenGLTexture*>& textures,
            const QSize& tileSize,
            const QSize& mapSize,
            const QRect& view,
            Window* renderTarget = nullptr
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Appends one single tile.
    ///
    /// \param tileIndex The index of the tile within the tileset.
    /// \param tileset The id of the tileset to pick tile from (defaults to 0).
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool appendTile(int tileIndex, int tileset = 0);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts one tile at \p index.
    ///
    /// \param index Map index to insert to. Will be mapped to X/Y.
    /// \param tileIndex The index of the tile within the tileset.
    /// \param tileset The id of the tileset to pick tile from (defaults to 0).
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool insertTile(int index, int tileIndex, int tileset = 0);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts one tile at \p x, \p y.
    ///
    /// \param x X-location to insert tile to, in tile units.
    /// \param y Y-location to insert tile to, in tile units.
    /// \param tileIndex The index of the tile within the tileset.
    /// \param tileset The id of the tileset to pick tile from (defaults to 0).
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool insertTile(int x, int y, int tileIndex, int tileset = 0);

    ////////////////////////////////////////////////////////////////////////////
    /// Replaces one tile at \p index.
    ///
    /// \param index Map index to replace. Will be mapped to X/Y.
    /// \param tileIndex The index of the new tile within the tileset.
    /// \param tileset The id of the tileset to pick tile from (defaults to 0).
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool replaceTile(int index, int tileIndex, int tileset = 0);

    ////////////////////////////////////////////////////////////////////////////
    /// Replaces one tile at \p x, \p y.
    ///
    /// \param x X-location to replace tile, in tile units.
    /// \param y Y-location to replace tile, in tile units.
    /// \param tileIndex The index of the new tile within the tileset.
    /// \param tileset The id of the tileset to pick tile from (defaults to 0).
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool replaceTile(int x, int y, int tileIndex, int tileset = 0);

    ////////////////////////////////////////////////////////////////////////////
    /// Appends a transparent tile of size (tilewidth, tileheight).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void appendNullTile();

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all tiles from the map.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllTiles();


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is null.
    ///
    /// \returns true if this batch is null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all the OpenGL objects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the tilemap.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders all tiles of the tilemap.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    bool createInternal(Window* rt);
    bool getUniformLocations();
    void bindObjects();
    void releaseObjects();
    void writeVertices();
    void modifyProgram();
    void modifyAttribs();
    void drawElements();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QVector<QOpenGLTexture*> m_textures;
    QVector<int>             m_uniformLocs;
    QOpenGLBuffer*           m_vertexBuffer;
    QOpenGLBuffer*           m_textureBuffer;
    priv::MapVertices        m_vertices;
    priv::IdVertices         m_ids;
    QRect                    m_view;
    int                      m_tileWidth;
    int                      m_tileHeight;
    int                      m_mapWidth;
    int                      m_mapHeight;
    int                      m_currentX;
    int                      m_currentY;
    bool                     m_update;
    bool                     m_ownTextures;

};


#define TILEMAP_MAX_SETS 10


////////////////////////////////////////////////////////////////////////////////
/// \class Tilemap
/// \ingroup Graphics
///
/// A tilemap can be used to draw multiple tiles from a tileset in a performant
/// way.
///
/// \code
/// m_tilemap = new Tilemap;
/// m_tilemap->create(":/tilesets/set.png", { 32, 32 }, { 40, 40, }, { });
/// m_tilemap->appendTile(0);
/// m_tilemap->appendTile(0);
/// m_tilemap->appendTile(1);
///
/// ...
///
/// m_tilemap->render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

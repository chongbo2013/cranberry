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

// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLTexture)
CRANBERRY_FORWARD_Q(QOpenGLBuffer)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders multiple tiles from a tilemap with a great performance.
///
/// \class Tilemap
/// \author Nicolas Kogler
/// \date August 16, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Tilemap final : public RenderBase
{
public:

    CRANBERRY_DEFAULT_COPY(Tilemap)
    CRANBERRY_DEFAULT_MOVE(Tilemap)

    Tilemap();
   ~Tilemap();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is null.
    ///
    /// \returns true if this batch is null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the tilemap object.
    ///
    /// \param tileset Tileset image to use.
    /// \param tileSize Size of each tile, in pixels.
    /// \param mapSize Size of the entire map, in tiles.
    /// \param view The map view inside the window. A null rectangle leads to
    ///        having the entire window as view.
    /// \param renderTarget Target to render batch on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(
            const QString& tileset,
            const QSize& tileSize,
            const QSize& mapSize,
            const QRect& view,
            Window* renderTarget = nullptr
            );

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

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the tiles in the map.
    ///
    /// \param tiles List of tile indices.
    /// \returns false if there are more tiles than the map can actually hold.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setTiles(const QVector<uint>& tiles);

    ////////////////////////////////////////////////////////////////////////////
    /// Appends one single tile.
    ///
    /// \param tileIndex The index of the tile within the tileset.
    /// \returns false if out of bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool appendTile(uint tileIndex);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all tiles from the map.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllTiles();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    void bindObjects();
    void releaseObjects();
    void writeVertices();
    void modifyProgram();
    void modifyAttribs();
    void drawElements();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLTexture*   m_texture;
    QOpenGLBuffer*    m_vertexBuffer;
    priv::MapVertices m_vertices;
    QRect             m_view;
    uint              m_tileWidth;
    uint              m_tileHeight;
    uint              m_setWidth;
    uint              m_setHeight;
    uint              m_mapWidth;
    uint              m_mapHeight;
    uint              m_currentX;
    uint              m_currentY;
    bool              m_update;

};


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

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
#include <Cranberry/Game/Mapping/MapTileset.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QDomElement>
#include <QOpenGLTexture>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "TMX (layer): Name attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_02, "TMX (tile): Gid attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_03, "Tilemap could not be created.")
CRANBERRY_CONST_VAR(QString, e_04, "Tile could not be added.")


CRANBERRY_USING_NAMESPACE


MapLayer::MapLayer(Map* parent)
    : m_tileMap(new Tilemap)
    , m_parent(parent)
    , m_opacity(1.0f)
    , m_isVisible(true)
    , m_offsetX(0)
    , m_offsetY(0)
    , m_encoding(LayerEncodingNone)
    , m_compression(CompressionModeNone)
{
}


MapLayer::~MapLayer()
{
    delete m_tileMap;
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


LayerEncoding MapLayer::dataEncoding() const
{
    return m_encoding;
}


CompressionMode MapLayer::dataCompressionMode() const
{
    return m_compression;
}


const QVector<MapTile>& MapLayer::tiles() const
{
    return m_tiles;
}


Tilemap* MapLayer::renderObject() const
{
    return m_tileMap;
}


bool MapLayer::parse(
    QDomElement* xmlElement,
    const QVector<MapTileset*>& tilesets,
    int layerId
    )
{
    m_layerId = layerId;

    // Parses all the attributes.
    if (!xmlElement->hasAttribute("name"))
    {
        return cranError(e_01);
    }
    else
    {
        m_name = xmlElement->attribute("name");
    }

    if (xmlElement->hasAttribute("opacity"))
    {
        m_opacity = xmlElement->attribute("opacity").toFloat();
    }

    if (xmlElement->hasAttribute("visible"))
    {
        m_isVisible = xmlElement->attribute("visible").toInt() == 1;
    }

    if (xmlElement->hasAttribute("offsetx"))
    {
        m_offsetX = xmlElement->attribute("offsetx").toInt();
    }

    if (xmlElement->hasAttribute("offsety"))
    {
        m_offsetY = xmlElement->attribute("offsety").toInt();
    }

    // Parses all the tile data.
    QDomElement dataElement = xmlElement
           ->elementsByTagName("data")
            .at(0)
            .toElement();

    if (dataElement.hasAttribute("encoding"))
    {
        m_encoding = getEncodingFromString(dataElement.attribute("encoding"));
    }

    if (m_encoding == LayerEncodingBase64 && dataElement.hasAttribute("compression"))
    {
        m_compression = getCompressionFromString(dataElement.attribute("compression"));
    }

    QList<int> tileIdentifiers;

    if (m_encoding == LayerEncodingBase64)
    {
        QString inner = dataElement.nodeValue();
        QByteArray decoded = QByteArray::fromBase64(inner.toUtf8());

        if (m_compression == CompressionModeZlib)
        {
            int expectedSize = m_parent->width() * m_parent->height() * sizeof(int);

            // TODO: Do we really need this? TMX used Qt for compression. Qt docs:
            // "This function will uncompress data compressed with qCompress()
            //  from this and any earlier Qt version"
            decoded.prepend((char)(expectedSize >> 24) & 0xFF);
            decoded.prepend((char)(expectedSize >> 16) & 0xFF);
            decoded.prepend((char)(expectedSize >> 8) & 0xFF);
            decoded.prepend((char)(expectedSize >> 0) & 0xFF);
            decoded = qUncompress(decoded);
        }

        for (int i = 0; i < decoded.size(); i += 4)
        {
            tileIdentifiers.push_back(
                    (decoded.at(i + 3) << 24) |
                    (decoded.at(i + 2) << 16) |
                    (decoded.at(i + 1) <<  8) |
                    (decoded.at(i + 0) <<  0));
        }
    }
    else if (m_encoding == LayerEncodingCsv)
    {
        QString inner = dataElement.firstChild().nodeValue().trimmed();
        QStringList indices = inner.split(',');

        for (const QString& s : indices)
        {
            tileIdentifiers.push_back(s.toInt());
        }
    }
    else
    {
        QDomNodeList tileList = dataElement.elementsByTagName("tile");
        for (int i = 0; i < tileList.size(); i++)
        {
            QDomElement tileElem = tileList.at(i).toElement();
            if (!tileElem.hasAttribute("gid"))
            {
                return cranError(e_02);
            }

            tileIdentifiers.push_back(tileElem.attribute("gid").toInt());
        }
    }

    // Get all the QOpenGLTexture instances from the given tilesets.
    QVector<QOpenGLTexture*> glTextures;
    for (MapTileset* set : tilesets)
    {
        glTextures.push_back(set->texture());
    }

    // Creates the tilemap for rendering.
    if (!m_tileMap->create(
            glTextures,
            QSize(m_parent->tileWidth(), m_parent->tileHeight()),
            QSize(m_parent->width(), m_parent->height()),
            QRect(),
            m_parent->renderTarget()
            ))
    {
        return cranError(e_03);
    }

    // Parses the tiles.
    for (int tile : tileIdentifiers)
    {
        // Reads the flags.
        // TODO: Actually use these flags to invert UV coordinates.
        bool flip_hor = tile & FlipFlagHorizontal; Q_UNUSED(flip_hor)
        bool flip_ver = tile & FlipFlagVertical;   Q_UNUSED(flip_ver)
        bool flip_dia = tile & FlipFlagDiagonal;   Q_UNUSED(flip_dia)

        // Clears the flags from the tile ID.
        tile &= ~FlipFlagAll;

        if (tile == 0)
        {
            m_tileMap->appendEmptyTile();
            m_tiles.append(MapTile());
            continue;
        }

        // Resolves the tile and finds the tileset.
        int setCount = tilesets.size();
        for (int i = setCount - 1; i >= 0; i--)
        {
            MapTileset* const tileset = tilesets.at(i);
            MapTile mTile;

            if (tileset->globalId() <= tile)
            {
                int realId = tile - tileset->globalId();
                if (!m_tileMap->appendTile(realId, i))
                {
                    return cranError(e_04);
                }

                mTile.setTileId(realId);
                mTile.setTilesetId(i);
                m_tiles.append(mTile);
            }
        }
    }

    return true;
}

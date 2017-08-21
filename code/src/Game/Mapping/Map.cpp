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


// Cranberry headers
#include <Cranberry/Game/Mapping/Map.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Could not open map file \"%2\".")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Could not parse tileset.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Could not parse layer.")


CRANBERRY_USING_NAMESPACE


Map::Map()
    : m_orientation(MapOrientationOrthogonal)
    , m_moveMode(PlayerMoveTiles)
    , m_playerMoveDir(MoveNone)
    , m_width(0)
    , m_height(0)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_playerX(0)
    , m_playerY(0)
    , m_targetX(0)
    , m_targetY(0)
    , m_playerMovingX(false)
    , m_playerMovingY(false)
{
}


Map::~Map()
{
    destroy();
}


bool Map::isNull() const
{
    return RenderBase::isNull() ||
           m_width == 0         ||
           m_height == 0        ||
           m_tileWidth == 0     ||
           m_tileHeight == 0    ||
           m_layers.isEmpty()   ||
           m_tilesets.isEmpty();
}


bool Map::isPlayerMoving() const
{
    return m_playerMovingX || m_playerMovingY;
}


MapOrientation Map::orientation() const
{
    return m_orientation;
}


PlayerMoveMode Map::playerMoveMode() const
{
    return m_moveMode;
}


int Map::width() const
{
    return m_width;
}


int Map::height() const
{
    return m_height;
}


int Map::tileWidth() const
{
    return m_tileWidth;
}


int Map::tileHeight() const
{
    return m_tileHeight;
}


const QColor& Map::backgroundColor() const
{
    return m_bgColor;
}


const QMap<QString, QVariant>& Map::properties() const
{
    return m_properties;
}


void Map::setPlayerMoveMode(PlayerMoveMode mode)
{
    m_moveMode = mode;
}


bool Map::movePlayerBy(int x, int y)
{
    // Reject any movement while there is one running.
    if (isPlayerMoving())
    {
        return false;
    }

    if (m_moveMode == PlayerMoveTiles)
    {
        for (MapLayer* layer : m_layers)
        {
            const MapTile& oldT = layer->tiles().at(getTileIndex(m_playerX, m_playerY));
            const MapTile& newT = layer->tiles().at(getTileIndex(m_playerX + x, m_playerY + y));

            if (!newT.isTransparent())
            {
                TileEvent event(newT, layer, m_tilesets[newT.tilesetId()]);
                onAboutStepTile(event);

                if (!event.isAccepted())
                {
                    // We e.g. hit something solid, abort.
                    return false;
                }
            }

            if (!oldT.isTransparent())
            {
                onLeaveTile(TileEvent(oldT, layer, m_tilesets[oldT.tilesetId()]));
            }
        }

        m_playerMoveDir = MoveNone;

        // Triggers a movement.
        if (x != 0) m_playerMovingX = true;
        if (y != 0) m_playerMovingY = true;

        if (x <= 0) m_playerMoveDir |= MoveWest;
        if (x >= 0) m_playerMoveDir |= MoveEast;
        if (y <= 0) m_playerMoveDir |= MoveNorth;
        if (y >= 0) m_playerMoveDir |= MoveSouth;

        m_targetX = m_playerX + x;
        m_targetY = m_playerY + y;
        m_realposX = m_playerX * m_tileWidth;
        m_realposY = m_playerY * m_tileHeight;
        m_realtargetX = m_realposX + (x * m_tileWidth);
        m_realtargetY = m_realposY + (y * m_tileHeight);
    }
    else
    {
        int oldTileX = m_playerX / m_tileWidth;
        int oldTileY = m_playerY / m_tileHeight;
        int newTileX = (m_playerX + x) / m_tileWidth;
        int newTileY = (m_playerY + y) / m_tileHeight;

        if (oldTileX != newTileX || oldTileY != newTileY)
        {
            // Current tile changed, send event to all layers.
            for (MapLayer* layer : m_layers)
            {
                const MapTile& oldT = layer->tiles().at(getTileIndex(oldTileX, oldTileY));
                const MapTile& newT = layer->tiles().at(getTileIndex(newTileX, newTileY));

                if (!newT.isTransparent())
                {
                    TileEvent event(newT, layer, m_tilesets[newT.tilesetId()]);
                    onAboutStepTile(event);

                    if (!event.isAccepted())
                    {
                        // We e.g. hit something solid, abort.
                        return false;
                    }

                    onStepTile(TileEvent(newT, layer, m_tilesets[newT.tilesetId()]));
                }

                onLeaveTile(TileEvent(oldT, layer, m_tilesets[oldT.tilesetId()]));
            }
        }

        // Moves the player in an instant.
        m_playerX += x;
        m_playerY += y;
    }

    return true;
}


void Map::movePlayerTo(int x, int y)
{
    if (m_moveMode == PlayerMoveTiles)
    {
        m_playerX = x;
        m_playerY = y;
    }
    else
    {
        m_playerX = x * m_tileWidth;
        m_playerY = y * m_tileHeight;
    }
}


bool Map::create(const QString& mapPath, Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    QFile file(mapPath);
    QTextStream textStream(&file);
    QDomDocument xmlDocument;

    // Attempts to open the XML file.
    if (!file.open(QFile::ReadOnly))
    {
        return cranError(ERRARG_1(e_01, mapPath));
    }

    xmlDocument.setContent(textStream.readAll());
    file.close();

    // Parses the <map> node.
    QDomElement mapNode = xmlDocument
            .elementsByTagName("map")
            .at(0)
            .toElement();

    m_orientation = getOrientationFromString(mapNode.attribute("orientation"));
    m_width = mapNode.attribute("width").toInt();
    m_height = mapNode.attribute("height").toInt();
    m_tileWidth = mapNode.attribute("tilewidth").toInt();
    m_tileHeight = mapNode.attribute("tileheight").toInt();
    m_bgColor = getColorFromString(mapNode.attribute("backgroundcolor"));

    QDomNodeList listTileset = mapNode.elementsByTagName("tileset");
    QDomNodeList listLayer = mapNode.elementsByTagName("layer");
    QDomNodeList listObject = mapNode.elementsByTagName("objectgroup");

    // Parses the tilesets.
    for (int i = 0; i < listTileset.size(); i++)
    {
        QDomElement elem = listTileset.at(i).toElement();
        MapTileset* tileset = new MapTileset;
        if (!tileset->parse(&elem))
        {
            return cranError(ERRARG(e_02));
        }

        m_tilesets.append(tileset);
    }

    // Parses the layers.
    for (int i = 0; i < listLayer.size(); i++)
    {
        QDomElement elem = listLayer.at(i).toElement();
        MapLayer* layer = new MapLayer(this);
        if (!layer->parse(&elem, m_tilesets, i))
        {
            return cranError(ERRARG(e_03));
        }

        m_layers.append(layer);
    }

    // Parses the objects.
    for (int i = 0; i < listObject.size(); i++)
    {
        QDomNodeList objs = listObject.at(i).childNodes();
        for (int j = 0; j < objs.size(); j++)
        {
            MapObject obj;
            QDomElement elem = objs.at(j).toElement();
            QDomElement props = elem
                    .elementsByTagName("properties")
                    .at(0)
                    .toElement();

            QString strId = elem.attribute("id");
            QString strX = elem.attribute("x");
            QString strY = elem.attribute("y");
            QString strW = elem.attribute("width");
            QString strH = elem.attribute("height");

            obj.setId(strId.toInt());
            obj.setX(strX.toInt());
            obj.setY(strY.toInt());
            obj.setWidth(strW.toInt());
            obj.setHeight(strH.toInt());
            getTmxProperties(&props, obj.properties());

            m_objects.push_back(obj);
        }
    }

    // Parses the properties.
    QDomElement propsNode = mapNode
            .elementsByTagName("properties")
            .at(0)
            .toElement();

    getTmxProperties(&propsNode, m_properties);

    return true;
}


void Map::destroy()
{
    for (MapLayer* m : m_layers)
    {
        delete m;
    }

    for (MapTileset* m : m_tilesets)
    {
        delete m;
    }

    m_layers.clear();
    m_tilesets.clear();

    RenderBase::destroy();
}


void Map::update(const GameTime& time)
{
    updateTransform(time);

    // Avoid artifacts while rendering all the tiles.
    float dx = round(x());
    float dy = round(y());

    // Update the layer positions.
    for (MapLayer* layer : m_layers)
    {
        layer->renderObject()->setPosition(dx + layer->offsetX(), dy + layer->offsetY());
        layer->renderObject()->setOpacity(layer->opacity());
    }

    updateTileMovement(time.deltaTime());
}


void Map::render()
{
    for (MapLayer* layer : m_layers)
    {
        if (layer->isVisible())
        {
            layer->renderObject()->render();
        }
    }
}


void Map::onAboutStepTile(const TileEvent &event)
{
    if (event.layer().layerId() == 0)
        printf("On about to step on tile (layer 0): %d\n", event.tile().tileId());
}


void Map::onStepTile(const TileEvent &event)
{
    if (event.layer().layerId() == 0)
    {
        printf("On stepping tile (layer 0): %d\n", event.tile().tileId());
        printf("Property \"type\": \"%s\"\n", event.properties().value("prop").toString().toStdString().c_str());
    }
}


void Map::onLeaveTile(const TileEvent &event)
{
    if (event.layer().layerId() == 0)
        printf("On leaving tile (layer 0): %d\n", event.tile().tileId());
}


const QVector<MapTileset*>& Map::tilesets() const
{
    return m_tilesets;
}


const QVector<MapLayer*>& Map::layers() const
{
    return m_layers;
}


const QVector<MapObject>& Map::objects() const
{
    return m_objects;
}


int Map::getTileIndex(int x, int y)
{
    return y * m_tileWidth + x;
}


void Map::updateTileMovement(double deltaTime)
{
    // Update running movements, if any.
    if (isPlayerMoving())
    {
        if (m_playerMovingX)
        {
            if (m_playerMoveDir & MoveEast)
            {
                m_realposX += moveSpeedX() * deltaTime;
                if (m_realposX >= m_realtargetX)
                {
                    m_realposX = m_realtargetX;
                    m_playerX = m_targetX;
                    m_playerMovingX = false;
                }
            }

            if (m_playerMoveDir & MoveWest)
            {
                m_realposX -= moveSpeedX() * deltaTime;
                if (m_realposX <= m_realtargetX)
                {
                    m_realposX = m_realtargetX;
                    m_playerX = m_targetX;
                    m_playerMovingX = false;
                }
            }
        }

        if (m_playerMovingY)
        {
            if (m_playerMoveDir & MoveSouth)
            {
                m_realposY += moveSpeedY() * deltaTime;
                if (m_realposY >= m_realtargetY)
                {
                    m_realposY = m_realtargetY;
                    m_playerY = m_targetY;
                    m_playerMovingY = false;
                }
            }
            else
            {
                m_realposY -= moveSpeedY() * deltaTime;
                if (m_realposY <= m_realtargetY)
                {
                    m_realposY = m_realtargetY;
                    m_playerY = m_targetY;
                    m_playerMovingY = false;
                }
            }
        }

        if (!isPlayerMoving())
        {
            // Current tile changed, send event to all layers.
            for (MapLayer* layer : m_layers)
            {
                const MapTile& tile = layer->tiles().at(getTileIndex(m_playerX, m_playerY));
                if (!tile.isTransparent())
                {
                    onStepTile(TileEvent(tile, layer, m_tilesets[tile.tilesetId()]));
                }
            }
        }
    }
}

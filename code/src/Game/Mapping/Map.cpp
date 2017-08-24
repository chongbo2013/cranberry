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
#include <Cranberry/Game/Mapping/MapTileLayer.hpp>
#include <Cranberry/Game/Mapping/MapObjectLayer.hpp>
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


const QVector<MapTileset*>& Map::tilesets() const
{
    return m_tilesets;
}


const QVector<MapLayer*>& Map::layers() const
{
    return m_layers;
}


MapLayer* Map::layerByName(const QString& name) const
{
    for (MapLayer* layer : m_layers)
    {
        if (layer->name() == name)
        {
            return layer;
        }
    }

    return nullptr;
}


MapObject* Map::objectByName(const QString& name) const
{
    for (MapLayer* layer : m_layers)
    {
        if (layer->layerType() == LayerTypeObject)
        {
            const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
            for (MapObject* o : ol->objects())
            {
                if (o->name() == name)
                {
                    return o;
                }
            }
        }
    }

    return nullptr;
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
    if (isPlayerMoving() || (x == 0 && y == 0))
    {
        return false;
    }

    if (m_moveMode == PlayerMoveTiles)
    {
        return movePlayerByTiles(x, y);
    }
    else
    {
        return movePlayerByPixels(x, y);
    }
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

    return loadTilesets(&mapNode) && loadLayers(&mapNode) && loadProperties(&mapNode);
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
    updateTileMovement(time.deltaTime());
}


void Map::render()
{
    for (MapLayer* layer : m_layers)
    {
        if (layer->isVisible())
        {
            layer->render();
        }
    }
}


int Map::getTileIndex(int x, int y)
{
    return y * m_tileWidth + x;
}


bool Map::loadTilesets(QDomElement* elem)
{
    QDomNodeList listTileset = elem->elementsByTagName("tileset");
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

    return true;
}


bool Map::loadLayers(QDomElement* elem)
{
    QDomNodeList listLayer = elem->childNodes();
    for (int i = 0; i < listLayer.size(); i++)
    {
        QDomElement node = listLayer.at(i).toElement();
        if (node.nodeName() == "layer")
        {
            MapTileLayer* layer = new MapTileLayer(this);
            if (!layer->parse(&node, m_tilesets, m_layers.size()))
            {
                return cranError(ERRARG(e_03));
            }

            m_layers.append(layer);
        }
        else if (node.nodeName() == "objectgroup")
        {
            MapObjectLayer* layer = new MapObjectLayer(this);
            if (!layer->parse(&node, m_layers.size()))
            {
                return cranError(ERRARG(e_03));
            }

            m_layers.append(layer);
        }
    }

    return true;
}


bool Map::loadProperties(QDomElement* elem)
{
    QDomElement props = elem->elementsByTagName("properties").at(0).toElement();
    getTmxProperties(&props, m_properties);

    return true;
}


bool Map::movePlayerByTiles(int x, int y)
{
    for (MapLayer* layer : m_layers)
    {
        if (layer->layerType() == LayerTypeTile)
        {
            const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
            const MapTile& told = tl->tiles().at(getTileIndex(m_playerX, m_playerY));
            const MapTile& tnew = tl->tiles().at(getTileIndex(m_playerX + x, m_playerY + y));

            if (!tnew.isNull())
            {
                TileEvent event(tnew, tl, m_tilesets[tnew.tilesetId()]);
                onAboutStepTile(event);

                if (!event.isAccepted())
                {
                    // We e.g. hit something solid, abort.
                    return false;
                }
            }

            if (!told.isNull())
            {
                onLeaveTile(TileEvent(told, tl, m_tilesets[told.tilesetId()]));
            }
        }
        else
        {
            const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
            for (MapObject* o : ol->objects())
            {
                const QRect r1(o->x(), o->y(), o->width(), o->height());
                const QRect r2(m_playerX + x, m_playerY + y, m_tileWidth, m_tileHeight);

                if (r1.intersects(r2) && !o->isNull())
                {
                    ObjectEvent event(o, ol);
                    onAboutStepObject(event);

                    if (!event.isAccepted())
                    {
                        // We e.g. hit something solid, abort.
                        return false;
                    }
                }

                onLeaveObject(ObjectEvent(o, ol));
            }
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

    return true;
}


bool Map::movePlayerByPixels(int x, int y)
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
            if (layer->layerType() == LayerTypeTile)
            {
                const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
                const MapTile& told = tl->tiles().at(getTileIndex(oldTileX, oldTileY));
                const MapTile& tnew = tl->tiles().at(getTileIndex(newTileX, newTileY));

                if (!tnew.isNull())
                {
                    TileEvent event(tnew, tl, m_tilesets[tnew.tilesetId()]);
                    onAboutStepTile(event);

                    if (!event.isAccepted())
                    {
                        // We e.g. hit something solid, abort.
                        return false;
                    }

                    onStepTile(event);
                }

                onLeaveTile(TileEvent(told, tl, m_tilesets[told.tilesetId()]));
            }
            else
            {
                const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
                for (MapObject* o : ol->objects())
                {
                    const QRect r1(o->x(), o->y(), o->width(), o->height());
                    const QRect r2(m_playerX + x, m_playerY + y, m_tileWidth, m_tileHeight);

                    if (r1.intersects(r2) && !o->isNull())
                    {
                        ObjectEvent event(o, ol);
                        onAboutStepObject(event);

                        if (!event.isAccepted())
                        {
                            // We e.g. hit something solid, abort.
                            return false;
                        }

                        onStepObject(event);
                    }

                    onLeaveObject(ObjectEvent(o, ol));
                }
            }
        }
    }

    // Moves the player in an instant.
    m_playerX += x;
    m_playerY += y;

    return true;
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
                if (layer->layerType() == LayerTypeTile)
                {
                    const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
                    const MapTile& tile = tl->tiles().at(getTileIndex(m_playerX, m_playerY));

                    if (!tile.isNull())
                    {
                        onStepTile(TileEvent(tile, tl, m_tilesets[tile.tilesetId()]));
                    }
                }
                else
                {
                    const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
                    for (MapObject* o : ol->objects())
                    {
                        const QRect r1(o->x(), o->y(), o->width(), o->height());
                        const QRect r2(m_playerX, m_playerY, m_tileWidth, m_tileHeight);

                        if (r1.intersects(r2))
                        {
                            onStepObject(ObjectEvent(o, ol));
                        }
                    }
                }
            }
        }
    }
}

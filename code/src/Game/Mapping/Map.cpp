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
#include <Cranberry/Game/Mapping/MapObjectLayer.hpp>
#include <Cranberry/Game/Mapping/MapTileLayer.hpp>
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
    , m_width(0)
    , m_height(0)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_player(new MapPlayer(this))
{
    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::startedMoveTile,
            [this] (const TileEvent& e) -> void { onAboutStepTile(e); }
            );

    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::startedMoveObject,
            [this] (const ObjectEvent& e) -> void { onAboutStepObject(e); }
            );

    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::finishedMoveTile,
            [this] (const TileEvent& e) -> void { onStepTile(e); }
            );

    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::finishedMoveObject,
            [this] (const ObjectEvent& e) -> void { onStepObject(e); }
            );

    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::startedLeaveTile,
            [this] (const TileEvent& e) -> void { onLeaveTile(e); }
            );

    QObject::connect(
            m_player->signals(),
            &MapPlayerEmitter::startedLeaveObject,
            [this] (const ObjectEvent& e) -> void { onLeaveObject(e); }
            );
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


MapOrientation Map::orientation() const
{
    return m_orientation;
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


MapPlayer* Map::player() const
{
    return m_player;
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
    for (MapLayer* layer : m_layers)
    {
        delete layer;
    }

    for (MapTileset* tileset : m_tilesets)
    {
        delete tileset;
    }

    m_layers.clear();
    m_tilesets.clear();

    RenderBase::destroy();
}


void Map::update(const GameTime& time)
{
    updateTransform(time);

    for (MapLayer* layer : m_layers)
    {
        layer->update(time);
    }

    m_player->update(time);
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

    m_player->render();
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

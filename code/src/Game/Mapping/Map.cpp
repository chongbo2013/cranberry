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

    QDomNodeList listTileset = mapNode.elementsByTagName("tileset");
    QDomNodeList listLayer = mapNode.elementsByTagName("layer");

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
        if (!layer->parse(&elem, m_tilesets))
        {
            return cranError(ERRARG(e_03));
        }

        m_layers.append(layer);
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


const QVector<MapTileset*>& Map::tilesets() const
{
    return m_tilesets;
}


const QVector<MapLayer*>& Map::layers() const
{
    return m_layers;
}

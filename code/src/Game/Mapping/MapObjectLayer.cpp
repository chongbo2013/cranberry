////////////////////////////////////////////////////////////////////////////////
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


// Cranberry headers
#include <Cranberry/Game/Mapping/Map.hpp>
#include <Cranberry/Game/Mapping/MapObjectLayer.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QDomElement>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "TMX (layer): Name attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_02, "TMX (object): ID attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_03, "TMX (object): X attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_04, "TMX (object): Y attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_05, "TMX (object): Width attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_06, "TMX (object): Height attribute is missing.")


CRANBERRY_USING_NAMESPACE


MapObjectLayer::MapObjectLayer(Map* parent)
    : MapLayer(parent)
{
}


MapObjectLayer::~MapObjectLayer()
{
    for (MapObject* obj : m_objects)
    {
        delete obj;
    }
}


MapObject* MapObjectLayer::objectByName(const QString& name) const
{
    for (MapObject* obj : m_objects)
    {
        if (obj->name() == name)
        {
            return obj;
        }
    }

    return nullptr;
}


const QVector<MapObject*>& MapObjectLayer::objects() const
{
    return m_objects;
}


bool MapObjectLayer::parse(QDomElement* xmlElement, int layerId)
{
    setLayerId(layerId);

    // Parses all the attributes.
    if (!xmlElement->hasAttribute("name"))
    {
        return cranError(e_01);
    }
    else
    {
        setName(xmlElement->attribute("name"));
    }

    if (xmlElement->hasAttribute("opacity"))
    {
        setOpacity(xmlElement->attribute("opacity").toFloat());
    }

    if (xmlElement->hasAttribute("visible"))
    {
        setVisibility(xmlElement->attribute("visible").toInt() == 1);
    }

    if (xmlElement->hasAttribute("offsetx"))
    {
        setOffsetX(xmlElement->attribute("offsetx").toInt());
    }

    if (xmlElement->hasAttribute("offsety"))
    {
        setOffsetY(xmlElement->attribute("offsety").toInt());
    }

    // Parses the object data.
    QDomNodeList listObject = xmlElement->elementsByTagName("object");
    for (int i = 0; i < listObject.size(); i++)
    {
        MapObject* obj = new MapObject;
        QDomElement elem = listObject.at(i).toElement();
        QDomElement props = elem.elementsByTagName("properties").at(0).toElement();

        if (!elem.hasAttribute("id"))
        {
            delete obj;
            return cranError(e_02);
        }
        else
        {
            obj->setId(elem.attribute("id").toInt());
        }

        if (!elem.hasAttribute("x"))
        {
            delete obj;
            return cranError(e_03);
        }
        else
        {
            obj->setX(elem.attribute("x").toInt());
        }

        if (!elem.hasAttribute("y"))
        {
            delete obj;
            return cranError(e_04);
        }
        else
        {
            obj->setY(elem.attribute("y").toInt());
        }

        float width, height;
        if (!elem.hasAttribute("width"))
        {
            delete obj;
            return cranError(e_05);
        }
        else
        {
            width = elem.attribute("width").toInt();
        }

        if (!elem.hasAttribute("height"))
        {
            delete obj;
            return cranError(e_06);
        }
        else
        {
            height = elem.attribute("height").toInt();
        }

        obj->setSize(width, height);

        if (elem.hasAttribute("name"))
        {
            obj->setName(elem.attribute("name"));
        }

        if (elem.hasAttribute("type"))
        {
            obj->setType(elem.attribute("type"));
        }

        getTmxProperties(&props, obj->properties());

        m_objects.append(obj);
    }

    return true;
}


LayerType MapObjectLayer::layerType() const
{
    return LayerTypeObject;
}


void MapObjectLayer::update(const GameTime& time)
{
    for (MapObject* obj : m_objects)
    {
        obj->update(time);
    }
}


void MapObjectLayer::render()
{
    for (MapObject* obj : m_objects)
    {
        obj->setX(obj->x() + offsetX() + map()->x());
        obj->setY(obj->y() + offsetY() + map()->y());
        obj->setOpacity(obj->opacity() * opacity() * map()->opacity());
        obj->render();
    }
}

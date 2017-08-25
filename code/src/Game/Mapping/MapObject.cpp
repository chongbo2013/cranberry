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
#include <Cranberry/Game/Mapping/MapObject.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>


CRANBERRY_USING_NAMESPACE


MapObject::MapObject()
    : m_id(-1)
    , m_renderObject(nullptr)
    , m_takeOwnership(false)
{
}


MapObject::~MapObject()
{
    if (m_takeOwnership)
    {
        delete m_renderObject;
    }
}


bool MapObject::isNull() const
{
    return m_id == -1;
}


int MapObject::id() const
{
    return m_id;
}


const QString& MapObject::name() const
{
    return m_name;
}


const QString& MapObject::type() const
{
    return m_type;
}


const QVariant& MapObject::propertyValue(const QString& property) const
{
    auto it = m_properties.find(property);
    return it.value();
}


RenderBase* MapObject::renderObject() const
{
    return m_renderObject;
}


QMap<QString, QVariant>& MapObject::properties()
{
    return m_properties;
}


void MapObject::setId(int id)
{
    m_id = id;
}


void MapObject::setName(const QString &name)
{
    m_name = name;
}


void MapObject::setType(const QString &type)
{
    m_type = type;
}


void MapObject::setRenderObject(RenderBase* obj, bool takeOwnership)
{
    m_takeOwnership = takeOwnership;
    m_renderObject = obj;
}


void MapObject::update(const GameTime& time)
{
    if (m_renderObject != nullptr)
    {
        m_renderObject->update(time);
    }
}


void MapObject::render()
{
    if (m_renderObject != nullptr)
    {
        copyTransform(this, m_renderObject, true);
        m_renderObject->render();
    }
}

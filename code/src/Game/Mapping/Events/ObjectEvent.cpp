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
#include <Cranberry/Game/Mapping/Events/ObjectEvent.hpp>
#include <Cranberry/Game/Mapping/MapObject.hpp>
#include <Cranberry/Game/Mapping/MapObjectLayer.hpp>


CRANBERRY_USING_NAMESPACE


ObjectEvent::ObjectEvent(const MapObject* obj, const MapObjectLayer* layer)
    : m_obj(obj)
    , m_layer(layer)
    , m_isAccepted(true)
{
}


const MapObject& ObjectEvent::object() const
{
    return *m_obj;
}


const MapObjectLayer& ObjectEvent::layer() const
{
    return *m_layer;
}


bool ObjectEvent::isAccepted() const
{
    return m_isAccepted;
}


void ObjectEvent::accept() const
{
    m_isAccepted = true;
}


void ObjectEvent::reject() const
{
    m_isAccepted = false;
}

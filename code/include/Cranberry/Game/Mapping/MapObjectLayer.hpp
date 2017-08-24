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
#ifndef CRANBERRY_GAME_MAPPING_MAPOBJECTLAYER_HPP
#define CRANBERRY_GAME_MAPPING_MAPOBJECTLAYER_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/MapLayer.hpp>
#include <Cranberry/Game/Mapping/MapObject.hpp>

// Qt headers
#include <QVector>

// Forward declarations
CRANBERRY_FORWARD_Q(QDomElement)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines an object layer with multiple objects in it.
///
/// \class MapObjectLayer
/// \author Nicolas Kogler
/// \date August 24, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapObjectLayer final : public MapLayer
{
public:

    CRANBERRY_DECLARE_DTOR(MapObjectLayer)
    CRANBERRY_DISABLE_COPY(MapObjectLayer)
    CRANBERRY_DISABLE_MOVE(MapObjectLayer)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new MapLTileayer with the given Map as parent.
    ///
    /// \param parent The parent of this layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapObjectLayer(Map* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a map object by name.
    ///
    /// \param name Name of the object.
    /// \returns a nullptr if not successful.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapObject* objectByName(const QString& name) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all objects of that layer.
    ///
    /// \returns all objects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<MapObject*>& objects() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Parses the given layer XML element.
    ///
    /// \param xmlElement Layer element to parse.
    /// \param layerId Index of this layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool parse(QDomElement* xmlElement, int layerId);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    LayerType layerType() const override;
    void render() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QVector<MapObject*> m_objects;
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapObjectLayer
/// \ingroup Game
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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


#pragma once
#ifndef CRANBERRY_GAME_MAPPING_MAPLAYER_HPP
#define CRANBERRY_GAME_MAPPING_MAPLAYER_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/Enumerations.hpp>
#include <Cranberry/System/GameTime.hpp>

// Forward declarations
CRANBERRY_FORWARD_C(Map)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Abstraction between various layer types in TMX.
///
/// \class MapLayer
/// \author Nicolas Kogler
/// \date August 23, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapLayer
{
public:

    CRANBERRY_DEFAULT_DTOR(MapLayer)
    CRANBERRY_DISABLE_COPY(MapLayer)
    CRANBERRY_DISABLE_MOVE(MapLayer)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new MapLayer with the given Map as parent.
    ///
    /// \param parent The parent of this layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapLayer(Map* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the parent map of this layer.
    ///
    /// \returns the map.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Map* map() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this layer.
    ///
    /// \returns the layer name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the opacity of the layer.
    ///
    /// \returns the layer opacity.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float opacity() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the visibility of the layer.
    ///
    /// \returns true if the layer is visible.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isVisible() const;

    ////////////////////////////////////////////////////////////////////////////
    /// The position (index) of the layer in the entire map.
    ///
    /// \returns the layer index.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int layerId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-offset of this layer.
    ///
    /// \returns the X-offset.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int offsetX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-offset of this layer.
    ///
    /// \returns the Y-offset.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int offsetY() const;

    void setName(const QString& name);
    void setOpacity(float opac);
    void setVisibility(bool visible);
    void setLayerId(int id);
    void setOffsetX(int x);
    void setOffsetY(int y);


public overridable:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    virtual LayerType layerType() const = 0;
    virtual void update(const GameTime& time) = 0;
    virtual void render() = 0;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Map*    m_parent;
    QString m_name;
    float   m_opacity;
    bool    m_isVisible;
    int     m_layerId;
    int     m_offsetX;
    int     m_offsetY;
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapLayer
/// \ingroup Game
///
/// When subclassing the Map class, the layers can be accessed and rendered in
/// a custom way.
///
/// \code
/// void MyMap::render()
/// {
///     for (MapLayer* layer : layers())
///     {
///         // Render our player
///         if (layer->name() == "playerLayer")
///         {
///             player()->render();
///         }
///
///         // Mess with the layer somehow
///         layer->setOpacity(0.5f);
///         layer->setOffsetX(16);
///         layer->setOffsetY(16);
///
///         layer->render();
///     }
/// }
/// \endcode
///
/// It is possible to iterate through the layers with a classic \em for loop and
/// check against the iterator variable in order to determine the location of
/// the player in between all those layers, while it gives you less flexibility
/// of where and how your player is rendered.
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

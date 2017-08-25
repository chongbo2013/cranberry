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
#ifndef CRANBERRY_GAME_MAPPING_MAP_HPP
#define CRANBERRY_GAME_MAPPING_MAP_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/MapPlayer.hpp>
#include <Cranberry/Game/Mapping/MapObject.hpp>
#include <Cranberry/Game/Mapping/MapTileset.hpp>
#include <Cranberry/Game/Mapping/Events/TileEvent.hpp>
#include <Cranberry/Game/Mapping/Events/ObjectEvent.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Loads maps from the Tiled Map Editor (TMX) and provides several virtual
/// functions in order to maximize the ability to customize.
///
/// \class Map
/// \author Nicolas Kogler
/// \date August 18, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT Map : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(Map)
    CRANBERRY_DECLARE_DTOR(Map)
    CRANBERRY_DISABLE_COPY(Map)
    CRANBERRY_DISABLE_MOVE(Map)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this map is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the orientation of the map.
    ///
    /// \returns the map orientation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapOrientation orientation() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the width of the map.
    ///
    /// \returns the map width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int width() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the height of the map.
    ///
    /// \returns the map height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int height() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the general grid width of the map.
    ///
    /// \returns the general tile width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileWidth() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the general grid height of the map.
    ///
    /// \returns the general tile height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileHeight() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the back color of the map.
    ///
    /// \returns the background color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QColor& backgroundColor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all tilesets of the map.
    ///
    /// \returns all tilesets.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<MapTileset*>& tilesets() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all layers of the map.
    ///
    /// \returns all layers.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<MapLayer*>& layers() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the player on the map.
    ///
    /// \returns the player.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapPlayer* player() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a layer by the given name.
    ///
    /// \param name Name of the layer to get.
    /// \returns nullptr if a layer with this name does not exist.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapLayer* layerByName(const QString& name) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves an object by the given name.
    ///
    /// \param name Name of the object to get.
    /// \returns nullptr if an object with this name does not exist.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapObject* objectByName(const QString& name) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the properties of the map.
    ///
    /// \returns the map properties.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QMap<QString, QVariant>& properties() const;


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Loads a TMX map from \p mapPath and renders it on \p renderTarget.
    ///
    /// \param mapPath Path to TMX file to load.
    /// \param renderTarget Target to render map on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(const QString& mapPath, Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all resources allocated by this instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void destroy();

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the map.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime &time);

    ////////////////////////////////////////////////////////////////////////////
    /// Renders all the layers of the map. Override this method in order to
    /// render a player sprite or something else in between the layers.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void render();


protected overridable:

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player is about to step on a tile.
    ///
    /// \param event Contains useful information about the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onAboutStepTile(const TileEvent& event) { Q_UNUSED(event); }

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player stepped on the tile.
    ///
    /// \param event Contains useful information about the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onStepTile(const TileEvent& event) { Q_UNUSED(event); }

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player left a tile.
    ///
    /// \param event Contains useful information about the tile.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onLeaveTile(const TileEvent& event) { Q_UNUSED(event); }

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player is about to step on an object.
    ///
    /// \param event Contains useful information about the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onAboutStepObject(const ObjectEvent& event) { Q_UNUSED(event); }

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player stepped on an object.
    ///
    /// \param event Contains useful information about the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onStepObject(const ObjectEvent& event) { Q_UNUSED(event); }

    ////////////////////////////////////////////////////////////////////////////
    /// This method is called when the player left an object.
    ///
    /// \param event Contains useful information about the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void onLeaveObject(const ObjectEvent& event) { Q_UNUSED(event); }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    bool loadTilesets(QDomElement* elem);
    bool loadLayers(QDomElement* elem);
    bool loadProperties(QDomElement* elem);

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    MapOrientation          m_orientation;
    int                     m_width;
    int                     m_height;
    int                     m_tileWidth;
    int                     m_tileHeight;
    QColor                  m_bgColor;
    MapPlayer*              m_player;
    QVector<MapLayer*>      m_layers;
    QVector<MapTileset*>    m_tilesets;
    QMap<QString, QVariant> m_properties;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Map
/// \ingroup Game
///
/// Renders multiple map layers and objects.
///
/// \code
/// Map map;
/// map.create(":/maps/map.tmx");
/// map.player()->setTileX(8);  // set initial X-position
/// map.player()->setTileY(10); // set initial Y-position
/// map.player()->setRenderObject(myObj); // set object to render as player
///
/// ...
///
/// map.render();
/// \endcode
///
/// Of course, it is also possible to sublass the map class and reorder the
/// layers and the player as desired or change tile/object behaviours.
///
/// \code
/// class MyMap : public Map
/// {
/// public overridden:
///
///     // Call base method, then implement custom loading (e.g. spawnpoints).
///     bool create(const QString& mapPath, Window* renderTarget) override;
///
///     // Render layers in a custom order.
///     void render() override;
///
/// protected overridden:
///
///     // Custom behaviour when the player is about to step on a tile. See the
///     // TileEvent docs to see possible use cases.
///     void onAboutStepTile(const TileEvent& event) override;
/// };
/// \endcode
///
/// You can even change the layer and appearance of the player when a certain
/// tile is being stepped on. Be creative!
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

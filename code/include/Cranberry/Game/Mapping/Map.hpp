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
#include <Cranberry/Game/Mapping/MapLayer.hpp>
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
    /// Determines whether the player is currently moving.
    ///
    /// \returns true if the player is moving.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isPlayerMoving() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the orientation of the map.
    ///
    /// \returns the map orientation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MapOrientation orientation() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the move mode of the player.
    ///
    /// \returns the player move mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    PlayerMoveMode playerMoveMode() const;

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
    /// Retrieves the X-position of the player. If playerMoveMode() equals
    /// PlayerMoveTiles, the position is measured in tiles. Measured in pixels
    /// otherwise.
    ///
    /// \returns the X-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int playerX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-position of the player. If playerMoveMode() equals
    /// PlayerMoveTiles, the position is measured in tiles. Measured in pixels
    /// otherwise.
    ///
    /// \returns the Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int playerY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the back color of the map.
    ///
    /// \returns the background color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QColor& backgroundColor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the properties of the map.
    ///
    /// \returns the map properties.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QMap<QString, QVariant>& properties() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the move mode of the player.
    ///
    /// \param mode Either move by tiles or pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setPlayerMoveMode(PlayerMoveMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the imaginary player by \p x and \p y on the map. Will
    /// trigger onAboutStepTile() and onStepTile() respectively.
    ///
    /// \note Synchronise calls to TransformBase::startMovingBy() with this.
    /// \note Scrolling the map can be achieved by using startMovingBy().
    /// \note To move by pixels, use setPlayerMoveMode(PlayerMovePixels).
    ///
    /// \param x Tiles/pixels to advance player in direction X.
    /// \param y Tiles/pixels to advance player in direction Y.
    /// \returns false if the player could not be moved for whatever reason.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool movePlayerBy(int x, int y);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the player to (\p x,\p y) (in tiles) instantly.
    ///
    /// \param x New X-position of the player in tiles.
    /// \param y New Y-position of the player in tiles.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void movePlayerTo(int x, int y);


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


public overridable:

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


protected:

    ////////////////////////////////////////////////////////////////////////////
    // Protected functions
    ////////////////////////////////////////////////////////////////////////////
    const QVector<MapTileset*>& tilesets() const;
    const QVector<MapLayer*>& layers() const;
    const QVector<MapObject>& objects() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    int getTileIndex(int, int);
    void updateTileMovement(double);

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    MapOrientation          m_orientation;
    PlayerMoveMode          m_moveMode;
    MoveDirections          m_playerMoveDir;
    int                     m_width;
    int                     m_height;
    int                     m_tileWidth;
    int                     m_tileHeight;
    int                     m_playerX;
    int                     m_playerY;
    int                     m_targetX;
    int                     m_targetY;
    float                   m_realposX;
    float                   m_realposY;
    float                   m_realtargetX;
    float                   m_realtargetY;
    bool                    m_playerMovingX;
    bool                    m_playerMovingY;
    QColor                  m_bgColor;
    QVector<MapLayer*>      m_layers;
    QVector<MapTileset*>    m_tilesets;
    QVector<MapObject>      m_objects;
    QVector<MapObject*>     m_aboutStepObjs;
    QMap<QString, QVariant> m_properties;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Map
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

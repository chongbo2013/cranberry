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
#ifndef CRANBERRY_MAPPLAYER_HPP
#define CRANBERRY_MAPPLAYER_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/Enumerations.hpp>
#include <Cranberry/Graphics/Base/TransformBase.hpp>
#include <Cranberry/System/Emitters/MapPlayerEmitter.hpp>
#include <Cranberry/System/Receivers/MapPlayerReceiver.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a player on a map.
///
/// \class MapPlayer
/// \author Nicolas Kogler
/// \date August 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapPlayer : public TransformBase
{
public:

    CRANBERRY_DECLARE_DTOR(MapPlayer)
    CRANBERRY_DEFAULT_COPY(MapPlayer)
    CRANBERRY_DEFAULT_MOVE(MapPlayer)

    MapPlayer(Map* parent);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-position of the player, in tiles.
    ///
    /// \returns the tile-based X-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-position of the player, in tiles.
    ///
    /// \returns the tile-based Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int tileY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the move mode of the player.
    ///
    /// \returns the move mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    PlayerMoveMode moveMode() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the object to render the player with.
    ///
    /// \returns the render object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RenderBase* renderObject() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the X-position, in tiles.
    ///
    /// \param x New tile-based X-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTileX(int x);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the Y-position, in tiles.
    ///
    /// \param y New tile-based Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTileY(int y);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the move mode of the player.
    ///
    /// \param mode New move mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMoveMode(PlayerMoveMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the object to render this player with. The transform properties
    /// of it will be replaced by the transform properties of the MapPlayer. If
    /// you do not want this, synchronise transformations with this object by
    /// issuing "mapPlayer->synchroniseWith(myRenderObject);".
    ///
    /// \param renderObject The object to render the player with.
    /// \param takeOwnership If true, object will be deleted automatically.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRenderObject(RenderBase* renderObject, bool takeOwnership = false);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the player by \p x horizontally and \p y vertically. If the move
    /// mode is PlayerMoveTiles, x and y will be multiplied by the tile width
    /// and the tile height of the map respectively.
    ///
    /// \param x X-offset to move by, either in tiles or pixels.
    /// \param y Y-offset to move by, either in tiles or pixels.
    /// \returns true if moved successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool movePlayerBy(int x, int y);

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the MapPlayer object.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time);

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the MapPlayer object. Renders nothing if the renderObject is
    /// not available.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render();


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    MapPlayerEmitter* signals() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    int getTileIndex(int x, int y);
    bool movePlayerByTiles(int x, int y);
    bool movePlayerByPixels(int x, int y);
    void moveFinished();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    MapPlayerEmitter  m_emitter;
    MapPlayerReceiver m_receiver;
    PlayerMoveMode    m_moveMode;
    Map*              m_parent;
    RenderBase*       m_renderObject;
    bool              m_takeOwnership;

    friend class MapPlayerReceiver;
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapPlayer
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

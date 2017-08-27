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
#include <Cranberry/Game/Mapping/MapPlayer.hpp>
#include <Cranberry/Game/Mapping/MapTileLayer.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>


CRANBERRY_USING_NAMESPACE


MapPlayer::MapPlayer(Map* parent)
    : TransformBase()
    , m_moveMode(PlayerMoveTiles)
    , m_parent(parent)
    , m_renderObject(nullptr)
    , m_takeOwnership(false)
{
    m_receiver.setMapPlayer(this);
    m_receiver.connect(
            signals(),
            &TransformBaseEmitter::finishedMove,
            &m_receiver,
            &MapPlayerReceiver::finishedMove
            );
}


MapPlayer::~MapPlayer()
{
    if (m_takeOwnership)
    {
        delete m_renderObject;
    }
}


int MapPlayer::tileX() const
{
    return static_cast<int>(x() / m_parent->tileWidth());
}


int MapPlayer::tileY() const
{
    return static_cast<int>(y() / m_parent->tileHeight());
}


PlayerMoveMode MapPlayer::moveMode() const
{
    return m_moveMode;
}


RenderBase* MapPlayer::renderObject() const
{
    return m_renderObject;
}


void MapPlayer::setTileX(int x)
{
    setX(x * m_parent->tileWidth());
}


void MapPlayer::setTileY(int y)
{
    setY(y * m_parent->tileHeight());
}


void MapPlayer::setMoveMode(PlayerMoveMode mode)
{
    m_moveMode = mode;
}


void MapPlayer::setRenderObject(RenderBase* renderObject, bool takeOwnership)
{
    m_renderObject = renderObject;
    m_takeOwnership = takeOwnership;
}


bool MapPlayer::movePlayerBy(int x, int y)
{
    // Reject any movement while there is one running.
    if (isMoving() || (x == 0 && y == 0))
    {
        return false;
    }

    if (m_moveMode == PlayerMoveTiles)
    {
        return movePlayerByTiles(x, y);
    }
    else
    {
        return movePlayerByPixels(x, y);
    }
}


void MapPlayer::update(const GameTime& time)
{
    updateTransform(time);

    if (m_renderObject != nullptr)
    {
        m_renderObject->update(time);
    }
}


void MapPlayer::render()
{
    if (m_renderObject != nullptr)
    {
        setSize(m_parent->tileWidth(), m_parent->tileHeight());
        copyTransform(this, m_renderObject);

        m_renderObject->setY(y() - (m_renderObject->height() - height()));
        m_renderObject->render();
    }
}


MapPlayerEmitter* MapPlayer::signals()
{
    return &m_emitter;
}


int MapPlayer::getTileIndex(int x, int y)
{
    return y * m_parent->mapWidth() + x;
}


bool MapPlayer::exceedsMapSize(int x, int y)
{
    QRect bounds(
          static_cast<int>(m_parent->x() / m_parent->tileWidth()),
          static_cast<int>(m_parent->y() / m_parent->tileHeight()),
          static_cast<int>(m_parent->mapWidth()),
          static_cast<int>(m_parent->mapHeight())
          );

    return !bounds.contains(x, y);
}


bool MapPlayer::movePlayerByTiles(int x, int y)
{
    if (exceedsMapSize(tileX() + x, tileY() + y))
    {
        return false;
    }

    for (MapLayer* layer : m_parent->layers())
    {
        if (layer->layerType() == LayerTypeTile)
        {
            const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
            const MapTile& told = tl->tiles().at(getTileIndex(tileX(), tileY()));
            const MapTile& tnew = tl->tiles().at(getTileIndex(tileX() + x, tileY() + y));

            if (!tnew.isNull())
            {
                TileEvent event(tnew, tl, m_parent->tilesets()[tnew.tilesetId()]);
                m_emitter.emitStartedMoveTile(event);

                if (!event.isAccepted())
                {
                    // We e.g. hit something solid, abort.
                    return false;
                }
            }

            if (!told.isNull())
            {
                m_emitter.emitStartedLeaveTile(TileEvent(
                        told,
                        tl,
                        m_parent->tilesets()[told.tilesetId()]
                        ));
            }
        }
        else
        {
            const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
            for (MapObject* o : ol->objects())
            {
                const QRect r1(o->x(), o->y(), o->width(), o->height());
                const QRect r2(tileX() + x, tileY() + y, m_parent->tileWidth(), m_parent->tileHeight());

                if (r1.intersects(r2) && !o->isNull())
                {
                    ObjectEvent event(o, ol);
                    m_emitter.emitStartedMoveObject(event);

                    if (!event.isAccepted())
                    {
                        // We e.g. hit something solid, abort.
                        return false;
                    }
                }

                m_emitter.emitStartedLeaveObject(ObjectEvent(o, ol));
            }
        }
    }

    moveBy(x * m_parent->tileWidth(), y * m_parent->tileHeight());
    return true;
}


bool MapPlayer::movePlayerByPixels(int dx, int dy)
{
    int pX = static_cast<int>(x());
    int pY = static_cast<int>(y());

    int oldTileX = pX / m_parent->tileWidth();
    int oldTileY = pY / m_parent->tileHeight();
    int newTileX = (pX + dx) / m_parent->tileWidth();
    int newTileY = (pY + dy) / m_parent->tileHeight();

    if (exceedsMapSize(newTileX, newTileY))
    {
        return false;
    }

    if (oldTileX != newTileX || oldTileY != newTileY)
    {
        // Current tile changed, send event to all layers.
        for (MapLayer* layer : m_parent->layers())
        {
            if (layer->layerType() == LayerTypeTile)
            {
                const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
                const MapTile& told = tl->tiles().at(getTileIndex(oldTileX, oldTileY));
                const MapTile& tnew = tl->tiles().at(getTileIndex(newTileX, newTileY));

                if (!tnew.isNull())
                {
                    TileEvent event(tnew, tl, m_parent->tilesets()[tnew.tilesetId()]);
                    m_emitter.emitStartedMoveTile(event);

                    if (!event.isAccepted())
                    {
                        // We e.g. hit something solid, abort.
                        return false;
                    }

                    m_emitter.emitFinishedMoveTile(event);
                }

                if (!told.isNull())
                {
                    m_emitter.emitStartedLeaveTile(TileEvent(
                            told,
                            tl,
                            m_parent->tilesets()[told.tilesetId()]
                            ));
                }
            }
            else
            {
                const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
                for (MapObject* o : ol->objects())
                {
                    const QRect r1(o->x(), o->y(), o->width(), o->height());
                    const QRect r2(pX + dx, pY + dy, m_parent->tileWidth(), m_parent->tileHeight());

                    if (r1.intersects(r2) && !o->isNull())
                    {
                        ObjectEvent event(o, ol);
                        m_emitter.emitStartedMoveObject(event);

                        if (!event.isAccepted())
                        {
                            // We e.g. hit something solid, abort.
                            return false;
                        }

                        m_emitter.emitFinishedMoveObject(event);
                    }

                    m_emitter.emitStartedLeaveObject(ObjectEvent(o, ol));
                }
            }
        }
    }

    // Moves the player in an instant.
    setPosition(pX + dx, pY + dy);
    return true;
}


void MapPlayer::moveFinished()
{
    for (MapLayer* layer : m_parent->layers())
    {
        if (layer->layerType() == LayerTypeTile)
        {
            const MapTileLayer* tl = static_cast<MapTileLayer*>(layer);
            const MapTile& tile = tl->tiles().at(getTileIndex(tileX(), tileY()));

            if (!tile.isNull())
            {
                m_emitter.emitFinishedMoveTile(TileEvent(
                      tile,
                      tl,
                      m_parent->tilesets()[tile.tilesetId()]
                      ));
            }
        }
        else
        {
            const MapObjectLayer* ol = static_cast<MapObjectLayer*>(layer);
            for (MapObject* o : ol->objects())
            {
                const QRect r1(o->x(), o->y(), o->width(), o->height());
                const QRect r2((int) x(), (int) y(), m_parent->tileWidth(), m_parent->tileHeight());

                if (r1.intersects(r2))
                {
                    m_emitter.emitFinishedMoveObject(ObjectEvent(o, ol));
                }
            }
        }
    }
}

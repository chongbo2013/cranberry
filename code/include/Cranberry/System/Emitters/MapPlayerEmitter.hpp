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
#ifndef CRANBERRY_SYSTEM_EMITTERS_MAPPLAYEREMITTER_HPP
#define CRANBERRY_SYSTEM_EMITTERS_MAPPLAYEREMITTER_HPP


// Cranberry headers
#include <Cranberry/Game/Mapping/Events/ObjectEvent.hpp>
#include <Cranberry/Game/Mapping/Events/TileEvent.hpp>
#include <Cranberry/System/Emitters/RenderBaseEmitter.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for MapPlayer objects.
///
/// \class MapPlayerEmitter
/// \author Nicolas Kogler
/// \date August 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT MapPlayerEmitter : public RenderBaseEmitter
{
public:

    CRANBERRY_DEFAULT_CTOR(MapPlayerEmitter)
    CRANBERRY_DEFAULT_DTOR(MapPlayerEmitter)
    CRANBERRY_DEFAULT_COPY(MapPlayerEmitter)
    CRANBERRY_DEFAULT_MOVE(MapPlayerEmitter)

    inline void emitStartedMoveTile(const TileEvent& e) { Q_EMIT startedMoveTile(e); }
    inline void emitFinishedMoveTile(const TileEvent& e) { Q_EMIT finishedMoveTile(e); }
    inline void emitStartedMoveObject(const ObjectEvent& e) { Q_EMIT startedMoveObject(e); }
    inline void emitFinishedMoveObject(const ObjectEvent& e) { Q_EMIT finishedMoveObject(e); }
    inline void emitStartedLeaveTile(const TileEvent& e) { Q_EMIT startedLeaveTile(e); }
    inline void emitStartedLeaveObject(const ObjectEvent& e) { Q_EMIT startedLeaveObject(e); }

Q_SIGNALS:

    void startedMoveTile(const TileEvent& e);
    void finishedMoveTile(const TileEvent& e);
    void startedMoveObject(const ObjectEvent& e);
    void finishedMoveObject(const ObjectEvent& e);
    void startedLeaveTile(const TileEvent& e);
    void startedLeaveObject(const ObjectEvent& e);


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

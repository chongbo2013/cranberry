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
#ifndef CRANBERRY_SYSTEM_RECEIVERS_MAPPLAYERRECEIVER_HPP
#define CRANBERRY_SYSTEM_RECEIVERS_MAPPLAYERRECEIVER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_FORWARD_C(MapPlayer)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Receives signals for MapPlayer objects.
///
/// \class MapPlayerReceiver
/// \author Nicolas Kogler
/// \date August 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT MapPlayerReceiver final : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(MapPlayerReceiver)
    CRANBERRY_DEFAULT_DTOR(MapPlayerReceiver)
    CRANBERRY_DEFAULT_COPY(MapPlayerReceiver)
    CRANBERRY_DEFAULT_MOVE(MapPlayerReceiver)

    void setMapPlayer(MapPlayer* MapPlayer);


public Q_SLOTS:

    void finishedMove();


private:

    MapPlayer* m_mapPlayer = nullptr;

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

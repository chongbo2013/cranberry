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
#ifndef CRANBERRY_GAMEMAP_HPP
#define CRANBERRY_GAMEMAP_HPP

// Cranberry headers
#include <Cranberry/Game/Mapping/Map.hpp>


CRANBERRY_USING_NAMESPACE


class GameMap : public Map
{
public:

    CRANBERRY_DEFAULT_CTOR(GameMap)
    CRANBERRY_DEFAULT_DTOR(GameMap)

public overridden:

    void render() override;

protected overridden:

    void onAboutStepTile(const TileEvent& event) override;
};


#endif

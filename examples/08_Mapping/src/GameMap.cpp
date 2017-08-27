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


// Example headers
#include <GameMap.hpp>


void GameMap::onAboutStepTile(const TileEvent& event)
{
    const QVariant& solid = event.properties().value("solid");
    if (solid.isValid() && solid.toBool())
    {
        event.reject();
    }
}


void GameMap::render()
{
    for (int i = 0; i < layers().size(); i++)
    {
        if (i == 2)
        {
            player()->render();
        }

        layers().at(i)->render();
    }
}

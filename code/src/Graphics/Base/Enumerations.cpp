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


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>

// Qt headers
#include <QStringList>


QString cran::getBlendModeString(BlendModes bm)
{
    if (bm == BlendNone)
    {
        return "None";
    }

    QStringList s;

    if ((bm & BlendMultiply) != 0)   s.append("Multiply");
    if ((bm & BlendScreen) != 0)     s.append("Screen");
    if ((bm & BlendOverlay) != 0)    s.append("Overlay");
    if ((bm & BlendDivide) != 0)     s.append("Divide");
    if ((bm & BlendAdd) != 0)        s.append("Add");
    if ((bm & BlendSubtract) != 0)   s.append("Subtract");
    if ((bm & BlendDifference) != 0) s.append("Difference");
    if ((bm & BlendDarken) != 0)     s.append("Darken");
    if ((bm & BlendLighten) != 0)    s.append("Lighten");

    return s.join(" | ");
}


QString cran::getEffectString(Effect e)
{
    switch (e)
    {
    case EffectNone:       return "None";
    case EffectGrayscale:  return "Grayscale";
    case EffectSepia:      return "Sepia";
    case EffectInvert:     return "Invert";
    case EffectSilhouette: return "Silhouette";
    default:               return "Unknown";
    }
}


QString cran::getMoveDirString(MoveDirections md)
{
    if (md == MoveNone)
    {
        return "None";
    }

    QStringList list;
    if ((md & MoveNorth) != 0) list.append("Up");
    if ((md & MoveSouth) != 0) list.append("Down");
    if ((md & MoveEast)  != 0) list.append("Right");
    if ((md & MoveWest)  != 0) list.append("Left");

    return list.join(" | ");
}


QString cran::getScrollModeString(ScrollMode sm)
{
    switch (sm)
    {
    case ScrollNormal:   return "Normal";
    case ScrollInfinite: return "Infinite";
    default:             return "Unknown";
    }
}

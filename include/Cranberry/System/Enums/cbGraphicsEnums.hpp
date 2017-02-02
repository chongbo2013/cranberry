//
//  cranberry: C++ game engine using the Qt framework and OpenGL/ES.
//  Copyright (C) 2017 Nicolas Kogler
//
//  Cranberry is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cranberry is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with cranberry. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef CRANBERRY_GRAPHICSENUMS_HPP
#define CRANBERRY_GRAPHICSENUMS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// This enum specifies the blending modes
/// within cranberry.
///
/// \enum BlendMode
///
enum BlendMode
{
    NoBlend         = 0x0000,   ///< Ignores the blend color.
    MultiplyBlend   = 0x0001,   ///< Multiplies pixel with blend color.
    ScreenBlend     = 0x0002,   ///< 1 - (1 - pixel) * (1 - blend)
    OverlayBlend    = 0x0004,   ///< Combines multiply and screen.
    DivideBlend     = 0x0008,   ///< Divides pixel with blend color.
    AddBlend        = 0x0010,   ///< Adds pixel and blend color.
    SubtractBlend   = 0x0020,   ///< Subtracts pixel and blend color.
    DiffBlend       = 0x0040,   ///< abs(pixel - blend)
    DarkenBlend     = 0x0080,   ///< Picks the smallest red, green and blue.
    LightenBlend    = 0x0100    ///< Picks the greatest red, green and blue.
};


// Constants for the graphics classes
#define VERTEX_0                0
#define VERTEX_1                1
#define VERTEX_2                2
#define VERTEX_3                3
#define ATTRIB_XYZ              0
#define ATTRIB_UV               1
#define ATTRIB_RGBA             2
#define ATTRIB_XYZ_SIZE         3
#define ATTRIB_UV_SIZE          2
#define ATTRIB_RGBA_SIZE        4
#define ATTRIB_UV_OFFSET        (const void*) (sizeof(float) * 3);
#define ATTRIB_RGBA_OFFSET      (const void*) (sizeof(float) * 5);
#define INDEX_BUFFER_SIZE       6

#define CBI_STRIDE              9
#define CBI_VERTEX_COUNT        CBI_STRIDE * 4


// Flag declarations
Q_DECLARE_FLAGS(BlendModes, BlendMode)


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GRAPHICSENUMS_HPP

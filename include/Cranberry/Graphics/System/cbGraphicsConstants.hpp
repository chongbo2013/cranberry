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


#ifndef CRANBERRY_GRAPHICSCONSTANTS_HPP
#define CRANBERRY_GRAPHICSCONSTANTS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


// General
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
#define ATTRIB_UV_OFFSET        ((const void*) (sizeof(float) * 3))
#define ATTRIB_RGBA_OFFSET      ((const void*) (sizeof(float) * 5))
#define INDEX_BUFFER_SIZE       6

// cbImage
#define CBI_STRIDE              9
#define CBI_VERTEX_COUNT        CBI_STRIDE * 4


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GRAPHICSCONSTANTS_HPP

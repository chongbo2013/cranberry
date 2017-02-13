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


#pragma once
#ifndef CRANBERRY_GRAPHICSCONSTANTS_HPP
#define CRANBERRY_GRAPHICSCONSTANTS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


// cran::Image
#define IMAGE_VERTEX_COUNT              6
#define IMAGE_VERTEX_POS                0
#define IMAGE_VERTEX_COORD              1
#define IMAGE_VERTEX_COLOR              2
#define IMAGE_VERTEX_POS_SIZE           3
#define IMAGE_VERTEX_COORD_SIZE         2
#define IMAGE_VERTEX_COLOR_SIZE         4
#define IMAGE_VERTEX_POS_OFFSET         (const void*) (nullptr)
#define IMAGE_VERTEX_COORD_OFFSET       (const void*) (sizeof(float) * 3)
#define IMAGE_VERTEX_COLOR_OFFSET       (const void*) (sizeof(float) * 5)
#define IMAGE_VERTEX_STRIDE             (int) (sizeof(float) * 9)
#define IMAGE_VERTEX_SIZE               (int) (sizeof(float) * 36)
#define IMAGE_SHADER_VERT               ":/shaders/image_vertex.glsl"
#define IMAGE_SHADER_FRAG               ":/shaders/image_fragment.glsl"


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GRAPHICSCONSTANTS_HPP

﻿#version %0

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


// Input variables
layout(location = 0) in vec2 i_xy;
layout(location = 1) in vec2 i_uv;
layout(location = 2) in int  i_id;

// Output variables
out vec2 o_uv;
out int  o_id;

// Uniform variables
uniform mat4 u_mvp;


void main()
{
    o_uv = i_uv;
    o_id = i_id;
    gl_Position = u_mvp * vec4(i_xy, 0.0, 1.0);
}

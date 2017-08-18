#version %0

////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt5 framework.
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
in vec2 o_uv;
flat in int o_id;

// Output variables
out vec4 o_pixel;

// Cranberry uniform variables
uniform sampler2D u_set0;
uniform sampler2D u_set1;
uniform sampler2D u_set2;
uniform sampler2D u_set3;
uniform sampler2D u_set4;
uniform sampler2D u_set5;
uniform sampler2D u_set6;
uniform sampler2D u_set7;
uniform sampler2D u_set8;
uniform sampler2D u_set9;
uniform float u_opac;


void main()
{
    vec4 vecOpac = vec4(1.0, 1.0, 1.0, u_opac);

    // Big and boring comparison, since sampler arrays would have limitations...
    if (o_id == 0)      o_pixel = texture(u_set0, o_uv) * vecOpac;
    else if (o_id == 1) o_pixel = texture(u_set1, o_uv) * vecOpac;
    else if (o_id == 2) o_pixel = texture(u_set2, o_uv) * vecOpac;
    else if (o_id == 3) o_pixel = texture(u_set3, o_uv) * vecOpac;
    else if (o_id == 4) o_pixel = texture(u_set4, o_uv) * vecOpac;
    else if (o_id == 5) o_pixel = texture(u_set5, o_uv) * vecOpac;
    else if (o_id == 6) o_pixel = texture(u_set6, o_uv) * vecOpac;
    else if (o_id == 7) o_pixel = texture(u_set7, o_uv) * vecOpac;
    else if (o_id == 8) o_pixel = texture(u_set8, o_uv) * vecOpac;
    else if (o_id == 9) o_pixel = texture(u_set9, o_uv) * vecOpac;
    else                o_pixel = vec4(0, 0, 0, 0);
}

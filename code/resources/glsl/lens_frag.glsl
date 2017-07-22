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
in vec4 o_rgba; // unused

// Output variables
out vec4 o_pixel;

// Cranberry uniform variables
uniform sampler2D u_tex;
uniform float u_opac;
uniform int u_mode; // unused
uniform int u_effect; // unused
uniform vec2 u_winSize; // unused

// Lens uniform variables
uniform float u_radiusX; // Start of the black area
uniform float u_radiusY; // End of the black area
uniform vec4 u_color;    // Lens surrounding color.

// ------------------
// Parameter meanings
// u_radiusX (range: 0 - 1)
// ---------
// If smaller, the visible area is smaller.
//
// u_radiusY (range: 0 - 1)
// ---------
// 1. If bigger than u_radiusX, lens is bent to the outside.
// 2. If smaller than u_radiusX, lens is bent to the inside.
// 3. If distance to u_radiusX is bigger, gradient is bigger.
// ------------------


void main()
{
    float dist = distance(o_uv, vec2(0.5, 0.5));
    float sstep = smoothstep(u_radiusX, u_radiusY, dist);

    vec4 src = vec4(u_color.rgb, u_color.a * (1.0 - sstep));
    vec4 dst = texture(u_tex, o_uv);

    float r = (src.a * src.r + dst.a * (1.0 - src.a) * dst.r);
    float g = (src.a * src.g + dst.a * (1.0 - src.a) * dst.g);
    float b = (src.a * src.b + dst.a * (1.0 - src.a) * dst.b);
    float a = (src.a + dst.a * (1.0 - src.a));

    o_pixel = vec4(r, g, b, a);
}

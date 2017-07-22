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

// Radial blur uniform variables
uniform float u_blur;   // blur factor
uniform float u_bright; // brightness factor
uniform int u_offset;   // blur offset (pass count)
uniform vec2 u_origin;  // blur position


void main()
{
    vec2 uv = o_uv;
    vec4 final = vec4(0.0, 0.0, 0.0, 0.0);

    // Gets the texture size.
    ivec2 size = textureSize(u_tex, 0);
    vec2 origin = u_origin / size;
    float radW = 1.0 / size.x;
    float radH = 1.0 / size.y;

    if (origin.x == 0 && origin.y == 0)
    {
        // Changes the origin to the middle of the texture if not specified.
        origin.x = 0.5;
        origin.y = 0.5;
    }

    uv += vec2(radW, radH) * 0.5 - origin;

    // Applies the radial blur.
    int noffset = u_offset - 1;
    for (int i = 0; i < u_offset; i++)
    {
        float factor = float(i) / noffset;
        float scale = 1.0 - u_blur * factor;

        final += texture(u_tex, uv * scale + origin);
    }

    o_pixel = final / float(u_offset) * u_bright;
}

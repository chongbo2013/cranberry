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

// Hatch uniform variables
uniform float u_offset;      // hatch offset
uniform float u_threshold_1; // threshold 1
uniform float u_threshold_2; // threshold 1
uniform float u_threshold_3; // threshold 1
uniform float u_threshold_4; // threshold 1


void main()
{
    vec3 final = vec3(1.0, 1.0, 1.0);
    vec4 pixel = texture(u_tex, o_uv);
    float limit = length(pixel.rgb);

    // Checks against the several limits.
    if (limit < u_threshold_1)
    {
        if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)
        {
            final = vec3(0.0, 0.0, 0.0);
        }
    }

    if (limit < u_threshold_2)
    {
        if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)
        {
            final = vec3(0.0, 0.0, 0.0);
        }
    }

    if (limit < u_threshold_3)
    {
        if (mod(gl_FragCoord.x + gl_FragCoord.y - u_offset, 10.0) == 0.0)
        {
            final = vec3(0.0, 0.0, 0.0);
        }
    }

    if (limit < u_threshold_4)
    {
        if (mod(gl_FragCoord.x - gl_FragCoord.y - u_offset, 10.0) == 0.0)
        {
            final = vec3(0.0, 0.0, 0.0);
        }
    }

    // Finally applies opacity and sets it as shader output.
    o_pixel = vec4(final.rgb, pixel.a * u_opac);
}

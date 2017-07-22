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

// Kaleido uniform variables
uniform float u_radius; // size of the rotated area
uniform float u_angle;  // amount of degrees to rotate
uniform vec2 u_origin;  // origin of the rotated area


void main()
{
    // Calculate absolute rotation point.
    ivec2 size = textureSize(u_tex, 0);
    vec2 coord = (o_uv * size) - u_origin;

    // Check if texel is in the distance of the effect.
    float dist = length(coord);
    if (dist < u_radius && u_angle != 0.0)
    {
        float ratio = (u_radius - dist) / u_radius;
        float t = ratio * ratio * u_angle * 8.0;
        float s = sin(t);
        float c = cos(t);

        coord = vec2(dot(coord, vec2(c, -s)), dot(coord, vec2(s, c)));
    }

    // Restore texture coordinate and fetch texel.
    o_pixel = texture(u_tex, (coord + u_origin) / size);
}

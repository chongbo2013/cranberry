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

// Kaleido uniform variables
uniform float u_sides; // reflection count
uniform float u_angle; // initial angle

// Constant variables
const float pi2 = 2.0 * 3.1416;


void main()
{
    // Calculate the angle.
    vec2 initUV = o_uv - 0.5;
    float magUV = length(initUV);
    float angle = atan(initUV.y, initUV.x) + u_angle;

    // Calculate the rotation around center point.
    angle = mod(angle, pi2 / u_sides);
    angle = abs(angle - pi2 / u_sides / 2.0);
    initUV = magUV * vec2(cos(angle), sin(angle));

    // Fetch texel and sets it as output.
    o_pixel = texture(u_tex, initUV + 0.5) * vec4(1.0, 1.0, 1.0, u_opac);
}

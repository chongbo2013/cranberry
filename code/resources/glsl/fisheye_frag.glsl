#version %0

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
in vec2 o_uv;
in vec4 o_pos;
in vec4 o_rgba; // unused

// Output variables
out vec4 o_pixel;

// Cranberry uniform variables
uniform sampler2D u_tex;
uniform float u_opac;

// Fisheye uniform variables
uniform float u_radius; // radius of fisheye
uniform float u_bend;   // bend factor of fisheye

// Constant variables
const float pi = 3.1415926535;
const float strength = 1.4427;
const float epsilon = 0.000011;
const vec4 corr_1 = vec4(0.5, 0.5, 1.0, 1.0);
const vec4 corr_2 = vec4(1.0, 1.0, 0.0, 0.0);
const vec4 corr_3 = vec4(1.5, 1.5, 0.0, 1.0);


void main()
{
    float bend = abs(u_bend);

    // Calculate fisheye position.
    vec4 pos = (corr_1 + (o_pos + corr_2)) - corr_3;
    float opt = u_radius / log2(bend * u_radius + 1.0) / strength;
    float rad = sqrt(pos.x * pos.x + pos.y * pos.y);

    float angleX = pos.x / rad;
    float angleY = pos.y / rad;

    // Determines which radius to take.
    float radiusToTake = 0.0;
    if (u_bend > 0.0)
    {
        radiusToTake = (exp2((rad / opt) * strength) - 1.0) / bend;
    }
    else
    {
        radiusToTake = (log2(bend * rad + 1.0) / strength) * opt;
    }

    // Determine texture coordinate.
    vec4 uv = vec4(0.0, 0.0, 0.0, 1.0);
    uv.x = radiusToTake * angleX + 0.5;
    uv.y = radiusToTake * angleY + 0.5;
    uv = rad <= u_radius ? uv : pos;
    uv = bend < epsilon ? pos : uv;

    // Fetches the texel and sets it as output.
    o_pixel = texture(u_tex, vec2(uv));
}

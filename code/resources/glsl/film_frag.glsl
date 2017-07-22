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

// Film uniform variables
uniform int u_time; // default: 0
uniform float u_noise; // default: 0.5
uniform float u_lines; // default: 0.05
uniform float u_count; // default: 4096

// Functions
float rand(vec2 coord);


void main()
{
    vec4 vecPixel = texture(u_tex, o_uv);
    float t = float(u_time % 1000);

    // Creates noise and adds it to the pixel.
    float noise = rand(o_uv + t);
    float clmpd = clamp(0.1 + noise, 0.0, 1.0);
    vec3 result = vecPixel.rgb + vecPixel.rgb * clmpd;
    vec2 scanln = vec2(sin(o_uv.y * u_count), cos(o_uv.y * u_count));

    // Adds scanlines to the pixel and converts it to grayscale.
    result += vecPixel.rgb * vec3(scanln.x, scanln.y, scanln.x) * u_lines;
    result  = vecPixel.rgb + clamp(u_noise, 0.0, 1.0) * (result - vecPixel.rgb);
    result  = vec3(result.r * 0.3 + result.g * 0.59 + result.b * 0.11);

    // Finally applies opacity and sets it as shader output.
    o_pixel = vec4(result, vecPixel.a * u_opac);
}


float rand(vec2 coord)
{
    float a = 12.9898;
    float b = 78.233;
    float c = 43758.5453;
    float d = dot(coord.xy, vec2(a, b));
    float e = mod(d, 3.14);
    return fract(sin(e) * c);
}

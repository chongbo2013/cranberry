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
uniform vec2 u_winSize;

// Blur uniform variables
uniform float u_pixelW; // pixel width
uniform float u_pixelH; // pixel height
uniform float u_width;  // render target width
uniform float u_height; // render target height


void main()
{
    // Finds new coordinate position.
    vec4 vecOpac = vec4(1.0, 1.0, 1.0, u_opac);
    float ratioX = u_pixelW * (1.0 / u_winSize.x);
    float ratioY = u_pixelH * (1.0 / u_winSize.y);
    vec2 newUV = vec2(
            ratioX * floor(o_uv.x / ratioX),
            ratioY * floor(o_uv.y / ratioY)
            );

    // Fetches it, applies opacity and sets it as shader output.
    o_pixel = texture(u_tex, newUV) * vecOpac;
}

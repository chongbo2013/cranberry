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

// Blur uniform variables
uniform float u_blurH;  // blur factor (horizontal)
uniform float u_blurV;  // blur factor (vertical)
uniform float u_width;  // render target width
uniform float u_height; // render target height


void main()
{
    vec4 final = vec4(0.0, 0.0, 0.0, 0.0);
    float factorH = u_blurH / u_width;
    float factorV = u_blurV / u_height;

    // Horizontal blur
    if (u_blurH != 0.0)
    {
        final += texture(u_tex, vec2(o_uv.x - 4.0 * factorH, o_uv.y)) * 0.051;
        final += texture(u_tex, vec2(o_uv.x - 3.0 * factorH, o_uv.y)) * 0.0918;
        final += texture(u_tex, vec2(o_uv.x - 2.0 * factorH, o_uv.y)) * 0.12245;
        final += texture(u_tex, vec2(o_uv.x - 1.0 * factorH, o_uv.y)) * 0.1531;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y)) * 0.1633;
        final += texture(u_tex, vec2(o_uv.x + 1.0 * factorH, o_uv.y)) * 0.1531;
        final += texture(u_tex, vec2(o_uv.x + 2.0 * factorH, o_uv.y)) * 0.12245;
        final += texture(u_tex, vec2(o_uv.x + 3.0 * factorH, o_uv.y)) * 0.0918;
        final += texture(u_tex, vec2(o_uv.x + 4.0 * factorH, o_uv.y)) * 0.051;
    }

    // Vertical blur
    if (u_blurV != 0.0)
    {
        final += texture(u_tex, vec2(o_uv.x, o_uv.y - 4.0 * factorV)) * 0.051;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y - 3.0 * factorV)) * 0.0918;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y - 2.0 * factorV)) * 0.12245;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y - 1.0 * factorV)) * 0.1531;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y)) * 0.1633;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y + 1.0 * factorV)) * 0.1531;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y + 2.0 * factorV)) * 0.12245;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y + 3.0 * factorV)) * 0.0918;
        final += texture(u_tex, vec2(o_uv.x, o_uv.y + 4.0 * factorV)) * 0.051;
    }

    // Finally applies opacity and sets it as shader output.
    o_pixel = vec4(final.rgb, final.a * u_opac);
}

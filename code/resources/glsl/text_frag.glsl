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
uniform int u_outlineWidth;


void main()
{
    vec4 final;
    if (u_outlineWidth != 0)
    {
        // Outline already anti-aliased, we do not need any fake blur.
        final = texture(u_tex, o_uv);
    }
    else
    {
        // Thanks to Qt, our text is very jaggy - create some fake blur.
        ivec2 texSize = textureSize(u_tex, 0);

        vec2 blur = vec2(0.65, 0.65);
        vec2 tile = vec2(1.0 / texSize.x, 1.0 / texSize.y);
        vec2 pos = vec2(fract(o_uv.x / tile.x), fract(o_uv.y / tile.y));

        vec2 p0 = vec2(floor(o_uv.x / tile.x) * tile.x, floor(o_uv.y / tile.y) * tile.y);
        vec2 p1 = vec2(clamp(p0.x - tile.x, 0.0, 1.0), p0.y);
        vec2 p2 = vec2(p0.x, clamp(p0.y - tile.y, 0.0, 1.0));
        vec2 p3 = vec2(p1.x, p2.y);
        vec2 mf = vec2(min(pos.x / blur.x, 1.0), min(pos.y / blur.y, 1.0));

        vec4 t1 = texture(u_tex, p1 + (tile / 2.0));
        vec4 t2 = texture(u_tex, p0 + (tile / 2.0));
        vec4 t3 = texture(u_tex, p3 + (tile / 2.0));
        vec4 t4 = texture(u_tex, p2 + (tile / 2.0));

        vec4 m1 = mix(t1, t2, mf.x);
        vec4 m2 = mix(t3, t4, mf.x);

        final = mix(m2, m1, mf.y);
    }

    // Finally applies opacity and sets it as shader output.
    o_pixel = final * vec4(1.0, 1.0, 1.0, u_opac);
}

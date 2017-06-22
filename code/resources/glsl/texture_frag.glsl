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


#version %0

// Input variables
in vec2 o_uv;
in vec4 o_rgba;

// Output variables
out vec4 o_pixel;

// Uniform variables
uniform sampler2D u_tex;
uniform float u_opac;
uniform int u_mode;

// Functions
vec4 applyMultiply(vec4, vec4);
vec4 applyScreen(vec4, vec4);
vec4 applyOverlay(vec4, vec4);
vec4 applyDivide(vec4, vec4);
vec4 applyAdd(vec4, vec4);
vec4 applySubtract(vec4, vec4);
vec4 applyDiff(vec4, vec4);
vec4 applyDarken(vec4, vec4);
vec4 applyLighten(vec4, vec4);


void main()
{
    vec4 vecOpac = vec4(1.0, 1.0, 1.0, u_opac);
    vec4 vecPixel = texture(u_tex, o_uv);

    // Apply blend modes
    if ((u_mode & 0x0001) != 0) { // Multiply
        vecPixel = applyMultiply(vecPixel, o_rgba);
    } if ((u_mode & 0x0002) != 0) { // Screen
        vecPixel = applyScreen(vecPixel, o_rgba);
    } if ((u_mode & 0x0004) != 0) { // Overlay
        vecPixel = applyOverlay(vecPixel, o_rgba);
    } if ((u_mode & 0x0008) != 0) { // Divide
        vecPixel = applyDivide(vecPixel, o_rgba);
    } if ((u_mode & 0x0010) != 0) { // Add
        vecPixel = applyAdd(vecPixel, o_rgba);
    } if ((u_mode & 0x0020) != 0) { // Subtract
        vecPixel = applySubtract(vecPixel, o_rgba);
    } if ((u_mode & 0x0040) != 0) { // Difference
        vecPixel = applyDiff(vecPixel, o_rgba);
    } if ((u_mode & 0x0080) != 0) { // Darken
        vecPixel = applyDarken(vecPixel, o_rgba);
    } if ((u_mode & 0x0100) != 0) { // Lighten
        vecPixel = applyLighten(vecPixel, o_rgba);
    }

    o_pixel = vecPixel * vecOpac;
}


vec4 applyMultiply(vec4 pixel, vec4 blend)
{
    return vec4(pixel.rgb * blend.rgb, pixel.a);
}


vec4 applyScreen(vec4 pixel, vec4 blend)
{
    vec3 vecInvert = vec3(1.0, 1.0, 1.0);
    vec3 vecPixInv = vecInvert.rgb - pixel.rgb;
    vec3 vecBldInv = vecInvert.rgb - blend.rgb;

    return vec4(vecInvert - (vecPixInv * vecBldInv), pixel.a);
}


vec4 applyOverlay(vec4 pixel, vec4 blend)
{
    // Red channel
    if (pixel.r < 0.5)
        pixel.r = 2 * pixel.r * blend.r;
    else
        pixel.r = 1 - (2 * (1 - pixel.r) * (1 - blend.r));

    // Green channel
    if (pixel.g < 0.5)
        pixel.g = 2 * pixel.g * blend.g;
    else
        pixel.g = 1 - (2 * (1 - pixel.g) * (1 - blend.g));

    // Blue channel
    if (pixel.b < 0.5)
        pixel.b = 2 * pixel.b * blend.b;
    else
        pixel.b = 1 - (2 * (1 - pixel.b) * (1 - pixel.b));

    return pixel;
}


vec4 applyDivide(vec4 pixel, vec4 blend)
{
    return vec4(pixel.rgb / blend.rgb, pixel.a);
}


vec4 applyAdd(vec4 pixel, vec4 blend)
{
    return vec4(pixel.rgb + blend.rgb, pixel.a);
}


vec4 applySubtract(vec4 pixel, vec4 blend)
{
    return vec4(pixel.rgb - blend.rgb, pixel.a);
}


vec4 applyDiff(vec4 pixel, vec4 blend)
{
    return vec4(abs(pixel.rgb - blend.rgb), pixel.a);
}


vec4 applyDarken(vec4 pixel, vec4 blend)
{
    return vec4(min(pixel.r, blend.r),
                min(pixel.g, blend.g),
                min(pixel.b, blend.b),
                min(pixel.a, blend.a));
}


vec4 applyLighten(vec4 pixel, vec4 blend)
{
    return vec4(max(pixel.r, blend.r),
                max(pixel.g, blend.g),
                max(pixel.b, blend.b),
                max(pixel.a, blend.a));
}

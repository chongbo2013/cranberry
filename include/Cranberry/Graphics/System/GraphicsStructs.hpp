//
//  cranberry: C++ game engine using the Qt framework and OpenGL/ES.
//  Copyright (C) 2017 Nicolas Kogler
//
//  Cranberry is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cranberry is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with cranberry. If not, see <http://www.gnu.org/licenses/>.
//


#pragma once
#ifndef CRANBERRY_GRAPHICSSTRUCTS_HPP
#define CRANBERRY_GRAPHICSSTRUCTS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QColor>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds a primitive vertex that consists
/// of XYZ position and RGBA color components.
///
/// \struct VxPrimitive
/// \author Nicolas Kogler
/// \date February 11, 2017
///
struct VxPrimitive
{
    // Members
    float x, y, z;
    float r, g, b, a;


    ///
    /// Initializes the alpha value to one.
    ///
    VxPrimitive() : a(1.f) { }

    ///
    /// Specifies the vertex position.
    ///
    /// \param _x X-position.
    /// \param _y Y-position.
    /// \param _z Z-position.
    ///
    inline void xyz(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    ///
    /// Specifies the vertex color.
    ///
    /// \param color The color of the vertex.
    ///
    inline void rgba(const QColor& color)
    {
        r = color.redF();
        g = color.greenF();
        b = color.blueF();
        a = color.alphaF();
    }
};


///
/// Holds a texture vertex that consists
/// of XYZ positon, UV coordinates and RGBA
/// color components.
///
/// \struct VxTexture
/// \author Nicolas Kogler
/// \date February 11, 2017
///
struct VxTexture
{
    float x, y, z;
    float u, v;
    float r, g, b, a;


    ///
    /// Initializes the alpha value to one.
    ///
    VxTexture() : a(1.f) { }

    ///
    /// Specifies the vertex position.
    ///
    /// \param _x X-position.
    /// \param _y Y-position.
    /// \param _z Z-position.
    ///
    inline void xyz(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    ///
    /// Specifies the texture coordinates.
    ///
    /// \param _u U-coordinate.
    /// \param _v V-coordinate.
    ///
    inline void uv(float _u, float _v)
    {
        u = _u;
        v = _v;
    }

    ///
    /// Specifies the vertex color.
    ///
    /// \param color The color of the vertex.
    ///
    inline void rgba(const QColor& color)
    {
        r = color.redF();
        g = color.greenF();
        b = color.blueF();
        a = color.alphaF();
    }
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GRAPHICSSTRUCTS_HPP

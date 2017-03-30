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
#ifndef CRANBERRY_TEXTUREATLAS_HPP
#define CRANBERRY_TEXTUREATLAS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Standard headers
#include <vector>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a texture atlas to pack multiple
/// textures in one big texture. Algorithm
/// based on code from Jukka Jylänki with
/// some performance adjustments.
///
/// \class TextureAtlas
/// \author Nicolas Kogler
/// \date March 28, 2017
///
class CRANBERRY_EXPORT TextureAtlas
{
public:

    CRANBERRY_DEFAULT_COPY(TextureAtlas)
    CRANBERRY_DEFAULT_MOVE(TextureAtlas)
    CRANBERRY_DEFAULT_DTOR(TextureAtlas)

    ///
    /// Initializes a new instance of TextureAtlas.
    ///
    /// \param width Maximum width of the texture.
    /// \param height Maximum height of the texture.
    ///
    TextureAtlas(int width, int height);


    ///
    /// Resets this atlas and makes space
    /// for a new one.
    ///
    /// \param width Width of the new atlas.
    /// \param height Height of the new atlas.
    ///
    void reset(int width, int height);

    ///
    /// \brief fit
    /// \param requestedSize Size of rect to insert into texture.
    /// \returns a fitting rectangle.
    ///
    QRectF find(const QSize& requestedSize);


private:

    // Functions
    bool contains(const QRect& r1, const QRect& r2);
    void place(const QRect& node);
    QRect score(int width, int height);
    bool split(const QRect& free, const QRect& used);
    void tidyUp();

    // Members
    std::vector<QRect> m_used;
    std::vector<QRect> m_free;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_TEXTUREATLAS_HPP

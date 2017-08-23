////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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


#pragma once
#ifndef CRANBERRY_GRAPHICS_BASE_TEXTUREATLAS_HPP
#define CRANBERRY_GRAPHICS_BASE_TEXTUREATLAS_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/TextureBase.hpp>

// Qt headers
#include <QRectF>
#include <QVector>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a texture atlas that holds multiple textures in one big texture.
/// Algorithm based on code from Jukka Jylänki with some adjustments.
///
/// \class TextureAtlas
/// \author Nicolas Kogler
/// \date June 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT TextureAtlas
{
public:

    CRANBERRY_DECLARE_DTOR(TextureAtlas)
    CRANBERRY_DISABLE_COPY(TextureAtlas)
    CRANBERRY_DISABLE_MOVE(TextureAtlas)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new texture atlas with the given size. Size is equal for width
    /// and height. Use ITexture::maxSize() to determine maximum size of OpenGL
    /// textures on the current hardware.
    ///
    /// \param size Size of the texture, in pixels.
    /// \param renderTarget Target to render atlas on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TextureAtlas(int size, Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes the texture atlas with an image.
    ///
    /// \param img Image that spans across the entire atlas.
    /// \param renderTarget Target to render atlas on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TextureAtlas(const QImage& img, Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts a new image into the texture atlas.
    ///
    /// \param img Image to insert.
    /// \returns true if image could be inserted.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool insert(const QImage& img);

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether another image could be inserted into the atlas.
    /// Rule of thumb: If 90% of the space is occupied, do not allow insertion.
    ///
    /// \returns false if can not insert another image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool canInsert() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rectangle inserted the last.
    ///
    /// \returns the last rectangle.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QRect& lastRectangle() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all rectangles inserted so far.
    ///
    /// \returns all the current rectangles.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<QRect>& rectangles() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the big texture associated with this atlas.
    ///
    /// \returns the big texture.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TextureBase* texture() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    void merge();
    void split(const QRect& free, const QRect& used);
    void drawIntoTexture(QImage img, const QRect& src);
    int score(int width, int height, const QRect& free);
    QRect find(int width, int height, int& index);

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* gl;
    QVector<QRect>    m_used;
    QVector<QRect>    m_free;
    qint32            m_size;
    qint64            m_usedSpace;
    qreal             m_occupancy;
    quint32           m_texId;
    TextureBase*      m_texture;
};


////////////////////////////////////////////////////////////////////////////////
/// \class TextureAtlas
/// \ingroup Graphics
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

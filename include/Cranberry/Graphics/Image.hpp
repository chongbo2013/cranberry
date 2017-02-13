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
#ifndef CRANBERRY_IMAGE_HPP
#define CRANBERRY_IMAGE_HPP


// Cranberry headers
#include <Cranberry/Graphics/System/Drawable.hpp>
#include <Cranberry/Graphics/System/GraphicsStructs.hpp>

// Standard headers
#include <array>

// Forward declarations
class QOpenGLVertexArrayObject;
class QOpenGLTexture;
class QMatrix4x4;


CRANBERRY_BEGIN_NAMESPACE


///
/// Renders a simple image.
///
/// \class Image
/// \author Nicolas Kogler
/// \date February 11, 2017
///
class CRANBERRY_EXPORT Image : public Drawable
{
public:

    CRANBERRY_DISABLE_MOVE(Image)

    ///
    /// Initializes a new instance of Image.
    ///
    Image();

    ///
    /// Copies one Image instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Image instance.
    ///
    Image(const Image& other);

    ///
    /// Copies one Image instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Image instance.
    ///
    Image& operator=(const Image& other);

    ///
    /// Destroys this Image instance.
    ///
    virtual ~Image();


    ///
    /// Retrieves the underlying OpenGL texture.
    ///
    /// \returns the texture.
    ///
    QOpenGLTexture* texture() const;

    ///
    /// Retrieves the raw QImage of this object.
    ///
    /// \returns the raw QImage.
    ///
    const QImage& image() const;

    ///
    /// Retrieves the width multiplied with scale.
    ///
    /// \returns the visible width.
    ///
    float visibleWidth() const;

    ///
    /// Retrieves the height multiplied with scale.
    ///
    /// \returns the visible height.
    ///
    float visibleHeight() const;


    ///
    /// Specifies the region of the image to draw.
    ///
    /// \param src Region of the image to draw.
    ///
    void setSourceRectangle(const QRectF& src);

    ///
    /// Specifies the blend color for all vertices.
    ///
    /// \param color The color to blend image with.
    ///
    void setBlendColor(const QColor& color);

    ///
    /// Specifies the blend color for each vertex.
    ///
    /// \param topLeft Top-left vertex blend color.
    /// \param topRight Top-right vertex blend color.
    /// \param btmRight Bottom-right vertex blend color.
    /// \param btmLeft Bottom-left vertex blend color.
    ///
    void setBlendColor(
            const QColor& topLeft,
            const QColor& topRight,
            const QColor& btmRight,
            const QColor& btmLeft);


    ///
    /// Attempts to allocate a new OpenGL texture
    /// and vertex buffer object for this object.
    /// Must either specify a render target by calling
    /// Drawable::setRenderTarget() beforehand or
    /// by passing a window to draw this image on
    /// to this function.
    ///
    /// \param img The image to draw.
    /// \param target The window to draw the image on.
    /// \returns true if successfully created.
    ///
    bool create(const QImage& img, Window* target = nullptr);

    ///
    /// Destroys the underlying OpenGL objects.
    ///
    void destroy() override;

    ///
    /// Updates movement, rotation, scale and fading.
    ///
    /// \param time Contains the delta time.
    ///
    void update(const GameTime &time) override;

    ///
    /// Renders the object in its current state.
    ///
    void render() override;


private:

    // Members
    uint32_t*       m_refCount;
    QOpenGLTexture* m_texture;
    QOpenGLBuffer*  m_indexBuffer;
    QMatrix4x4*     m_mvpMatrix;
    bool            m_needsUpdate;
    bool            m_isInit;

    std::array<VxTexture, 4> m_vertices;

    // Static functions
    static void createOpenGL();
    static void destroyOpenGL();

    // Static variables
    static uint32_t g_instanceCount;
    static QOpenGLVertexArrayObject* g_vao;
    static QOpenGLShaderProgram* g_program;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_IMAGE_HPP

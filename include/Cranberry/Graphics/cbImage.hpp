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


#ifndef CRANBERRY_CBIMAGE_HPP
#define CRANBERRY_CBIMAGE_HPP


// Cranberry headers
#include <Cranberry/Graphics/System/cbDrawable.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QtMath>


CRANBERRY_BEGIN_NAMESPACE


///
/// Renders a simple image on the given render target.
///
/// \class cbImage
/// \author Nicolas Kogler
/// \date February 2nd, 2017
///
class CRANBERRY_EXPORT cbImage : public cbDrawable
{
public:

    ///
    /// Constructor
    /// Constructs an instance of cbImage and specifies
    /// the target on which this image should be rendered.
    /// Equivalent to cbImage::setRenderTarget().
    ///
    /// \param renderTarget The window to render this image on.
    ///
    cbImage(cbWindow* renderTarget = nullptr);

    ///
    /// Copy constructor
    /// Uses implicit resource sharing, so copying an
    /// instance of cbImage will not result in copying
    /// all the pixel and buffer data.
    ///
    /// \param other The other cbImage instance.
    ///
    cbImage(const cbImage& other);

    ///
    /// Move constructor
    ///
    /// \param other The other cbImage instance.
    ///
    cbImage(cbImage&& other) = delete;

    ///
    /// Assignment operator
    /// Uses implicit resource sharing, so copying an
    /// instance of cbImage will not result in copying
    /// all the pixel and buffer data.
    ///
    /// \param other The other cbImage instance.
    ///
    cbImage& operator=(const cbImage& other);

    ///
    /// Destructor
    /// Will free any OpenGL resources, if not already
    /// done by using cbImage::destroy().
    ///
    ~cbImage();


    ///
    /// Retrieves the underlying OpenGL texture.
    ///
    /// \returns the OpenGL texture object.
    ///
    QOpenGLTexture* texture() const;

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
    /// \param source Region of the image to draw.
    ///
    void setSourceRectangle(const QRectF& source);

    ///
    /// Specifies the blend color for all four vertices.
    ///
    /// \param color The color to blend the image with.
    ///
    void setBlendColor(const QColor& color);

    ///
    /// Specifies the blend colors for each vertex.
    ///
    /// \param topLeft Top-left vertex blend color.
    /// \param topRight Top-right vertex blend color.
    /// \param btmRight Bottom-right vertex blend color.
    /// \param btmLeft Bottom-left vertex blend color.
    ///
    void setBlendColors(const QColor& topLeft,
                        const QColor& topRight,
                        const QColor& btmRight,
                        const QColor& btmLeft);


    ///
    /// Attempts to allocate a new OpenGL texture
    /// and vertex buffer storage for this object.
    /// Must specify a render target beforehand.
    ///
    /// \returns true if successful.
    ///
    bool create(const QImage& img);

    ///
    /// Destroys the underlying OpenGL texture.
    ///
    void destroy() override;

    ///
    /// Updates movement, rotation and scale, if
    /// any of these animations are currently running.
    ///
    /// \param time Contains time since start and delta time.
    ///
    void update(const cbGameTime& time) override;

    ///
    /// Draws the object in its current state.
    ///
    void render() override;


private:

    // Members
    uint32_t*       m_refCount;
    QOpenGLTexture* m_texture;
    QMatrix4x4      m_mvpMatrix;
    bool            m_needsUpdate;
    bool            m_isValid;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBIMAGE_HPP

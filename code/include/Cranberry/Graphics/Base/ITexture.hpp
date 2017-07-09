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


#pragma once
#ifndef CRANBERRY_ITEXTURE_HPP
#define CRANBERRY_ITEXTURE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>

// Qt headers
#include <QMatrix4x4>


// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLBuffer)
CRANBERRY_FORWARD_Q(QOpenGLTexture)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines the base class for all texture-based graphics objects.
///
/// \class ITexture
/// \author Nicolas Kogler
/// \date June 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT ITexture
    : public IRenderable
    , public ITransformable
{
public:

    CRANBERRY_DISABLE_COPY(ITexture)
    CRANBERRY_DISABLE_MOVE(ITexture)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of the ITexture class and sets all members
    /// to their logical default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ITexture();

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys this texture. Last resort for releasing all OpenGL resources
    /// before the context will eventually be destroyed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~ITexture();


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the underlying QOpenGLTexture.
    ///
    /// \returns the underlying QOpenGLTexture.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLTexture* texture() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Reimplements IRenderable::isNull(). Adds the condition that the
    /// underlying texture and vertex buffer are valid.
    ///
    /// \returns true if the object is invalid.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// In addition to creating IRenderable, creates the texture and the vertex
    /// buffer.
    ///
    /// \param img The QImage to load the pixel data from.
    /// \param renderTarget Target to render texture on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(const QImage& img, Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// In addition to creating IRenderable, stores the texture and creates
    /// the vertex  buffer. Takes ownership of the texture and deletes it as
    /// soon as this instance is destroyed, therefore allocate it on the heap.
    ///
    /// \param img The heap-allocated QOpenGLTexture to use.
    /// \param renderTarget Target to render texture on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(QOpenGLTexture* img, Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all OpenGL resources allocated for this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the transformations of this object.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void render() override;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the region of the object to be rendered.
    ///
    /// \param rc Region to render.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSourceRectangle(const QRectF& rc);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend color that will be applied on this object. Depends
    /// on the blend mode used.
    ///
    /// \param color Color to use for blending.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend color that will be applied on this object. Depends
    /// on the blend mode used.
    ///
    /// \param tl Top left vertex.
    /// \param tr Top right vertex.
    /// \param br Bottom right vertex.
    /// \param bl Bottom left vertex.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendColor(
            const QColor& tl,
            const QColor& tr,
            const QColor& br,
            const QColor& bl
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend mode to render this object with.
    ///
    /// \param modes One or multiple blending modes.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendMode(BlendModes modes);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the string representation of this object.
    ///
    /// \returns the string representation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    operator QString() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the maximum texture size on the current hardware. This is
    /// needed to pack multiple textures into a single one, while trying to
    /// create as big textures as possible.
    ///
    /// \note Needs an active OpenGL context.
    /// \returns the maximum size (in pixels).
    ///
    ////////////////////////////////////////////////////////////////////////////
    static int maxSize();


protected:

    priv::QuadVertices& vertices();
    QOpenGLBuffer* buffer() const;
    void requestUpdate();
    virtual void initializeData();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    bool createBuffers();
    bool createTexture(const QImage& img);
    auto buildMatrix() -> QMatrix4x4;
    void bindObjects();
    void releaseObjects();
    void writeVertices();
    void modifyProgram();
    void modifyAttribs();
    void drawElements();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    priv::QuadVertices  m_vertices;
    BlendModes          m_blendMode;
    Effect              m_effect;
    QOpenGLTexture*     m_texture;
    QOpenGLBuffer*      m_vertexBuffer;
    QOpenGLBuffer*      m_indexBuffer;
    bool                m_update;
};


typedef std::array<uint, 6> IndexBuf;


////////////////////////////////////////////////////////////////////////////////
/// \class ITexture
/// \ingroup Graphics
///
/// The base class for all texture-based objects.
///
/// \code
/// class Sprite : public ITexture
/// {
/// public:
///
///     ...
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

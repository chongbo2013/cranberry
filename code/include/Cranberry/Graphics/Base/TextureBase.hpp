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
#ifndef CRANBERRY_GRAPHICS_BASE_TEXTUREBASE_HPP
#define CRANBERRY_GRAPHICS_BASE_TEXTUREBASE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>

// Forward declarations and aliases
CRANBERRY_FORWARD_Q(QOpenGLBuffer)
CRANBERRY_FORWARD_Q(QOpenGLTexture)
CRANBERRY_ALIAS_ARR(uint, 6, IndexBuf)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines the base class for all texture-based graphics objects.
///
/// \class TextureBase
/// \author Nicolas Kogler
/// \date June 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT TextureBase : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(TextureBase)
    CRANBERRY_DECLARE_DTOR(TextureBase)
    CRANBERRY_DISABLE_COPY(TextureBase)
    CRANBERRY_DISABLE_MOVE(TextureBase)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the underlying QOpenGLTexture.
    ///
    /// \returns the underlying QOpenGLTexture.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLTexture* texture() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the region of the object to be rendered.
    ///
    /// \param rc Region to render.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSourceRectangle(const QRectF& rc);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the region of the object to be rendered.
    ///
    /// \param x X-position in pixels.
    /// \param y Y-position in pixels.
    /// \param width Region width.
    /// \param height Region height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSourceRectangle(qreal x, qreal y, qreal width, qreal height);

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


public overridden:

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
    /// \param imgPath Holds the path to the image to load.
    /// \param renderTarget Target to render texture on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(const QString& imgPath, Window* renderTarget);

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
    /// Retrieves the root model item of this instance.
    ///
    /// \returns the root model item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual TreeModelItem* rootModelItem() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void updateProperties() override;

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


protected overridable:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    virtual bool initializeData();

    ////////////////////////////////////////////////////////////////////////////
    // Protected functions
    ////////////////////////////////////////////////////////////////////////////
    priv::QuadVertices& vertices();
    QOpenGLBuffer* buffer() const;
    void requestUpdate();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    bool createBuffers();
    bool createTexture(const QImage& img);
    void bindObjects();
    void releaseObjects();
    void writeVertices();
    void modifyProgram();
    void modifyAttribs();
    void drawElements();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem*     m_rootModelItem;
    priv::QuadVertices m_vertices;
    BlendModes         m_blendMode;
    Effect             m_effect;
    QRectF             m_sourceRect;
    QOpenGLTexture*    m_texture;
    QOpenGLBuffer*     m_vertexBuffer;
    QOpenGLBuffer*     m_indexBuffer;
    bool               m_update;
};


////////////////////////////////////////////////////////////////////////////////
/// \class TextureBase
/// \ingroup Graphics
///
/// The base class for all texture-based objects.
///
/// \code
/// class Sprite : public TextureBase
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

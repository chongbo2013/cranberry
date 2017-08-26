////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#ifndef CRANBERRY_SPRITEBATCH_HPP
#define CRANBERRY_SPRITEBATCH_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>
#include <Cranberry/System/GameTime.hpp>

// Qt headers
#include <QList>

// Forward declarations
CRANBERRY_FORWARD_C(Window)
CRANBERRY_FORWARD_C(OpenGLShader)
CRANBERRY_FORWARD_Q(QOpenGLFunctions)
CRANBERRY_FORWARD_Q(QOpenGLExtraFunctions)
CRANBERRY_FORWARD_Q(QOpenGLFramebufferObject)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders multiple objects in a batch and applies post-processing effects.
///
/// \class SpriteBatch
/// \author Nicolas Kogler
/// \date July 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT SpriteBatch final : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(SpriteBatch)
    CRANBERRY_DECLARE_DTOR(SpriteBatch)
    CRANBERRY_DISABLE_COPY(SpriteBatch)
    CRANBERRY_DISABLE_MOVE(SpriteBatch)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the background color of the batch.
    ///
    /// \returns the background color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QColor& backgroundColor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the geometry of the batch.
    ///
    /// \returns the geometry.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QRectF& geometry() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the effect of the batch.
    ///
    /// \returns the effect.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Effect effect() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the background color of this batch. Setting this to
    /// QColor() [isValid() returns false] will result in the BG color
    /// of the render target.
    ///
    /// \param color Background color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBackgroundColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the geometry of the batch. If a null rectangle is passed
    /// [i.e. QRectF()], the batch spans over the entire screen.
    ///
    /// \param rc The geometry (position and size) of the batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setGeometry(const QRectF& rc);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new sprite batch based on an existing fbo. Takes ownership
    /// of the Qt framebuffer object, you must not free it yourself.
    ///
    /// \param fbo Existing fbo with attached texture.
    /// \param renderTarget Target to render batch on.
    /// \param takeOwnership If true, deletes the given FBO automatically.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(
            QOpenGLFramebufferObject* fbo,
            Window* renderTarget = nullptr,
            bool takeOwnership = false
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Adds a new renderable object and puts it at the end of the list.
    ///
    /// \note "End of the list" implies top-most.
    /// \param object Object to add to the batch.
    /// \returns false if that object already exists.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool addObject(RenderBase* object);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts a new renderable object at the specified layer. Zero is the
    /// down-most layer. If layer exceeds the size of the list, it will be
    /// simply appended.
    ///
    /// \param layer Layer to insert object to.
    /// \param object Object to insert into the batch.
    /// \returns false if that object already exists.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool insertObject(int layer, RenderBase* object);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given object from the sprite batch.
    ///
    /// \param object Object to remove from the batch.
    /// \returns false if that object does not exist.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool removeObject(RenderBase* object);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is null.
    ///
    /// \returns true if this batch is null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the necessary OpenGL objects for the sprite batch.
    ///
    /// \param renderTarget Target to render batch on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(Window* renderTarget = nullptr) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all the OpenGL objects for the sprite batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates all objects in the sprite batch (e.g. transformations).
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders all objects in the batch and applies the effects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the root model item for this instance. Use this method only if
    /// the debug overlay is about to be shown.
    ///
    /// \returns the root model item of this instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem* rootModelItem() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void updateProperties() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    bool createInternal(Window*);
    bool createData();
    bool createFboRbo();
    bool createBuffers();
    void recreateFboRbo();
    void updateVertices();
    bool writeData();
    bool writeBuffers();
    bool writeFramebuffer();
    bool writeTexture();
    bool writeRenderbuffer();
    void destroyFboRbo();
    void destroyBuffers();
    void setupBatch();
    void setupFrame();
    void renderBatch();
    void renderFrame();
    void releaseFrame();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLExtraFunctions*    egl;
    QOpenGLFramebufferObject* m_fbo;
    TreeModelItem*            m_rootModelItem;
    Effect                    m_effect;
    priv::QuadVertices        m_vertices;
    QList<RenderBase*>        m_objects;
    QRectF                    m_geometry;
    QColor                    m_backColor;
    uint                      m_frameBuffer;
    uint                      m_msFrameBuffer;
    uint                      m_renderBuffer;
    uint                      m_vertexArray;
    uint                      m_vertexBuffer;
    uint                      m_indexBuffer;
    uint                      m_frameTexture;
    uint                      m_msFrameTexture;
    bool                      m_isEmbedded;
    bool                      m_takeOwnership;
};


////////////////////////////////////////////////////////////////////////////////
/// \class SpriteBatch
/// \ingroup Graphics
///
/// The SpriteBatch class should be mainly used to apply post-processing
/// effects (even transitions) to a large group of objects.
///
/// \code
/// // Applies the sepia effect to multiple objects.
/// m_batch = new SpriteBatch;
/// m_batch->create(this);
/// m_batch->addObject(m_animation);
/// m_batch->addObject(m_text);
/// ...
/// m_batch->setEffect(EffectSepia);
///
/// ...
///
/// m_batch->render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

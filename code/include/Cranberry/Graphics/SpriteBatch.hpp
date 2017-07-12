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
#ifndef CRANBERRY_SPRITEBATCH_HPP
#define CRANBERRY_SPRITEBATCH_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>
#include <Cranberry/System/GameTime.hpp>

// Qt headers
#include <QList>


// Forward declarations
CRANBERRY_FORWARD_C(Window)
CRANBERRY_FORWARD_C(OpenGLShader)
CRANBERRY_FORWARD_Q(QOpenGLFunctions)
CRANBERRY_FORWARD_Q(QOpenGLExtraFunctions)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders multiple objects in a batch and applies post-processing effects.
///
/// \class SpriteBatch
/// \author Nicolas Kogler
/// \date July 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT SpriteBatch
    : public IRenderable
    , public ITransformable
{
public:

    CRANBERRY_DISABLE_COPY(SpriteBatch)
    CRANBERRY_DISABLE_MOVE(SpriteBatch)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of SpriteBatch and sets all members to their
    /// logical default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    SpriteBatch();

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys this sprite batch. Last resort for releasing all OpenGL
    /// resources before the context will eventually be destroyed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~SpriteBatch();


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
    /// \param renderTarget Target to render batch on (on entire surface).
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
    /// Adds a new renderable object and puts it at the end of the list.
    ///
    /// \note "End of the list" implies top-most.
    /// \param object Object to add to the batch.
    /// \returns false if that object already exists.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool addObject(IRenderable* object);

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
    bool insertObject(int layer, IRenderable* object);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given object from the sprite batch.
    ///
    /// \param object Object to remove from the batch.
    /// \returns false if that object does not exist.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool removeObject(IRenderable* object);

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


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void updateVertices();
    bool createBuffers();
    bool writeBuffers();
    void setupBatch();
    void setupFrame();
    void renderBatch();
    void renderFrame();
    void releaseFrame();
    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLExtraFunctions* egl;
    Effect                 m_effect;
    priv::QuadVertices     m_vertices;
    QList<IRenderable*>    m_objects;
    QString                m_name;
    qreal                  m_opacity;
    uint                   m_frameBuffer;
    uint                   m_renderBuffer;
    uint                   m_vertexArray;
    uint                   m_vertexBuffer;
    uint                   m_indexBuffer;
    uint                   m_frameTexture;
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

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
#include <Cranberry/OpenGL/OpenGLVertex.hpp>
#include <Cranberry/System/GameTime.hpp>

// Qt headers
#include <QList>


// Forward declarations
CRANBERRY_FORWARD_C(Window)
CRANBERRY_FORWARD_C(IRenderable)
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
    bool isNull() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the necessary OpenGL objects for the sprite batch.
    ///
    /// \param renderTarget Target to render batch on (on entire surface).
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all the OpenGL objects for the sprite batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy();

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
    /// Specifies the shader program that will apply the post-processing effects
    /// to the batch. When writing a GLSL shader for SpriteBatch, please follow
    /// the conventions described in the documentation. Will NOT take ownership
    /// of \p program. You need to free it yourself.
    ///
    /// \param program Program that applies the effects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setShaderProgram(OpenGLShader* program);

    ////////////////////////////////////////////////////////////////////////////
    /// Updates all objects in the sprite batch (e.g. transformations).
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time);

    ////////////////////////////////////////////////////////////////////////////
    /// Renders all objects in the batch and applies the effects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render();


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the opacity of the entire scene.
    ///
    /// \returns the opacity of the batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float opacity() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the opacity of the entire scene.
    ///
    /// \param opacity New opacity of the batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOpacity(float opacity);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this batch.
    ///
    /// \returns the name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Setting a name is not required, but recommended, since it can be easily
    /// retrieved by name and it simplifies debugging by being able to track
    /// down the error source.
    ///
    /// \param name New name of this batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);


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
    auto buildMatrix() -> QMatrix4x4;

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions*      gl;
    QOpenGLExtraFunctions* egl;
    Window*                m_renderTarget;
    OpenGLShader*          m_shader;
    OpenGLShader*          m_defaultShader;
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
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

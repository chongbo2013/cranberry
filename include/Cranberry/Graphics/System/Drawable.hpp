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
#ifndef CRANBERRY_DRAWABLE_HPP
#define CRANBERRY_DRAWABLE_HPP


// Cranberry headers
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/Graphics/System/GraphicsEnums.hpp>
#include <Cranberry/Window/Window.hpp>

// Forward declarations
class QOpenGLBuffer;
class QOpenGLContext;
class QOpenGLFunctions;
class QOpenGLShaderProgram;


CRANBERRY_BEGIN_NAMESPACE


///
/// Contains basic functionality for all
/// renderable objects within cranberry.
///
/// \class Drawable
/// \author Nicolas Kogler
/// \date February 11, 2017
///
class CRANBERRY_EXPORT Drawable : public QObject
{
public:

    CRANBERRY_DISABLE_MOVE(Drawable)

    ///
    /// Initializes a new instance of Drawable.
    ///
    Drawable();

    ///
    /// Copies one Drawable instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Drawable instance.
    ///
    Drawable(const Drawable& other);

    ///
    /// Copies one Drawable instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Drawable instance.
    ///
    Drawable& operator=(const Drawable& other);

    ///
    /// Destroys this Drawable instance.
    ///
    virtual ~Drawable();


    ///
    /// Retrieves the current render target.
    ///
    /// \returns the associated window.
    ///
    Window* renderTarget() const;

    ///
    /// Retrieves the blending mode.
    ///
    /// \returns the blending mode.
    ///
    BlendModes blendMode() const;

    ///
    /// Retrieves the custom shader program
    /// for this renderable cranberry object.
    ///
    /// \returns the custom shader program.
    ///
    QOpenGLShaderProgram* customProgram() const;


    ///
    /// Specifies the render target for this object.
    /// If a nullptr is passed, the active window as
    /// returned by Window::activeWindow() will be
    /// used as render target.
    ///
    /// \param target The window to render this object on.
    ///
    void setRenderTarget(Window* target);

    ///
    /// Specifies one or more blend modes in which
    /// the blend color should be rendered on top of
    /// the object. Multiple blend modes are possible
    /// by combining the cran::BlendMode flags via OR.
    ///
    /// \param mode New blend mode(s).
    ///
    void setBlendMode(BlendModes mode);

    ///
    /// Specifies the custom shader program for this object.
    /// In order to use the default one, pass a null pointer.
    /// You do not have to free the memory you allocate by
    /// creating an instance of the given program.
    ///
    /// \param program Custom shader program.
    /// \remarks The documentation provides a list of in/out
    ///          and location values your shader has to provide.
    ///
    void setCustomProgram(QOpenGLShaderProgram* program);


    // Functions to override.
    virtual void destroy() = 0;
    virtual void update(const GameTime& time) = 0;
    virtual void render() = 0;


protected:

    ///
    /// Allocates one vertex buffer object.
    ///
    /// \param target Render target to use. Can not be nullptr.
    /// \param size The size (in bytes) to allocate.
    /// \returns false if no render target specified.
    ///
    bool createInternal(Window* target, int32_t size);

    ///
    /// Destroys all OpenGL resources.
    ///
    void destroyInternal();

    ///
    /// Returns the private vertex buffer.
    ///
    /// \returns the private VBO.
    ///
    QOpenGLBuffer* vertexBuffer() const;


    // Protected members
    QOpenGLFunctions* gl;


signals:

    ///
    /// Is emitted once the object has been fully
    /// created and is ready to be rendered.
    ///
    void created();

    ///
    /// Is emitted once the object is destroyed.
    ///
    void destroyed();


private:

    // Members
    uint32_t*               m_refCount;
    Window*                 m_renderTarget;
    QOpenGLBuffer*          m_vertexBuffer;
    QOpenGLShaderProgram*   m_customProgram;
    BlendModes              m_blendModes;
    bool                    m_isInit;

    // Metadata
    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_DRAWABLE_HPP

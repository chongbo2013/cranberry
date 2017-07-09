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
#ifndef CRANBERRY_IRENDERABLE_HPP
#define CRANBERRY_IRENDERABLE_HPP


// Cranberry headers
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/System/Emitters/RenderableEmitter.hpp>

// Qt headers
#include <QObject>


// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLFunctions)
CRANBERRY_FORWARD_C(Window)
CRANBERRY_FORWARD_C(OpenGLShader)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Provides an interface for all renderable objects in cranberry.
///
/// \class IRenderable
/// \author Nicolas Kogler
/// \date June 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT IRenderable
{
public:

    CRANBERRY_DISABLE_COPY(IRenderable)
    CRANBERRY_DISABLE_MOVE(IRenderable)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new IRenderable instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    IRenderable();

    ////////////////////////////////////////////////////////////////////////////
    /// Destructs this IRenderable instance and calls IRenderable::destroy() as
    /// last resort to free OpenGL-related objects.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~IRenderable();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this IRenderable object is valid. If a devired class
    /// needs more conditions for the object to be valid, this method should
    /// be overridden.
    ///
    /// \returns true if this object is not valid.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool isNull() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates this object by specifying the render target and retrieving the
    /// OpenGL functions from its context.
    ///
    /// \param renderTarget Window to render object on.
    /// \returns false if initialization failed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the custom shader object, if any. It is recommended to
    /// override this method in order to implement custom clean-up.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void destroy();

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the object. Must implement.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the object. Must implement.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void render() = 0;


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a pointer to the render target.
    ///
    /// \returns a pointer to the render target.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Window* renderTarget() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a pointer to the shader program. If no custom shader program
    /// was specified using IRenderable::setShaderProgram, the default shader
    /// program for this object will be returned.
    ///
    /// \returns a pointer to the currently used shader program.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader* shaderProgram() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this object.
    ///
    /// \returns the name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the shader program. If the given program is nullptr, the
    /// default shader program will be used instead. Will NOT take ownership
    /// of \p program. You need to free it yourself.
    ///
    /// \param program Program to use. Nullptr yields default shader program.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setShaderProgram(OpenGLShader* program);

    ////////////////////////////////////////////////////////////////////////////
    /// Setting a name is not required, but recommended, since it can be easily
    /// retrieved by name and it simplifies debugging by being able to track
    /// down the error source.
    ///
    /// \param name New name of this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the emitter for this object.
    ///
    /// \returns a pointer to the emitter.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RenderableEmitter* renderableEmitter();


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the string representation of this object.
    ///
    /// \returns the string representation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    operator QString() const;


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the default shader program. Only available to sub-classes.
    ///
    /// \param program Program to use. Must not be nullptr.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setDefaultShaderProgram(OpenGLShader* program);

    ////////////////////////////////////////////////////////////////////////////
    // Protected members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* gl;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Window*           m_renderTarget;   ///< Target to render object on
    OpenGLShader*     m_defaultProgram; ///< Default shader program
    OpenGLShader*     m_customProgram;  ///< Custom shader program
    QString           m_name;           ///< Name of the object
    RenderableEmitter m_emitter;        ///< Emits signals for this class
};


////////////////////////////////////////////////////////////////////////////////
/// \class IRenderable
/// \ingroup Graphics
///
/// The IRenderable class is the base for all graphic objects in cranberry. It
/// provides the render target, the shader program and the OpenGL functions.
///
/// \code
/// class MyObject : public IRenderable
/// {
/// public:
///     bool isNull() const override;
///     bool create() override;
///     void destroy() override;
///     void update(const GameTime& time) override;
///     void render() override;
///     ...
/// };
///
/// {
///     renderTarget()->makeCurrent();
///     ...
///     gl->glDrawArrays(...);
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

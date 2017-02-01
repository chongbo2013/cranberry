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


#ifndef CRANBERRY_CBDRAWABLE_HPP
#define CRANBERRY_CBDRAWABLE_HPP


// Cranberry headers
#include <Cranberry/System/cbGameTime.hpp>
#include <Cranberry/System/Enums/cbGraphicsEnums.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

// Standard headers
#include <vector>


CRANBERRY_BEGIN_NAMESPACE


// Forward declarations
class cbWindow;


///
/// Defines basic functionality for all
/// renderable objects in cranberry.
///
/// \class cbDrawable
/// \author Nicolas Kogler
/// \date January 30th, 2017
///
class CRANBERRY_EXPORT cbDrawable : protected QOpenGLFunctions
{
public:

    ///
    /// Constructor
    /// Constructs an instance of cbDrawable and specifies
    /// the target on which this image should be rendered.
    /// Equivalent to cbImage::setRenderTarget().
    ///
    /// \param renderTarget The window to render this image on.
    ///        If a null pointer is passed, the currently active
    ///        render target will be taken instead.
    ///
    cbDrawable(cbWindow* renderTarget = nullptr);

    ///
    /// Copy constructor
    /// Uses implicit resource sharing, so copying an
    /// instance of cbDrawable will not copy the buffer.
    ///
    /// \param other The other cbDrawable instance.
    ///
    cbDrawable(const cbDrawable& other);

    ///
    /// Move constructor
    ///
    /// \param other The other cbDrawable instance.
    ///
    cbDrawable(cbDrawable&& other) = delete;

    ///
    /// Assignment operator
    /// Uses implicit resource sharing, so copying an
    /// instance of cbDrawable will not copy the buffer.
    ///
    /// \param other The other cbDrawable instance.
    ///
    cbDrawable& operator=(const cbDrawable& other);

    ///
    /// Destructor
    ///
    ~cbDrawable();


    ///
    /// Retrieves the current render target.
    ///
    /// \returns the associated window.
    ///
    cbWindow* renderTarget() const;

    ///
    /// Retrieves the custom shader program.
    /// A custom shader program can be set using
    /// cbDrawable::setShaderProgram().
    ///
    QOpenGLShaderProgram* shaderProgram() const;

    ///
    /// Retrieves the mode in which the blend color
    /// is rendered on top of the object.
    ///
    /// \returns the color blend mode.
    ///
    BlendModes blendMode() const;


    ///
    /// Specifies a render target for this image.
    /// If no access to a cbWindow instance is provided,
    /// use cbWindow::current() to retrieve the cbWindow
    /// that currently has the input focus.
    ///
    /// \param renderTarget The window to render this image on.
    ///        If a null pointer is passed, the currently active
    ///        render target will be taken instead.
    ///
    void setRenderTarget(cbWindow* renderTarget);

    ///
    /// Specifies a custom shader program for this object.
    /// If one wants to specify the default cranberry shader
    /// again, a nullptr should be passed to this function.
    /// Note that you do not have to free the memory you
    /// have allocated for the QOpenGLShaderProgram.
    ///
    /// \param program Custom shader program. Must be designed
    ///        in a way that the parent cranberry object can
    ///        interact with it easily. Look at the object
    ///        documentation for more information.
    ///
    /// \code
    /// auto* program = new QOpenGLShaderProgram;
    /// // do something with program.
    /// sprite.setShaderProgram(program);
    /// // do not delete program, it will be managed.
    /// \endcode
    ///
    void setShaderProgram(QOpenGLShaderProgram* program);

    ///
    /// Specifies the mode in which the blend color
    /// should be rendered on top of the object. Multiple
    /// blend modes are possible through combining the
    /// cran::BlendMode flags via logical OR.
    ///
    /// \param mode New blend modes.
    ///
    void setBlendMode(BlendModes mode);


    ///
    /// Destroys the underlying vertex buffer.
    ///
    virtual void destroy();

    ///
    /// Performs updates for the renderable object.
    /// The delta time of the given cbGameTime may
    /// be used to perform movements or similar.
    ///
    /// \param time Contains time since start and delta time.
    ///
    virtual void update(const cbGameTime& time) = 0;

    ///
    /// Performs rendering for the renderable object.
    ///
    virtual void render() = 0;


protected:

    ///
    /// Allocates one vertex buffer object.
    ///
    /// \returns false if no render target.
    ///
    bool create();

    ///
    /// Retrieves the private vertex buffer.
    ///
    /// \returns the private vertex buffer.
    ///
    QOpenGLBuffer* vertexBuffer() const;

    ///
    /// Retrieves the private list of vertices.
    ///
    /// \returns the private list of vertices.
    ///
    std::vector<float>* vertices() const;


private:

    // Members
    uint32_t*               m_refCount;
    cbWindow*               m_renderTarget;
    QOpenGLBuffer*          m_vertexBuffer;
    QOpenGLShaderProgram*   m_customProgram;
    BlendModes              m_blendMode;
    std::vector<float>*     m_vertices;
    bool                    m_isValid;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBDRAWABLE_HPP

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
#ifndef CRANBERRY_OPENGLSHADER_HPP
#define CRANBERRY_OPENGLSHADER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLShaderProgram)
CRANBERRY_FORWARD_Q(QOpenGLShader)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class is able to load vertex and fragment shaders from a file. For
/// each of them, it applies the shader abstraction between OpenGL and GLES.
/// This is the only class of module OpenGL that is also accessible from
/// outside of cranberry, since the programmers are given the possibility to
/// override the default shader program for graphics objects.
///
/// \class OpenGLShader
/// \author Nicolas Kogler
/// \date May 26, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_OPENGL_EXPORT OpenGLShader
{
public:

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of OpenGLShader. Creates a new reference
    /// counter for this instance and initializes it with one.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader();

    ////////////////////////////////////////////////////////////////////////////
    /// Copies the given instance into another instance while incrementing the
    /// reference counter by one.
    ///
    /// \param other Shader to shallow copy.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader(const OpenGLShader& other);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the given instance into another instance while incrementing the
    /// reference counter by one.
    ///
    /// \param other Shader to shallow copy.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader(OpenGLShader&& other);

    ////////////////////////////////////////////////////////////////////////////
    /// Destructs this instance and decrements the reference counter by one.
    /// If the reference counter now equals zero, all the underlying data is
    /// being destroyed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~OpenGLShader();

    ////////////////////////////////////////////////////////////////////////////
    /// Copies the given instance into another instance while incrementing the
    /// reference counter by one.
    ///
    /// \param other Shader to shallow copy.
    /// \returns a reference to the assigned object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader& operator =(const OpenGLShader& other);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the given instance into another instance while incrementing the
    /// reference counter by one.
    ///
    /// \param other Shader to shallow copy.
    /// \returns a reference to the assigned object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader& operator =(OpenGLShader&& other);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a pointer to the shader program.
    ///
    /// \returns the shader program.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram* program();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a pointer to the vertex shader. If no vertex shader has
    /// been specified before, a nullptr is returned.
    ///
    /// \returns the vertex shader, if any.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLShader* vertexShader();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a pointer to the fragment shader. If no vertex shader has
    /// been specified before, a nullptr is returned.
    ///
    /// \returns the fragment shader, if any.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLShader* fragmentShader();


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the code of the vertex shader. If you want to load the
    /// source code from a file, use setVertexShaderFromFile().
    ///
    /// \param code The raw source code of the shader.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setVertexShaderFromCode(const QString& code);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the code of the vertex shader, loaded from a file. If you
    /// want to load the source code from a string, use
    /// setVertexShaderFromCode().
    ///
    /// \param file A path to a file containing source code.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setVertexShaderFromFile(const QString& file);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the code of the fragment shader. If you want to load the
    /// source code from a file, use setFragmentShaderFromFile().
    ///
    /// \param code The raw source code of the shader.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setFragmentShaderFromCode(const QString& code);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the code of the fragment shader, loaded from a file. If you
    /// want to load the source code from a string, use
    /// setFragmentShaderFromCode().
    ///
    /// \param file A path to a file containing source code.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool setFragmentShaderFromFile(const QString& file);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    QString getFileContents(const QString& path);
    bool loadShaderPrivate(int type, QString code);

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram* m_program;  ///< The program containing all shaders
    QOpenGLShader*        m_vertex;   ///< The vertex shader.
    QOpenGLShader*        m_fragment; ///< The fragment shader.
    uint*                 m_refCount; ///< Counts the "copies" of this instance
};


////////////////////////////////////////////////////////////////////////////////
/// \class OpenGLShader
/// \ingroup OpenGL
///
/// The OpenGLShader class is used to flawlessly abstract shaders of types
/// OpenGL 3+ and GLES 2+. The example below illustrates its use:
///
/// \code
/// OpenGLShader shader;
/// shader.setVertexShaderFromFile(":/shaders/myshader.vert");
/// shader.setFragmentShaderFromFile(":/shaders/myshader.frag");
/// object.setShaderProgram(shader);
/// \endcode
///
/// The program will automatically be linked as soon as both shader types have
/// been specified successfully.
///
/// How to design the shaders? It is safe to use GLSL code that is
/// equivalent to shader code from OpenGL version 3.0 to 3.3:
///
/// \code
/// // %0 is important - replaced with either 330 or 300 es!
/// #version %0
///
/// layout(location = 0) in vec3 in_xyz;
/// ...
///
/// void main()
/// {
///     ...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

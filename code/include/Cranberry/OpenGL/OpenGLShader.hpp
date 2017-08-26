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
#ifndef CRANBERRY_OPENGL_OPENGLSHADER_HPP
#define CRANBERRY_OPENGL_OPENGLSHADER_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>

// Qt headers
#include <QRectF>
#include <QString>

// Forward declarations
CRANBERRY_FORWARD_Q(QMatrix4x4)
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

    CRANBERRY_DECLARE_CTOR(OpenGLShader)
    CRANBERRY_DECLARE_DTOR(OpenGLShader)
    CRANBERRY_DISABLE_COPY(OpenGLShader)
    CRANBERRY_DISABLE_MOVE(OpenGLShader)

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
    /// Determines whether the program has been linked yet.
    ///
    /// \returns true if linked already.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isLinked() const;

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

    ////////////////////////////////////////////////////////////////////////////
    /// Binds this shader program. Note: In order to optimize, as little calls
    /// as possible to glUseProgram(id) are issued. This can be achieved using
    /// a boolean member variable. Therefore make sure to _always_ call
    /// OpenGLShader::release() after you are done!
    ///
    ////////////////////////////////////////////////////////////////////////////
    void bind();

    ////////////////////////////////////////////////////////////////////////////
    /// Releases this shader program.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void release();

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the sampler for uniform u_tex. This function will fail if the
    /// shader program has not yet been linked. Will call bind() automatically.
    ///
    /// \param samplerId GL_TEXTURE0, GL_TEXTURE1, ...
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSampler(int samplerId);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the model-view-projection matrix for uniform u_mvp. This
    /// function will fail if the shader program has not yet been linked.
    /// Will call bind() automatically.
    ///
    /// \param mvp Pointer to the MVP matrix.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMvpMatrix(QMatrix4x4* mvp);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the opacity for the uniform u_opac. This function will fail
    /// if the shader program has not yet been linked. Will call bind()
    /// automatically.
    ///
    /// \param opacity The opacity ranging from 0 to 1.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOpacity(float opacity);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend mode for uniform u_mode. This function will fail
    /// if the shader program has not yet been linked. Will call bind()
    /// automatically.
    ///
    /// \param blendMode The blend modes to use.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendMode(BlendModes blendMode);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect for uniform u_effect. This function will fail if
    /// the shader program has not yet been linked. Will call bind()
    /// automatically.
    ///
    /// \param effect The effect to use.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the size for uniform u_winSize. This function will fail if
    /// the shader program has not yet been linked. Will call bind()
    /// automatically.
    ///
    /// \param size Size of the render target.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setWindowSize(const QSize& size);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the source rectangle for uniform u_sourceRect. This function
    /// will fail if the shader program has not yet been linked. Will call
    /// bind() automatically.
    ///
    /// \param rect Source rectangle.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSourceRect(const QRectF& rect);

    ////////////////////////////////////////////////////////////////////////////
    /// Gets the layout location of the uniform called \p name.
    ///
    /// \param name Name of the uniform.
    /// \returns >0 if successful and -1 if failed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int uniformLocation(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Integer value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, int value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Unsigned integer value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, uint value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Boolean value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, bool value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Floating-point number value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, float value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Point (vec2) value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, const QPointF& value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Size (vec2) value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, const QSizeF& value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param value Pointer to matrix (mat4) value to store.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, QMatrix4x4* value);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param x First component of vec2.
    /// \param y Second component of vec2.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, float x, float y);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param x First component of vec3.
    /// \param y Second component of vec3.
    /// \param z Third component of vec3.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, float x, float y, float z);

    ////////////////////////////////////////////////////////////////////////////
    /// Sets the uniform value at \p location to \p value. To optimize these
    /// calls, a call to OpenGLShader::uniformLocation() should be issued. The
    /// return value of that method can then be used for this method.
    ///
    /// \param location Layout location of the uniform.
    /// \param x First component of vec4.
    /// \param y Second component of vec4.
    /// \param z Third component of vec4.
    /// \param w Fourth component of vec4.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setUniformValue(int location, float x, float y, float z, float w);


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// This virtual method is called after the program has been successfully
    /// linked. Use it to load custom uniform locations.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void afterLink() { }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    QString getFileContents(const QString& path);
    bool loadShaderPrivate(int type, QString code);
    bool link();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram* m_program;   ///< The program containing all shaders
    QOpenGLShader*        m_vertex;    ///< The vertex shader
    QOpenGLShader*        m_fragment;  ///< The fragment shader
    QString               m_vertName;  ///< Vertex shader name
    QString               m_fragName;  ///< Fragment shader name
    uint*                 m_refCount;  ///< Counts the "copies" of this instance
    bool                  m_isBound;   ///< Is currently bound?
    int                   m_locTex;    ///< Uniform location of u_tex
    int                   m_locMvp;    ///< Uniform location of u_mvp
    int                   m_locOpac;   ///< Uniform location of u_opac
    int                   m_locMode;   ///< Uniform location of u_mode
    int                   m_locEffect; ///< Uniform location of u_effect
    int                   m_locSize;   ///< Uniform location of u_winSize
    int                   m_locRect;   ///< Uniform location of u_sourceRect
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
/// You can also set common cranberry uniform values and misc uniform values
/// without needing the dependency to QtOpenGL:
///
/// \code
/// shader->bind(); // optimize by binding/releasing ourselves
/// shader->setSampler(GL_TEXTURE0); // Will modify u_tex in cranberry shaders
/// int ownUniform = shader->uniformLocation("u_myUniform");
/// shader->setUniformValue(ownUniform, 1, 1, 1, 1);
/// shader->release();
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

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


// Cranberry headers
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>

// Qt headers
#include <QFile>
#include <QFileInfo>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

// Macroes
#define ensure_bound(x) { bool b = m_isBound; if(!b) bind(); x; if (!b) release(); }
#define set_uniform(loc, val) { ensure_bound(m_program->setUniformValue(loc, val)); }

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "OpenGLShader: Shader file %0 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "OpenGLShader: Program could not be created.")
CRANBERRY_CONST_VAR(QString, e_03, "OpenGLShader: Shader could not be added. ")
CRANBERRY_CONST_VAR(QString, e_04, "OpenGLShader (%0,%1): Program could not be linked. Log: ")
CRANBERRY_CONST_VAR(QString, e_05, "OpenGLShader::setSampler(): ID is not valid.")
CRANBERRY_CONST_VAR(QString, e_06, "OpenGLShader (%0,%1): OpenGL could not find "
                                   "the following attributes:\n\"%2\"\nIgnore "
                                   "this message if these attributes are unused "
                                   "in your shader program.")


CRANBERRY_USING_NAMESPACE


OpenGLShader::OpenGLShader()
    : m_program(nullptr)
    , m_vertex(nullptr)
    , m_fragment(nullptr)
    , m_refCount(new uint(1))
    , m_isBound(false)
    , m_locTex(-1)
    , m_locMvp(-1)
    , m_locOpac(-1)
    , m_locMode(-1)
    , m_locEffect(-1)
    , m_locSize(-1)
{
}


OpenGLShader::~OpenGLShader()
{
    if (m_program != nullptr)
    {
        m_program->removeAllShaders();
    }

    delete m_program;
}


QOpenGLShaderProgram* OpenGLShader::program()
{
    return m_program;
}


QOpenGLShader* OpenGLShader::vertexShader()
{
    return m_vertex;
}


QOpenGLShader* OpenGLShader::fragmentShader()
{
    return m_fragment;
}


bool OpenGLShader::setVertexShaderFromCode(const QString& code)
{
    return loadShaderPrivate(QOpenGLShader::Vertex, code);
}


bool OpenGLShader::setVertexShaderFromFile(const QString& file)
{
    m_vertName = QFileInfo(file).baseName();

    return loadShaderPrivate(QOpenGLShader::Vertex, getFileContents(file));
}


bool OpenGLShader::setFragmentShaderFromCode(const QString& code)
{
    return loadShaderPrivate(QOpenGLShader::Fragment, code);
}


bool OpenGLShader::setFragmentShaderFromFile(const QString& file)
{
    m_fragName = QFileInfo(file).baseName();

    return loadShaderPrivate(QOpenGLShader::Fragment, getFileContents(file));
}


void OpenGLShader::bind()
{
    if (m_isBound)
    {
        return;
    }

    glDebug(m_program->bind());

    m_isBound = true;
}


void OpenGLShader::release()
{
    m_isBound = false;
}


void OpenGLShader::setSampler(int samplerId)
{
    if (samplerId < GL_TEXTURE0 || samplerId > GL_TEXTURE31)
    {
        cranError(e_05);
        return;
    }

    int mapped = samplerId - GL_TEXTURE0;
    ensure_bound(glDebug(m_program->setUniformValue(m_locTex, mapped)));
}


void OpenGLShader::setMvpMatrix(QMatrix4x4* mvp)
{
    ensure_bound(glDebug(m_program->setUniformValue(m_locMvp, *mvp)));
}


void OpenGLShader::setOpacity(float opacity)
{
    ensure_bound(glDebug(m_program->setUniformValue(m_locOpac, opacity)));
}


void OpenGLShader::setBlendMode(BlendModes blendMode)
{
    ensure_bound(glDebug(m_program->setUniformValue(m_locMode, (int) blendMode)));
}


void OpenGLShader::setEffect(Effect effect)
{
    ensure_bound(glDebug(m_program->setUniformValue(m_locEffect, (int) effect)));
}


void OpenGLShader::setWindowSize(const QSize& size)
{
    ensure_bound(glDebug(m_program->setUniformValue(m_locSize, QSizeF(size))));
}


void OpenGLShader::setUniformValue(int location, int value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, uint value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, bool value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, float value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, const QPointF& value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, const QSizeF& value)
{
    set_uniform(location, value);
}


void OpenGLShader::setUniformValue(int location, QMatrix4x4* value)
{
    set_uniform(location, *value);
}


void OpenGLShader::setUniformValue(int location, float x, float y)
{
    QVector2D vec2(x, y);
    set_uniform(location, vec2);
}


void OpenGLShader::setUniformValue(int location, float x, float y, float z)
{
    QVector3D vec3(x, y, z);
    set_uniform(location, vec3);
}


void OpenGLShader::setUniformValue(int location, float x, float y, float z, float w)
{
    QVector4D vec4(x, y, z, w);
    set_uniform(location, vec4);
}


QString OpenGLShader::getFileContents(const QString& path)
{
    QString code;
    QFile file(path);
    QTextStream stream(&file);

    if (!file.open(QFile::ReadOnly))
    {
        cranError(e_01.arg(path));
    }
    else
    {
        code = stream.readAll();
    }

    return code;
}


bool OpenGLShader::loadShaderPrivate(int type, QString code)
{
    auto shaderType = static_cast<QOpenGLShader::ShaderTypeBit>(type);

    // Creates a new program if not already existing.
    if (m_program == nullptr)
    {
        m_program = new QOpenGLShaderProgram;
        if (!m_program->create())
        {
            return cranError(e_02);
        }
    }

    // Specifies the version string, depending on the API.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGLES)
    {
        code.replace("%0", "300 es");
    }
    else
    {
        code.replace("%0", "330");
    }

    // Adds the shader to the program and links it if required.
    if (!m_program->addShaderFromSourceCode(shaderType, code))
    {
        return cranError(e_03 + m_program->log());
    }

    if (shaderType == QOpenGLShader::Vertex)
    {
        m_vertex = m_program->shaders().last();
    }
    else
    {
        m_fragment = m_program->shaders().last();
    }

    if (m_vertex != nullptr && m_fragment != nullptr)
    {
        if (!link())
        {
            return false;
        }

        glDebug(afterLink());
    }

    return true;
}


bool OpenGLShader::link()
{
    if (!m_program->link() || !m_program->bind())
    {
        return cranError(e_04.arg(m_vertName, m_fragName) + m_program->log());
    }

    // Loads common cranberry uniforms.
    glDebug(m_locTex = m_program->uniformLocation("u_tex"));
    glDebug(m_locMvp = m_program->uniformLocation("u_mvp"));
    glDebug(m_locOpac = m_program->uniformLocation("u_opac"));
    glDebug(m_locMode = m_program->uniformLocation("u_mode"));
    glDebug(m_locEffect = m_program->uniformLocation("u_effect"));
    glDebug(m_locSize = m_program->uniformLocation("u_winSize"));

    // Stores all invalid attributes in a list.
    QStringList attr;
    if (m_locTex == -1) attr << "u_tex";
    if (m_locMvp == -1) attr << "u_mvp";
    if (m_locOpac == -1) attr << "u_opac";
    if (m_locMode == -1) attr << "u_mode";
    if (m_locSize == -1) attr << "u_winSize";
    if (m_locEffect == -1) attr << "u_effect";

    if (!attr.isEmpty())
    {
        return cranWarning(e_06.arg(m_vertName, m_fragName, attr.join(", ")));
    }

    return true;
}

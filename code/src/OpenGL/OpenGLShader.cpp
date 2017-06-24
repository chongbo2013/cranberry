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


// Cranberry headers
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>

// Qt headers
#include <QFile>
#include <QOpenGLContext>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


CRANBERRY_USING_NAMESPACE


OpenGLShader::OpenGLShader()
    : m_program(nullptr)
    , m_vertex(nullptr)
    , m_fragment(nullptr)
    , m_refCount(new uint(1))
{
}


OpenGLShader::OpenGLShader(const OpenGLShader& other)
    : m_program(other.m_program)
    , m_vertex(other.m_vertex)
    , m_fragment(other.m_fragment)
    , m_refCount(other.m_refCount)
{
    *m_refCount += 1;
}


OpenGLShader::OpenGLShader(OpenGLShader&& other)
    : m_program(std::move(other.m_program))
    , m_vertex(std::move(other.m_vertex))
    , m_fragment(std::move(other.m_fragment))
    , m_refCount(std::move(other.m_refCount))
{
    *m_refCount += 1;
}


OpenGLShader::~OpenGLShader()
{
    auto& ref = *m_refCount;
    if (--ref == 0)
    {
        if (m_program != nullptr)
        {
            m_program->removeAllShaders();
        }

        delete m_program;
    }
}


OpenGLShader& OpenGLShader::operator =(const OpenGLShader& other)
{
    m_program = other.m_program;
    m_vertex = other.m_vertex;
    m_fragment = other.m_fragment;
    m_refCount = other.m_refCount;

    *m_refCount += 1;

    return *this;
}


OpenGLShader& OpenGLShader::operator =(OpenGLShader&& other)
{
    m_program = std::move(other.m_program);
    m_vertex = std::move(other.m_vertex);
    m_fragment = std::move(other.m_fragment);
    m_refCount = std::move(other.m_refCount);

    *m_refCount += 1;

    return *this;
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
    return loadShaderPrivate(QOpenGLShader::Vertex, getFileContents(file));
}


bool OpenGLShader::setFragmentShaderFromCode(const QString& code)
{
    return loadShaderPrivate(QOpenGLShader::Fragment, code);
}


bool OpenGLShader::setFragmentShaderFromFile(const QString& file)
{
    return loadShaderPrivate(QOpenGLShader::Fragment, getFileContents(file));
}


QString OpenGLShader::getFileContents(const QString& path)
{
    QString code;
    QFile file(path);
    QTextStream stream(&file);

    if (!file.open(QFile::ReadOnly))
    {
        cranError("OpenGLShader: Shader file does not exist.");
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
            return cranError("OpenGLShader: Program could not be created.");
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
        QString msg("OpenGLShader: Shader could not be added. ");
        return cranError(msg.append(m_program->log()));
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
        if (!m_program->link())
        {
            QString msg("OpenGLShader: Program could not be linked. ");
            return cranError(msg.append(m_program->log()));
        }
    }

    return true;
}

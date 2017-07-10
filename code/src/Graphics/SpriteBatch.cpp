﻿////////////////////////////////////////////////////////////////////////////////
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
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - The given render target is invalid.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Frame buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Render buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Vertex array could not be created.")
CRANBERRY_CONST_VAR(QString, e_05, "%0 [%1] - Vertex buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_06, "%0 [%1] - Index buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_07, "%0 [%1] - Texture could not be created.")
CRANBERRY_CONST_VAR(QString, e_08, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_VAR(QString, e_09, "%0 [%1] - Frame buffer not complete.")
CRANBERRY_CONST_ARR(uint, 6, c_ibo, (0, 1, 2, 2, 3, 0))


SpriteBatch::SpriteBatch()
    : gl(nullptr)
    , m_renderTarget(nullptr)
    , m_shader(nullptr)
    , m_effect(EffectNone)
    , m_name("<no name>")
    , m_opacity(1)
    , m_frameBuffer(0)
    , m_renderBuffer(0)
    , m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
    , m_frameTexture(0)
{
    m_vertices.at(0).uv(0, 0);
    m_vertices.at(1).uv(1, 0);
    m_vertices.at(2).uv(1, 1);
    m_vertices.at(3).uv(0, 1);
    m_vertices.at(0).rgba(1, 1, 1, 1);
    m_vertices.at(1).rgba(1, 1, 1, 1);
    m_vertices.at(2).rgba(1, 1, 1, 1);
    m_vertices.at(3).rgba(1, 1, 1, 1);
}


SpriteBatch::~SpriteBatch()
{
    destroy();
}


bool SpriteBatch::isNull() const
{
    return m_renderTarget == nullptr ||
           m_frameBuffer  == 0       ||
           m_renderBuffer == 0       ||
           m_vertexArray  == 0       ||
           m_vertexBuffer == 0       ||
           m_indexBuffer  == 0       ||
           m_frameTexture == 0;
}


bool SpriteBatch::create(Window* renderTarget)
{
    if (renderTarget == nullptr)
    {
        if ((renderTarget = Window::activeWindow()) == nullptr)
        {
            return cranError(ERRARG(e_01));
        }
    }

    gl = renderTarget->context()->functions();
    egl = renderTarget->context()->extraFunctions();
    m_renderTarget = renderTarget;
    m_defaultShader = OpenGLDefaultShaders::get("cb.glsl.texture");

    return createBuffers() && writeBuffers();
}


void SpriteBatch::updateVertices()
{
    m_vertices.at(0).xyz(0, 0, 0);
    m_vertices.at(1).xyz(m_renderTarget->width(), 0, 0);
    m_vertices.at(2).xyz(m_renderTarget->width(), m_renderTarget->height(), 0);
    m_vertices.at(3).xyz(0, m_renderTarget->height(), 0);
}


bool SpriteBatch::createBuffers()
{
    // Generates all required OpenGL objects.
    glDebug(gl->glGenFramebuffers(1, &m_frameBuffer));
    if (m_frameBuffer == 0)
    {
        return cranError(ERRARG(e_02));
    }

    glDebug(gl->glGenRenderbuffers(1, &m_renderBuffer));
    if (m_renderBuffer == 0)
    {
        return cranError(ERRARG(e_03));
    }

    glDebug(egl->glGenVertexArrays(1, &m_vertexArray));
    if (m_vertexArray == 0)
    {
        return cranError(ERRARG(e_04));
    }

    glDebug(gl->glGenBuffers(1, &m_vertexBuffer));
    if (m_vertexBuffer == 0)
    {
        return cranError(ERRARG(e_05));
    }

    glDebug(gl->glGenBuffers(1, &m_indexBuffer));
    if (m_indexBuffer == 0)
    {
        return cranError(ERRARG(e_06));
    }

    glDebug(gl->glGenTextures(1, &m_frameTexture));
    if (m_frameTexture == 0)
    {
        return cranError(ERRARG(e_07));
    }

    return true;
}


bool SpriteBatch::writeBuffers()
{
    // Binds the VAO and the VBO to it.
    glDebug(egl->glBindVertexArray(m_vertexArray));
    glDebug(egl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
    glDebug(egl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
    glDebug(gl->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer));

    updateVertices();

    // Allocates a texture as big as the screen & enable smoothing.
    glDebug(gl->glBindTexture(GL_TEXTURE_2D, m_frameTexture));
    glDebug(gl->glTexImage2D(
                GL_TEXTURE_2D,
                GL_ZERO,
                GL_RGBA8,
                m_renderTarget->width(),
                m_renderTarget->height(),
                GL_ZERO,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                NULL
                ));

    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glDebug(gl->glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_frameTexture,
                GL_ZERO
                ));

    // Attaches the render buffer to the frame buffer.
    glDebug(gl->glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer));
    glDebug(gl->glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH24_STENCIL8,
                m_renderTarget->width(),
                m_renderTarget->height()
                ));

    glDebug(gl->glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                m_renderBuffer
                ));

    // Check for frame buffer completeness.
    uint status;
    glDebug(status = gl->glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        return cranError(ERRARG(e_09));
    }

    // Allocates static data for the vertex buffer.
    glDebug(gl->glBufferData(
                GL_ARRAY_BUFFER,
                priv::TextureVertex::size() * 4,
                m_vertices.data(),
                GL_STATIC_DRAW
                ));

    glDebug(gl->glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                sizeof(uint) * 6,
                c_ibo.data(),
                GL_STATIC_DRAW
                ));

    // Bind this render target's VAO back again.
    glDebug(egl->glBindVertexArray(m_renderTarget->vao()));
    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, GL_ZERO));
    glDebug(egl->glBindRenderbuffer(GL_RENDERBUFFER, GL_ZERO));

    return true;
}


void SpriteBatch::destroy()
{
    if (m_frameBuffer != 0)
    {
        glDebug(gl->glDeleteFramebuffers(1, &m_frameBuffer));
    }
    if (m_renderBuffer != 0)
    {
        glDebug(gl->glDeleteRenderbuffers(1, &m_renderBuffer));
    }
    if (m_vertexArray != 0)
    {
        glDebug(egl->glDeleteVertexArrays(1, &m_vertexArray));
    }
    if (m_vertexBuffer != 0)
    {
        glDebug(gl->glDeleteBuffers(1, &m_vertexBuffer));
    }
    if (m_indexBuffer != 0)
    {
        glDebug(gl->glDeleteBuffers(1, &m_indexBuffer));
    }
    if (m_frameTexture != 0)
    {
        glDebug(gl->glDeleteTextures(1, &m_frameTexture));
    }
}


bool SpriteBatch::addObject(IRenderable* object)
{
    if (m_objects.contains(object)) return false;

    m_objects.append(object);
    return true;
}


bool SpriteBatch::insertObject(int layer, IRenderable* object)
{
    if (m_objects.contains(object)) return false;
    if (layer < 0 || layer >= m_objects.size()) m_objects.append(object);
    else m_objects.insert(layer, object);

    return true;
}


bool SpriteBatch::removeObject(IRenderable* object)
{
    return m_objects.removeOne(object);
}


void SpriteBatch::setShaderProgram(OpenGLShader* program)
{
    m_shader = program;
}


void SpriteBatch::update(const GameTime& time)
{
    for (IRenderable* obj : m_objects)
    {
        obj->update(time);
    }
}


void SpriteBatch::render()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_08));
        return;
    }
    else if (QOpenGLContext::currentContext() != m_renderTarget->context())
    {
        m_renderTarget->makeCurrent();
    }

    setupBatch();
    renderBatch();
    setupFrame();
    renderFrame();
}


void SpriteBatch::setupBatch()
{
    glDebug(gl->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer));
    glDebug(gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


void SpriteBatch::renderBatch()
{
    for (IRenderable* obj : m_objects)
    {
        obj->render();
    }
}


void SpriteBatch::setupFrame()
{
    OpenGLShader* shader = (m_shader != nullptr) ? m_shader : m_defaultShader;
    QOpenGLShaderProgram* program = shader->program();

    // Bind default framebuffer.
    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glDebug(egl->glBindVertexArray(m_vertexArray));
    glDebug(egl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
    glDebug(egl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
    glDebug(egl->glUseProgram(program->programId()));
    glDebug(egl->glActiveTexture(GL_TEXTURE0));
    glDebug(egl->glBindTexture(GL_TEXTURE_2D, m_frameTexture));

    // Modify the states of the program.
    glDebug(program->setUniformValue("u_tex", GL_ZERO));
    glDebug(program->setUniformValue("u_mvp", buildMatrix()));
    glDebug(program->setUniformValue("u_opac", (float) m_opacity));
    glDebug(program->setUniformValue("u_effect", (uint) m_effect));
    glDebug(program->setUniformValue("u_mode", GL_ZERO));

    // Enables the vertex attributes.
    glDebug(program->enableAttributeArray(priv::TextureVertex::xyzAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::uvAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::rgbaAttrib()));

    // Modifies the vertex attributes.
    glDebug(gl->glVertexAttribPointer(
                priv::TextureVertex::xyzAttrib(),
                priv::TextureVertex::xyzLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::TextureVertex::size(),
                priv::TextureVertex::xyzOffset()
                ));

    glDebug(gl->glVertexAttribPointer(
                priv::TextureVertex::uvAttrib(),
                priv::TextureVertex::uvLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::TextureVertex::size(),
                priv::TextureVertex::uvOffset()
                ));

    glDebug(gl->glVertexAttribPointer(
                priv::TextureVertex::rgbaAttrib(),
                priv::TextureVertex::rgbaLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::TextureVertex::size(),
                priv::TextureVertex::rgbaOffset()
                ));
}


void SpriteBatch::renderFrame()
{
    // Renders the elements specified by the index buffer.
    glDebug(gl->glDrawElements(
                GL_TRIANGLES,
                QUADS_TO_TRIANGLES(4),
                GL_UNSIGNED_INT,
                priv::TextureVertex::xyzOffset()
                ));
}

void SpriteBatch::releaseFrame()
{
    glDebug(egl->glBindTexture(GL_TEXTURE_2D, 0));
    glDebug(egl->glUseProgram(0));
    glDebug(egl->glBindVertexArray(m_renderTarget->vao()));
}


QMatrix4x4 SpriteBatch::buildMatrix()
{
    float fw = static_cast<float>(m_renderTarget->width());
    float fh = static_cast<float>(m_renderTarget->height());

    QMatrix4x4 proj;
    proj.ortho(0.f, fw, 0.f, fh, -1.f, 1.f);

    return proj;
}


float SpriteBatch::opacity() const
{
    return m_opacity;
}

void SpriteBatch::setOpacity(float opacity)
{
    m_opacity = opacity;
}


void SpriteBatch::setEffect(Effect effect)
{
    m_effect = effect;
}


const QString& SpriteBatch::name() const
{
    return m_name;
}


void SpriteBatch::setName(const QString& name)
{
    m_name = name;
}


SpriteBatch::operator QString() const
{
    QString s;

    s.append("--------------------------------------------------------\n");
    s.append("--- Cranberry object\n");
    s.append("-- SpriteBatch\n");

    if (isNull())
    {
        s.append("Object is null.");
        return s;
    }

    s.append(QString("Name: ") + m_name + "\n");
    s.append(QString("Is valid: ") + ((isNull())? "false\n" : "true\n"));
    s.append(QString("Render target: " + m_renderTarget->settings().title() + "\n"));
    s.append(QString("Objects bound: ") + QString::number(m_objects.size()) + "\n");

    for (int i = 0; i < m_objects.size(); i++)
    {
        IRenderable* obj = m_objects.at(i);
        QString sl = QString("Layer %0: ").arg(QString::number(i));

        s.append(sl + obj->name() + "\n");
    }

    return s.append("\n");
}
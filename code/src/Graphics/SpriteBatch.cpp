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
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Frame buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Render buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Vertex array could not be created.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Vertex buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_05, "%0 [%1] - Index buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_06, "%0 [%1] - Texture could not be created.")
CRANBERRY_CONST_VAR(QString, e_07, "%0 [%1] - Frame buffer not complete.")
CRANBERRY_CONST_ARR(uint, 6, c_ibo, 0, 1, 2, 2, 3, 0)


SpriteBatch::SpriteBatch()
    : egl(nullptr)
    , m_fbo(nullptr)
    , m_effect(EffectNone)
    , m_name("<no name>")
    , m_backColor(Qt::transparent)
    , m_frameBuffer(0)
    , m_msFrameBuffer(0)
    , m_renderBuffer(0)
    , m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
    , m_frameTexture(0)
    , m_msFrameTexture(0)
{
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
    return RenderBase::isNull() ||
           m_frameBuffer  == 0   ||
           m_msFrameBuffer == 0  ||
           m_renderBuffer == 0   ||
           m_vertexArray  == 0   ||
           m_vertexBuffer == 0   ||
           m_indexBuffer  == 0   ||
           m_frameTexture == 0   ||
           m_msFrameTexture == 0;
}


bool SpriteBatch::create(Window* rt)
{
    return create(nullptr, rt);
}


bool SpriteBatch::create(QOpenGLFramebufferObject* fbo, Window* rt)
{
    m_fbo = fbo;

    return createInternal(rt) && createData() && writeData();
}


void SpriteBatch::destroy()
{
    destroyFboRbo();
    destroyBuffers();
}


bool SpriteBatch::addObject(RenderBase* object)
{
    if (m_objects.contains(object)) return false;

    m_objects.append(object);
    return true;
}


bool SpriteBatch::insertObject(int layer, RenderBase* object)
{
    if (m_objects.contains(object)) return false;
    if (layer < 0 || layer >= m_objects.size()) m_objects.append(object);
    else m_objects.insert(layer, object);

    return true;
}


bool SpriteBatch::removeObject(RenderBase* object)
{
    return m_objects.removeOne(object);
}


void SpriteBatch::update(const GameTime& time)
{
    updateTransform(time);

    for (RenderBase* obj : m_objects)
    {
        obj->update(time);
    }
}


void SpriteBatch::render()
{
    if (!prepareRendering()) return;
    if (!m_objects.isEmpty())
    {
        setupBatch();
        renderBatch();
    }

    setupFrame();
    renderFrame();
    releaseFrame();
}


const QColor& SpriteBatch::backgroundColor() const
{
    return m_backColor;
}


const QRectF& SpriteBatch::geometry() const
{
    return m_geometry;
}


void SpriteBatch::setBackgroundColor(const QColor& color)
{
    m_backColor = color;
}


void SpriteBatch::setGeometry(const QRectF& rc)
{
    if (rc == m_geometry) return;

    m_geometry = rc;
    setPosition(rc.x(), rc.y());
    recreateFboRbo();
}


Effect SpriteBatch::effect() const
{
    return m_effect;
}


void SpriteBatch::setEffect(Effect effect)
{
    m_effect = effect;
}



bool SpriteBatch::createInternal(Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    egl = renderTarget()->context()->extraFunctions();
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return true;
}


bool SpriteBatch::createData()
{
    return createFboRbo() && createBuffers();
}


bool SpriteBatch::createFboRbo()
{
    glDebug(gl->glGenFramebuffers(1, &m_frameBuffer));
    if (m_frameBuffer == 0)
    {
        return cranError(ERRARG(e_01));
    }

    glDebug(gl->glGenTextures(1, &m_frameTexture));
    if (m_frameTexture == 0)
    {
        return cranError(ERRARG(e_06));
    }

    glDebug(gl->glGenRenderbuffers(1, &m_renderBuffer));
    if (m_renderBuffer == 0)
    {
        return cranError(ERRARG(e_02));
    }

    if (m_fbo == nullptr)
    {
        glDebug(gl->glGenFramebuffers(1, &m_msFrameBuffer));
        if (m_msFrameBuffer == 0)
        {
            return cranError(ERRARG(e_01));
        }

        glDebug(gl->glGenTextures(1, &m_msFrameTexture));
        if (m_msFrameTexture == 0)
        {
            return cranError(ERRARG(e_06));
        }
    }
    else
    {
        m_msFrameBuffer = m_fbo->handle();
        m_msFrameTexture = m_fbo->texture();
    }

    return true;
}


bool SpriteBatch::createBuffers()
{
    glDebug(egl->glGenVertexArrays(1, &m_vertexArray));
    if (m_vertexArray == 0)
    {
        return cranError(ERRARG(e_03));
    }

    glDebug(gl->glGenBuffers(1, &m_vertexBuffer));
    if (m_vertexBuffer == 0)
    {
        return cranError(ERRARG(e_04));
    }

    glDebug(gl->glGenBuffers(1, &m_indexBuffer));
    if (m_indexBuffer == 0)
    {
        return cranError(ERRARG(e_05));
    }

    return true;
}


void SpriteBatch::recreateFboRbo()
{
    destroyFboRbo();
    updateVertices();
    createFboRbo();
    writeTexture();
    writeRenderbuffer();
    writeFramebuffer();
}


void SpriteBatch::updateVertices()
{
    // If null, takes the entire screen.
    auto cp = m_geometry;
    if (cp.isNull())
    {
        cp.setWidth(renderTarget()->width());
        cp.setHeight(renderTarget()->height());
    }

    if (m_fbo != nullptr)
    {
        cp.setWidth(m_fbo->width());
        cp.setHeight(m_fbo->height());
    }

    float uvX = cp.x() / cp.width();
    float uvY = cp.y() / cp.height();
    float uvW = uvX + 1;
    float uvH = uvY + 1;

    // Specifies the vertex locations.
    m_vertices.at(0).xyz(0,          0,           0);
    m_vertices.at(1).xyz(cp.width(), 0,           0);
    m_vertices.at(2).xyz(cp.width(), cp.height(), 0);
    m_vertices.at(3).xyz(0,          cp.height(), 0);

    // Specifies the texture coordinates.
    m_vertices.at(0).uv(uvX, uvH);
    m_vertices.at(1).uv(uvW, uvH);
    m_vertices.at(2).uv(uvW, uvY);
    m_vertices.at(3).uv(uvX, uvY);

    setSize(cp.width(), cp.height());
    setOrigin(width() / 2, height() / 2);
}


bool SpriteBatch::writeData()
{
    updateVertices();

    return writeBuffers() &&
           writeTexture() &&
           writeRenderbuffer() &&
           writeFramebuffer();
}


bool SpriteBatch::writeBuffers()
{
    // Binds the VAO and the VBO/IBO to it.
    glDebug(egl->glBindVertexArray(m_vertexArray));
    glDebug(egl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
    glDebug(egl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));

    // Allocates static data for the vertex and index buffer.
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
    glDebug(egl->glBindVertexArray(renderTarget()->vao()));
    return true;
}


bool SpriteBatch::writeFramebuffer()
{
    // Assigns the underlying texture.
    glDebug(gl->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer));
    glDebug(gl->glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_frameTexture,
                GL_ZERO
                ));

    // Check for frame buffer completeness.
    uint status;
    glDebug(status = gl->glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        return cranError(ERRARG(e_07));
    }

    // Assigns the underlying multisampled texture and the multisampled rbo.
    if (m_fbo == nullptr)
    {
        glDebug(gl->glBindFramebuffer(GL_FRAMEBUFFER, m_msFrameBuffer));
        glDebug(gl->glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D_MULTISAMPLE,
                    m_msFrameTexture,
                    GL_ZERO));
        glDebug(gl->glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_STENCIL_ATTACHMENT,
                    GL_RENDERBUFFER,
                    m_renderBuffer
                    ));

        // Check for frame buffer completeness.
        glDebug(status = gl->glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            cranError(ERRARG(e_07));
        }
    }

    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, GL_ZERO));
    return status == GL_FRAMEBUFFER_COMPLETE;
}



bool SpriteBatch::writeTexture()
{
    // Allocates a texture, enable smoothing.
    glDebug(gl->glBindTexture(GL_TEXTURE_2D, m_frameTexture));
    glDebug(gl->glTexImage2D(
                GL_TEXTURE_2D,
                GL_ZERO,
                GL_RGBA8,
                width(),
                height(),
                GL_ZERO,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                NULL
                ));

    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    glDebug(gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glDebug(gl->glBindTexture(GL_TEXTURE_2D, GL_ZERO));

    // Allocates a multisampled texture.
    if (m_fbo == nullptr)
    {
        glDebug(egl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msFrameTexture));
        glDebug(egl->glTexStorage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    4,
                    GL_RGBA,
                    width(),
                    height(),
                    GL_TRUE
                    ));

        glDebug(egl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_ZERO));
    }

    return true;
}


bool SpriteBatch::writeRenderbuffer()
{
    glDebug(egl->glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer));
    glDebug(egl->glRenderbufferStorageMultisample(
                GL_RENDERBUFFER,
                4,
                GL_DEPTH24_STENCIL8,
                width(),
                height()
                ));

    glDebug(egl->glBindRenderbuffer(GL_RENDERBUFFER, GL_ZERO));
    return true;
}


void SpriteBatch::destroyFboRbo()
{
    // If using a fbo allocated by Qt, do not delete it manually.
    if (m_fbo == nullptr)
    {
        if (m_msFrameBuffer != 0)
        {
            glDebug(gl->glDeleteFramebuffers(1, &m_msFrameBuffer));
            m_msFrameBuffer = 0;
        }
        if (m_msFrameTexture != 0)
        {
            glDebug(gl->glDeleteTextures(1, &m_msFrameTexture));
            m_msFrameTexture = 0;
        }
    }
    else
    {
        delete m_fbo;
        m_fbo = nullptr;
    }

    if (m_frameBuffer != 0)
    {
        glDebug(gl->glDeleteFramebuffers(1, &m_frameBuffer));
        m_frameBuffer = 0;
    }

    if (m_frameTexture != 0)
    {
        glDebug(gl->glDeleteTextures(1, &m_frameTexture));
        m_frameTexture = 0;
    }

    if (m_renderBuffer != 0)
    {
        glDebug(gl->glDeleteRenderbuffers(1, &m_renderBuffer));
        m_renderBuffer = 0;
    }
}


void SpriteBatch::destroyBuffers()
{
    if (m_vertexArray != 0)
    {
        glDebug(egl->glDeleteVertexArrays(1, &m_vertexArray));
        m_vertexArray = 0;
    }

    if (m_vertexBuffer != 0)
    {
        glDebug(gl->glDeleteBuffers(1, &m_vertexBuffer));
        m_vertexBuffer = 0;
    }

    if (m_indexBuffer != 0)
    {
        glDebug(gl->glDeleteBuffers(1, &m_indexBuffer));
        m_indexBuffer = 0;
    }
}


void SpriteBatch::setupBatch()
{
    // Change clear color for frame buffer.
    if (m_backColor.isValid())
    {
        glDebug(gl->glClearColor(
                    m_backColor.redF(),
                    m_backColor.greenF(),
                    m_backColor.blueF(),
                    m_backColor.alphaF()
                    ));
    }

    glDebug(gl->glBindFramebuffer(GL_FRAMEBUFFER, m_msFrameBuffer));
    glDebug(gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Revert clear color back to default.
    if (m_backColor.isValid())
    {
        const auto& cc = renderTarget()->settings().clearColor();
        glDebug(gl->glClearColor(
                    cc.redF(),
                    cc.greenF(),
                    cc.blueF(),
                    cc.alphaF()
                    ));
    }
}


void SpriteBatch::renderBatch()
{
    for (RenderBase* obj : m_objects)
    {
        obj->setOffscreenRenderer(m_msFrameBuffer);
        obj->render();
        obj->setOffscreenRenderer(0);
    }
}


void SpriteBatch::setupFrame()
{
    QOpenGLShaderProgram* program = shaderProgram()->program();

    // Blit MSAA fbo to normal fbo.
    glDebug(egl->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFrameBuffer));
    glDebug(egl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer));
    glDebug(egl->glBlitFramebuffer(
                0, 0, width(), height(),
                0, 0, width(), height(),
                GL_COLOR_BUFFER_BIT,
                GL_NEAREST
                ));

    // Bind default framebuffer.
    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, offscreenRenderer()));
    glDebug(egl->glBindVertexArray(m_vertexArray));
    glDebug(egl->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer));
    glDebug(egl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
    glDebug(egl->glUseProgram(program->programId()));
    glDebug(egl->glActiveTexture(GL_TEXTURE0));
    glDebug(egl->glBindTexture(GL_TEXTURE_2D, m_frameTexture));

    // Modify the states of the program.
    glDebug(program->setUniformValue("u_tex", GL_ZERO));
    glDebug(program->setUniformValue("u_mvp", matrix(this)));
    glDebug(program->setUniformValue("u_opac", opacity()));
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
    // Bind previous OpenGL objects.
    glDebug(egl->glBindTexture(GL_TEXTURE_2D, 0));
    glDebug(egl->glUseProgram(0));
    glDebug(egl->glBindVertexArray(renderTarget()->vao()));
    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, offscreenRenderer()));
}


SpriteBatch::operator QString() const
{
    QString s;

    s.append(RenderBase::operator QString());
    s.append("-- SpriteBatch\n");
    s.append(QString("Objects bound: ") + QString::number(m_objects.size()) + "\n");

    for (int i = 0; i < m_objects.size(); i++)
    {
        RenderBase* obj = m_objects.at(i);
        QString sl = QString("Layer %0: ").arg(QString::number(i));

        s.append(sl + obj->name() + "\n");
    }

    return s.append("\n");
}

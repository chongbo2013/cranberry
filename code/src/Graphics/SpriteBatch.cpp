﻿////////////////////////////////////////////////////////////////////////////////
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


// Cranberry headers
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
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
    : RenderBase()
    , egl(nullptr)
    , m_fbo(nullptr)
    , m_effect(EffectNone)
    , m_backColor(Qt::transparent)
    , m_frameBuffer(0)
    , m_msFrameBuffer(0)
    , m_renderBuffer(0)
    , m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
    , m_frameTexture(0)
    , m_msFrameTexture(0)
    , m_isEmbedded(false)
    , m_takeOwnership(false)
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


const QColor& SpriteBatch::backgroundColor() const
{
    return m_backColor;
}


const QRectF& SpriteBatch::geometry() const
{
    return m_geometry;
}


Effect SpriteBatch::effect() const
{
    return m_effect;
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


void SpriteBatch::setEffect(Effect effect)
{
    m_effect = effect;
}


bool SpriteBatch::create(
    QOpenGLFramebufferObject* fbo,
    Window* rt,
    bool takeOwnership
    )
{
    m_fbo = fbo;
    m_takeOwnership = takeOwnership;

    return createInternal(rt) && createData() && writeData();
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


void SpriteBatch::destroy()
{
    destroyFboRbo();
    destroyBuffers();

    RenderBase::destroy();
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


TreeModelItem* SpriteBatch::rootModelItem()
{
    return m_rootModelItem;
}


void SpriteBatch::createProperties(TreeModel* model)
{
    auto cp = m_geometry;
    if (cp.isNull())
    {
        cp.setWidth(renderTarget()->width());
        cp.setHeight(renderTarget()->height());
    }

    TreeModelItem* tmiEffe = new TreeModelItem("Effect", getEffectString(m_effect));
    TreeModelItem* tmiBack = new TreeModelItem("Backcolor", m_backColor);
    TreeModelItem* tmiObjs = new TreeModelItem("Objects");
    TreeModelItem* tmiGeom = new TreeModelItem("Geometry");
    TreeModelItem* tmiGeoX = new TreeModelItem("x", cp.x());
    TreeModelItem* tmiGeoY = new TreeModelItem("y", cp.y());
    TreeModelItem* tmiGeoW = new TreeModelItem("w", cp.width());
    TreeModelItem* tmiGeoH = new TreeModelItem("h", cp.height());
    TreeModelItem* tmiOpGL = new TreeModelItem("OpenGL");
    TreeModelItem* tmiSFbo = new TreeModelItem("Single-sampled frame buffer", m_frameBuffer);
    TreeModelItem* tmiMFbo = new TreeModelItem("Multi-sampled frame buffer", m_msFrameBuffer);
    TreeModelItem* tmiORbo = new TreeModelItem("Render buffer", m_renderBuffer);
    TreeModelItem* tmiOVao = new TreeModelItem("Vertex array", m_vertexArray);
    TreeModelItem* tmiOVbo = new TreeModelItem("Vertex buffer", m_vertexBuffer);
    TreeModelItem* tmiOIbo = new TreeModelItem("Index buffer", m_indexBuffer);
    TreeModelItem* tmiSTex = new TreeModelItem("Single-sampled texture", m_frameTexture);
    TreeModelItem* tmiMTex = new TreeModelItem("Multi-sampled texture", m_msFrameTexture);

    m_rootModelItem = new TreeModelItem("SpriteBatch");
    m_rootModelItem->appendChild(tmiEffe);
    m_rootModelItem->appendChild(tmiBack);
    m_rootModelItem->appendChild(tmiObjs);
    m_rootModelItem->appendChild(tmiGeom);
    m_rootModelItem->appendChild(tmiOpGL);

    tmiGeom->appendChild(tmiGeoX);
    tmiGeom->appendChild(tmiGeoY);
    tmiGeom->appendChild(tmiGeoW);
    tmiGeom->appendChild(tmiGeoH);
    tmiOpGL->appendChild(tmiSFbo);
    tmiOpGL->appendChild(tmiMFbo);
    tmiOpGL->appendChild(tmiORbo);
    tmiOpGL->appendChild(tmiOVao);
    tmiOpGL->appendChild(tmiOVbo);
    tmiOpGL->appendChild(tmiOIbo);
    tmiOpGL->appendChild(tmiSTex);
    tmiOpGL->appendChild(tmiMTex);

    Q_FOREACH (RenderBase* rb, m_objects)
    {
        tmiObjs->appendChild(new TreeModelItem("Name", rb->name()));
    }

    // The sprite batch is also used by other classes; give the possibility to
    // simply append the root item to another item.
    if (model != nullptr)
    {
        model->addItem(m_rootModelItem);
        RenderBase::createProperties(model);
    }
    else
    {
        m_isEmbedded = true;
    }
}


void SpriteBatch::updateProperties()
{
    // Update child objects if count changed.
    if (m_rootModelItem->childAt(2)->childCount() != m_objects.size())
    {
        m_rootModelItem->childAt(2)->removeAllChildren();

        Q_FOREACH (RenderBase* rb, m_objects)
        {
            m_rootModelItem->childAt(2)->appendChild(new TreeModelItem("Name", rb->name()));
        }
    }

    auto cp = m_geometry;
    if (cp.isNull())
    {
        cp.setWidth(renderTarget()->width());
        cp.setHeight(renderTarget()->height());
    }

    m_rootModelItem->childAt(0)->setValue(getEffectString(m_effect));
    m_rootModelItem->childAt(1)->setValue(m_backColor);
    m_rootModelItem->childAt(3)->childAt(0)->setValue(cp.x());
    m_rootModelItem->childAt(3)->childAt(1)->setValue(cp.y());
    m_rootModelItem->childAt(3)->childAt(2)->setValue(cp.width());
    m_rootModelItem->childAt(3)->childAt(3)->setValue(cp.height());
    m_rootModelItem->childAt(4)->childAt(0)->setValue(m_frameBuffer);
    m_rootModelItem->childAt(4)->childAt(1)->setValue(m_msFrameBuffer);
    m_rootModelItem->childAt(4)->childAt(2)->setValue(m_renderBuffer);
    m_rootModelItem->childAt(4)->childAt(3)->setValue(m_vertexArray);
    m_rootModelItem->childAt(4)->childAt(4)->setValue(m_vertexBuffer);
    m_rootModelItem->childAt(4)->childAt(5)->setValue(m_indexBuffer);
    m_rootModelItem->childAt(4)->childAt(6)->setValue(m_frameTexture);
    m_rootModelItem->childAt(4)->childAt(7)->setValue(m_msFrameTexture);

    if (!m_isEmbedded)
    {
        RenderBase::updateProperties();
    }
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
    else if (m_takeOwnership)
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
    OpenGLShader* program = shaderProgram();

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

    // Activates unit 0 and binds our target texture to it.
    glDebug(egl->glActiveTexture(GL_TEXTURE0));
    glDebug(egl->glBindTexture(GL_TEXTURE_2D, m_frameTexture));

    // Modify the states of the program.
    glDebug(program->bind());
    glDebug(program->setSampler(GL_TEXTURE0));
    glDebug(program->setMvpMatrix(matrix(this)));
    glDebug(program->setOpacity(opacity()));
    glDebug(program->setEffect(m_effect));
    glDebug(program->setBlendMode(BlendNone));

    // Enables the vertex attributes.
    glDebug(gl->glEnableVertexAttribArray(priv::TextureVertex::xyzAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::TextureVertex::uvAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::TextureVertex::rgbaAttrib()));

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
    glDebug(egl->glBindVertexArray(renderTarget()->vao()));
    glDebug(egl->glBindFramebuffer(GL_FRAMEBUFFER, offscreenRenderer()));
    shaderProgram()->release();
}

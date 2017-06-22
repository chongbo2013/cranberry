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
#include <Cranberry/Graphics/Base/ITexture.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Vertex buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Index buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Texture creation failed.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_ARR(uint, 6, c_ibo, (0, 1, 2, 2, 3, 0))


ITexture::ITexture()
    : IRenderable()
    , ITransformable()
    , m_blendMode(BlendNone)
    , m_texture(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_update(false)
{
}


ITexture::~ITexture()
{
    destroy();
}


bool ITexture::isNull() const
{
    return IRenderable::isNull()       ||
           m_texture == nullptr        ||
           m_vertexBuffer == nullptr   ||
           m_indexBuffer == nullptr    ||
          !m_vertexBuffer->isCreated() ||
          !m_indexBuffer->isCreated();
}


bool ITexture::create(const QImage& img, Window* renderTarget)
{
    if (!IRenderable::create(renderTarget)) return false;
    if (!createBuffers()) return false;
    if (!createTexture(img)) return false;

    initializeData();

    return true;
}


bool ITexture::create(QOpenGLTexture* img, Window* renderTarget)
{
    if (!IRenderable::create(renderTarget)) return false;
    if (!createBuffers()) return false;

    m_texture = img;
    initializeData();

    return true;
}


bool ITexture::createBuffers()
{
    // Attempts to create the buffer holding the vertex data.
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_vertexBuffer->create() || !m_vertexBuffer->bind())
    {
        return cranError(ERRARG(e_01));
    }

    // Attempts to create the index buffer to optimize quad rendering.
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if (!m_indexBuffer->create() || !m_indexBuffer->bind())
    {
        return cranError(ERRARG(e_02));
    }

    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vertexBuffer->allocate(priv::TextureVertex::size() * 4);
    m_vertexBuffer->release();

    m_indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer->allocate(c_ibo.data(), sizeof(uint) * 6);
    m_indexBuffer->release();

    return true;
}


bool ITexture::createTexture(const QImage& img)
{
    m_texture = new QOpenGLTexture(img);
    if (!m_texture->isCreated())
    {
        return cranError(ERRARG(e_03));
    }

    return true;
}


void ITexture::initializeData()
{
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    setSize((float) m_texture->width(), (float) m_texture->height());
    setSourceRectangle({ 0.f, 0.f, width(), height() });
    setOrigin({ width() / 2.f, height() / 2.f });
    setBlendColor(QColor(Qt::white));
}


void ITexture::destroy()
{
    IRenderable::destroy();

    if (m_vertexBuffer != nullptr)
    {
        m_vertexBuffer->destroy();
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }

    if (m_indexBuffer != nullptr)
    {
        m_indexBuffer->destroy();
        delete m_indexBuffer;
        m_indexBuffer = nullptr;
    }

    if (m_texture != nullptr)
    {
        m_texture->destroy();
        delete m_texture;
        m_texture = nullptr;
    }
}


void ITexture::update(const GameTime& time)
{
    updateTransform(time);
}


void ITexture::render()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_04));
        return;
    }
    else if (QOpenGLContext::currentContext() != renderTarget()->context())
    {
        renderTarget()->makeCurrent();
    }

    bindObjects();
    writeVertices();
    modifyProgram();
    modifyAttribs();
    drawElements();
    releaseObjects();
}


QMatrix4x4 ITexture::buildMatrix()
{
    float fw = static_cast<float>(renderTarget()->width());
    float fh = static_cast<float>(renderTarget()->height());

    // TODO: Not cheap - maybe optimize this?!
    QMatrix4x4 proj, tran, rot, scale, orig, norig;
    proj.ortho(0.f, fw, fh, 0.f, -1.f, 1.f);
    tran.translate(x(), y());
    rot.rotate(angleX(), 1.f, 0.f, 0.f);
    rot.rotate(angleY(), 0.f, 1.f, 0.f);
    rot.rotate(angleZ(), 0.f, 0.f, 1.f);
    scale.scale(scaleX(), scaleY());
    orig.translate(origin());
    norig.translate(origin() * -1);

    return proj * tran * orig * rot * norig * scale;
}


void ITexture::bindObjects()
{
    // Binds the texture to unit 0.
    // TODO: Actually support blending between two textures!
    glDebug(gl->glActiveTexture(GL_TEXTURE0));
    glDebug(m_texture->bind());

    glDebug(m_vertexBuffer->bind());
    glDebug(m_indexBuffer->bind());
    glDebug(shaderProgram()->program()->bind());
}


void ITexture::releaseObjects()
{
    glDebug(m_texture->release());
    glDebug(m_vertexBuffer->release());
    glDebug(m_indexBuffer->release());
    glDebug(shaderProgram()->program()->release());
}


void ITexture::writeVertices()
{
    // Only update data if update occured.
    if (m_update)
    {
        glDebug(m_vertexBuffer->write(
                0, m_vertices.data(),
                priv::TextureVertex::size() * 4)
                );

        m_update = false;
    }
}


void ITexture::modifyProgram()
{
    QOpenGLShaderProgram* program = shaderProgram()->program();

    glDebug(program->setUniformValue("u_tex", GL_ZERO));
    glDebug(program->setUniformValue("u_mvp", buildMatrix()));
    glDebug(program->setUniformValue("u_opac", opacity()));
    glDebug(program->setUniformValue("u_mode", (uint) m_blendMode));

    glDebug(program->enableAttributeArray(priv::TextureVertex::xyzAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::uvAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::rgbaAttrib()));
}


void ITexture::modifyAttribs()
{
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


void ITexture::drawElements()
{
    // Renders the elements specified by the index buffer.
    glDebug(gl->glDrawElements(
                GL_TRIANGLES,
                QUADS_TO_TRIANGLES(4),
                GL_UNSIGNED_INT,
                priv::TextureVertex::xyzOffset()
                ));
}


void ITexture::setSourceRectangle(const QRectF& rc)
{
    float texW = width();
    float texH = height();
    float dstW = rc.width();
    float dstH = rc.height();
    float uvcX = rc.x() / texW;
    float uvcY = rc.y() / texH;
    float uvcW = uvcX + (dstW / texW);
    float uvcH = uvcY + (dstH / texH);

    m_vertices.at(0).xyz(0.f,  0.f,  0.f);
    m_vertices.at(1).xyz(dstW, 0.f,  0.f);
    m_vertices.at(2).xyz(dstW, dstH, 0.f);
    m_vertices.at(3).xyz(0.f,  dstH, 0.f);

    m_vertices.at(0).uv(uvcX, uvcY);
    m_vertices.at(1).uv(uvcW, uvcY);
    m_vertices.at(2).uv(uvcW, uvcH);
    m_vertices.at(3).uv(uvcX, uvcH);

    m_update = true;
}


void ITexture::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void ITexture::setBlendColor(
        const QColor& tl,
        const QColor& tr,
        const QColor& br,
        const QColor& bl
        )
{
    m_vertices.at(0).rgba(tl);
    m_vertices.at(1).rgba(tr);
    m_vertices.at(2).rgba(br);
    m_vertices.at(3).rgba(bl);

    m_update = true;
}


void ITexture::setBlendMode(BlendModes modes)
{
    m_blendMode = modes;
}


const priv::QuadVertices& ITexture::vertices() const
{
    return m_vertices;
}


QOpenGLTexture* ITexture::texture() const
{
    return m_texture;
}


QOpenGLBuffer* ITexture::buffer() const
{
    return m_vertexBuffer;
}


void ITexture::requestUpdate()
{
    m_update = true;
}

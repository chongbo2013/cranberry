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
#include <Cranberry/Graphics/Base/TextureBase.hpp>
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

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Vertex buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Index buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Texture creation failed.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_ARR(uint, 6, c_ibo, 0, 1, 2, 2, 3, 0)


CRANBERRY_USING_NAMESPACE


TextureBase::TextureBase()
    : RenderBase()
    , m_blendMode(BlendNone)
    , m_effect(EffectNone)
    , m_texture(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_update(false)
{
}


TextureBase::~TextureBase()
{
    destroy();
}


bool TextureBase::isNull() const
{
    return RenderBase::isNull()        ||
           m_texture == nullptr        ||
           m_vertexBuffer == nullptr   ||
           m_indexBuffer == nullptr    ||
          !m_texture->isCreated()      ||
          !m_vertexBuffer->isCreated() ||
          !m_indexBuffer->isCreated();
}


bool TextureBase::create(const QImage& img, Window* renderTarget)
{
    if (!RenderBase::create(renderTarget))
    {
        return false;
    }
    else if (!createBuffers())
    {
        return false;
    }
    else if (!createTexture(img))
    {
        return false;
    }

    return initializeData();
}


bool TextureBase::create(QOpenGLTexture* img, Window* renderTarget)
{
    if (!RenderBase::create(renderTarget))
    {
        return false;
    }
    else if (!createBuffers())
    {
        return false;
    }

    m_texture = img;

    return initializeData();
}


bool TextureBase::createBuffers()
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


bool TextureBase::createTexture(const QImage& img)
{
    m_texture = new QOpenGLTexture(img);
    if (!m_texture->isCreated() || img.isNull())
    {
        return cranError(ERRARG(e_03));
    }

    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    return true;
}


bool TextureBase::initializeData()
{
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    setSize(m_texture->width(), m_texture->height());
    setSourceRectangle(0.0f, 0.0f, width(), height());
    setOrigin(width() / 2.0f, height() / 2.0f);
    setBlendColor(QColor(Qt::white));

    return true;
}


void TextureBase::destroy()
{
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_texture;

    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
    m_texture = nullptr;

    RenderBase::destroy();
}


void TextureBase::update(const GameTime& time)
{
    updateTransform(time);
}


void TextureBase::render()
{
    if (!prepareRendering())
    {
        return;
    }

    bindObjects();
    writeVertices();
    modifyProgram();
    modifyAttribs();
    drawElements();
    releaseObjects();
}


void TextureBase::bindObjects()
{
    // Binds the texture to unit 0.
    // TODO: Actually support blending between two textures!
    glDebug(gl->glActiveTexture(GL_TEXTURE0));
    glDebug(m_texture->bind());

    glDebug(m_vertexBuffer->bind());
    glDebug(m_indexBuffer->bind());
    glDebug(shaderProgram()->program()->bind());
}


void TextureBase::releaseObjects()
{
    glDebug(m_texture->release());
    glDebug(m_vertexBuffer->release());
    glDebug(m_indexBuffer->release());
    glDebug(shaderProgram()->program()->release());
}


void TextureBase::writeVertices()
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


void TextureBase::modifyProgram()
{
    QOpenGLShaderProgram* program = shaderProgram()->program();

    glDebug(program->setUniformValue("u_tex", GL_ZERO));
    glDebug(program->setUniformValue("u_mvp", matrix(this)));
    glDebug(program->setUniformValue("u_opac", opacity()));
    glDebug(program->setUniformValue("u_mode", (uint) m_blendMode));
    glDebug(program->setUniformValue("u_effect", (uint) m_effect));

    glDebug(program->enableAttributeArray(priv::TextureVertex::xyzAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::uvAttrib()));
    glDebug(program->enableAttributeArray(priv::TextureVertex::rgbaAttrib()));
}


void TextureBase::modifyAttribs()
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


void TextureBase::drawElements()
{
    // Renders the elements specified by the index buffer.
    glDebug(gl->glDrawElements(
                GL_TRIANGLES,
                QUADS_TO_TRIANGLES(4),
                GL_UNSIGNED_INT,
                priv::TextureVertex::xyzOffset()
                ));
}


void TextureBase::setSourceRectangle(const QRectF& rc)
{
    setSourceRectangle(rc.x(), rc.y(), rc.width(), rc.height());
}


void TextureBase::setSourceRectangle(qreal x, qreal y, qreal w, qreal h)
{
    float texW = width();
    float texH = height();
    float dstW = w;
    float dstH = h;
    float uvcX = x / texW;
    float uvcY = y / texH;
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


void TextureBase::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void TextureBase::setBlendColor(
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


void TextureBase::setBlendMode(BlendModes modes)
{
    m_blendMode = modes;
}


void TextureBase::setEffect(Effect effect)
{
    m_effect = effect;
}


priv::QuadVertices& TextureBase::vertices()
{
    return m_vertices;
}


QOpenGLTexture* TextureBase::texture() const
{
    return m_texture;
}


QOpenGLBuffer* TextureBase::buffer() const
{
    return m_vertexBuffer;
}


void TextureBase::requestUpdate()
{
    m_update = true;
}


int TextureBase::maxSize()
{
    if (QOpenGLContext::currentContext() == nullptr)
    {
        return 0;
    }

    GLint texSize;
    QOpenGLContext::currentContext()
            ->functions()
            ->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

    return texSize;
}


TextureBase::operator QString() const
{
    QString s;

    s.append(RenderBase::operator QString());
    s.append(TransformBase::operator QString());
    s.append("-- Texture\n");
    s.append("OpenGL texture ID: " + QString::number(m_texture->textureId()) + "\n\n");

    return s;
}

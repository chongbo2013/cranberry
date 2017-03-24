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


// Cranberry headers
#include <Cranberry/Graphics/Image.hpp>
#include <Cranberry/Graphics/System/GraphicsConstants.hpp>
#include <Cranberry/System/GLDebug.hpp>
#include <Cranberry/System/GLShader.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QtMath>


CRANBERRY_BEGIN_NAMESPACE


Image::Image()
    : Drawable()
    , Transformable()
    , Fadable()
    , m_refCount(new uint32_t(1))
    , m_texture(nullptr)
    , m_indexBuffer(nullptr)
    , m_needsUpdate(false)
    , m_isInit(false)
{
    g_instanceCount += 1;
}


Image::Image(const Image& other)
    : Drawable(other)
    , Transformable(other)
    , Fadable(other)
    , m_refCount(other.m_refCount)
    , m_texture(other.m_texture)
    , m_indexBuffer(other.m_indexBuffer)
    , m_needsUpdate(other.m_needsUpdate)
    , m_isInit(other.m_isInit)
{
    *m_refCount += 1;
}


Image& Image::operator =(const Image& other)
{
    m_refCount = other.m_refCount;
    m_texture = other.m_texture;
    m_indexBuffer = other.m_indexBuffer;
    m_needsUpdate = other.m_needsUpdate;
    m_isInit = other.m_isInit;
   *m_refCount += 1;

    return *this;
}


Image::~Image()
{
    g_instanceCount -= 1;
    *m_refCount -= 1;

    if (*m_refCount == 0)
    {
        delete m_refCount;
        if (m_isInit)
            destroy();
    }
}


QOpenGLTexture* Image::texture() const
{
    return m_texture;
}


float Image::visibleWidth() const
{
    return m_texture->width();
}


float Image::visibleHeight() const
{
    return m_texture->height();
}


void Image::setSourceRectangle(const QRectF& src)
{
    // Calculates vertex positions and UV coordinates.
    float texW = m_texture->width();
    float texH = m_texture->height();
    float dstW = src.width();
    float dstH = src.height();
    float uvcX = src.x() / texW;
    float uvcY = src.y() / texH;
    float uvcW = uvcX + (dstW / texW);
    float uvcH = uvcY + (dstH / texH);

    // Modifies the vertex position data.
    m_vertices.at(0).xyz(0.f,  0.f,  0.f);
    m_vertices.at(1).xyz(dstW, 0.f,  0.f);
    m_vertices.at(2).xyz(dstW, dstH, 0.f);
    m_vertices.at(3).xyz(0.f,  dstH, 0.f);

    // Modifies the coordinate data.
    m_vertices.at(0).uv(uvcX, uvcY);
    m_vertices.at(1).uv(uvcW, uvcY);
    m_vertices.at(2).uv(uvcW, uvcH);
    m_vertices.at(3).uv(uvcX, uvcH);

    m_needsUpdate = true;
}


void Image::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void Image::setBlendColor(
        const QColor& topLeft,
        const QColor& topRight,
        const QColor& btmRight,
        const QColor& btmLeft)
{
    // Modifies the color data.
    m_vertices.at(0).rgba(topLeft);
    m_vertices.at(1).rgba(topRight);
    m_vertices.at(2).rgba(btmRight);
    m_vertices.at(3).rgba(btmLeft);

    m_needsUpdate = true;
}


constexpr std::array<uint32_t, 6> indices {{0,1,2,2,3,0}};
bool Image::create(const QImage& img, Window* target)
{
    if (target == nullptr)
        target = Window::activeWindow();

    // Attempts to allocate the vertex buffer.
    if (!createInternal(target, IMAGE_VERTEX_SIZE))
        return false;

    // Creates a new texture and index buffer.
    m_texture = new QOpenGLTexture(img);
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    if (!m_indexBuffer->create())
        return false;

    // Fills the index buffer with static data.
    m_indexBuffer->bind();
    m_indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer->allocate(sizeof(uint32_t) * 6);
    m_indexBuffer->write(0, indices.data(), sizeof(uint32_t) * 6);
    m_indexBuffer->release();

    // Initializes the vertices to default values.
    setSourceRectangle(QRectF(0, 0, m_texture->width(), m_texture->height()));
    setOrigin(QVector2D(m_texture->width() / 2, m_texture->height() / 2));
    setBlendColor(QColor(Qt::black));
    m_isInit = true;

    // Eventually initializes static OpenGL resources.
    if (g_instanceCount <= 1)
        createOpenGL();

    emit created();
    return true;
}


void Image::destroy()
{
    // Destroys the texture and index buffer.
    destroyInternal();
    m_texture->destroy();
    m_indexBuffer->destroy();
    m_isInit = false;

    delete m_texture;
    delete m_indexBuffer;

    // Eventually destroys static OpenGL resources.
    if (g_instanceCount <= 0)
        destroyOpenGL();

    emit destroyed();
}


void Image::update(const GameTime& time)
{
    updateTransform(time);
    updateFade(time);
}


void Image::render()
{
    if (Q_UNLIKELY(!m_isInit))
        return;

    renderTarget()->makeCurrent();

    // Constructs the MVP matrix.
    QMatrix4x4 munit, mproj, mtran, mrot, mscale, mtor, mitor, mvp;
    renderTarget()->applyOrtho(&mproj);
    mtran.translate(x(), y());
    mrot.rotate(angle(), rotationAxis());
    mscale.scale(scale());
    mtor.translate(origin());
    mitor.translate(origin() * -1);
    mvp = mproj * mtran * mtor * mrot * mitor * mscale * munit;

    // Determines which program to use.
    auto* program = g_program;
    if (customProgram() != nullptr)
        program = customProgram();

    // Binds the OpenGL objects necessary for rendering.
    glDebug(g_vao->bind());
    glDebug(vertexBuffer()->bind());
    glDebug(m_indexBuffer->bind());

    // Updates vertex data, if requested.
    if (m_needsUpdate)
    {
        glDebug(vertexBuffer()->write(0, m_vertices.data(), IMAGE_VERTEX_SIZE));
        m_needsUpdate = false;
    }

    // Forwards the MVP matrix, opacity and blending mode.
    glDebug(program->bind());
    glDebug(program->setUniformValue("uni_texture", GL_ZERO));
    glDebug(program->setUniformValue("uni_mvp", mvp));
    glDebug(program->setUniformValue("uni_opacity", opacity()));
    glDebug(program->setUniformValue("uni_mode", (int) blendMode()));

    // Enables all the vertex attributes to be used.
    glDebug(program->enableAttributeArray(IMAGE_VERTEX_POS));
    glDebug(program->enableAttributeArray(IMAGE_VERTEX_COORD));
    glDebug(program->enableAttributeArray(IMAGE_VERTEX_COLOR));

    // Specifies the attribute buffers.
    glDebug(gl->glVertexAttribPointer(
                IMAGE_VERTEX_POS,
                IMAGE_VERTEX_POS_SIZE,
                GL_FLOAT, GL_FALSE,
                IMAGE_VERTEX_STRIDE,
                IMAGE_VERTEX_POS_OFFSET));

    glDebug(gl->glVertexAttribPointer(
                IMAGE_VERTEX_COORD,
                IMAGE_VERTEX_COORD_SIZE,
                GL_FLOAT, GL_FALSE,
                IMAGE_VERTEX_STRIDE,
                IMAGE_VERTEX_COORD_OFFSET));

    glDebug(gl->glVertexAttribPointer(
                IMAGE_VERTEX_COLOR,
                IMAGE_VERTEX_COLOR_SIZE,
                GL_FLOAT, GL_FALSE,
                IMAGE_VERTEX_STRIDE,
                IMAGE_VERTEX_COLOR_OFFSET));

    // Renders the texture.
    glDebug(gl->glActiveTexture(GL_TEXTURE0));
    glDebug(m_texture->bind());
    glDebug(gl->glDrawElements(
                GL_TRIANGLES,
                IMAGE_VERTEX_COUNT,
                GL_UNSIGNED_INT,
                IMAGE_VERTEX_POS_OFFSET));

    // Unbinds all the OpenGL objects.
    glDebug(program->release());
    glDebug(m_indexBuffer->release());
    glDebug(vertexBuffer()->release());
    glDebug(g_vao->release());
}


void Image::requestUpdate()
{
    m_needsUpdate = true;
}


std::array<VxTexture, 4>& Image::vertices()
{
    return m_vertices;
}


void Image::createOpenGL()
{
    g_vao = new QOpenGLVertexArrayObject();
    g_program = new QOpenGLShaderProgram();
    g_vao->create();

    GLShader::load(g_program, IMAGE_SHADER_VERT, IMAGE_SHADER_FRAG);
}


void Image::destroyOpenGL()
{
    g_vao->destroy();
    g_program->removeAllShaders();

    delete g_vao;
    delete g_program;
}


uint32_t Image::g_instanceCount = 0;
QOpenGLVertexArrayObject* Image::g_vao = nullptr;
QOpenGLShaderProgram* Image::g_program = nullptr;


CRANBERRY_END_NAMESPACE

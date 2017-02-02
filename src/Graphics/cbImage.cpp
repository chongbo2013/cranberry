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
#include <Cranberry/System/cbGLDebug.hpp>
#include <Cranberry/Graphics/cbImage.hpp>
#include <Cranberry/Graphics/System/cbGraphicsConstants.hpp>
#include <Cranberry/Window/cbWindow.hpp>

// Qt headers
#include <QOpenGLVertexArrayObject>

// Standard headers


CRANBERRY_BEGIN_NAMESPACE


cbImage::
cbImage(cbWindow* renderTarget)
    : cbDrawable(renderTarget)
    , m_refCount(new uint32_t)
    , m_texture(nullptr)
    , m_needsUpdate(false)
    , m_isValid(false)
{
    *m_refCount += 1;
    vertices()->resize(CBI_VERTEX_COUNT, 0);
}


cbImage::
cbImage(const cbImage& other)
    : cbDrawable(renderTarget())
    , m_refCount(other.m_refCount)
    , m_texture(other.m_texture)
    , m_mvpMatrix(other.m_mvpMatrix)
    , m_needsUpdate(other.m_needsUpdate)
    , m_isValid(other.m_isValid)
{
    *m_refCount += 1;
}


cbImage&
cbImage::operator=(const cbImage& other)
{
    m_refCount = other.m_refCount;
    m_texture = other.m_texture;
    m_mvpMatrix = other.m_mvpMatrix;
    m_needsUpdate = other.m_needsUpdate;
    m_isValid = other.m_isValid;
   *m_refCount += 1;

    return *this;
}


cbImage::
~cbImage()
{
    if (*m_refCount == 1)
    {
        // This is the last instance, free heap.
        if (m_isValid)
            destroy();
    }
    else
    {
        *m_refCount -= 1;
    }
}


QOpenGLTexture*
cbImage::texture() const
{
    return m_texture;
}


float
cbImage::visibleWidth() const
{
    return m_texture->width() /* * scale() */;
}


float
cbImage::visibleHeight() const
{
    return m_texture->height() /* * scale() */;
}


void
cbImage::setSourceRectangle(const QRectF& source)
{
    // Retrieve C++ reference to vertices to use the set* macroes.
    std::vector<float>& data = *vertices();

    // Calculates vertex positions and UV coordinates.
    float texW = m_texture->width();
    float texH = m_texture->height();
    float dstW = source.width();
    float dstH = source.height();
    float uvcX = source.x() / texW;
    float uvcY = source.y() / texH;
    float uvcW = uvcX + (dstW / texW);
    float uvcH = uvcY + (dstH / texH);

    // Modifies the vertex data.        x     y
    set_xy(data, CBI_STRIDE, VERTEX_0, 0.f,  0.f);
    set_xy(data, CBI_STRIDE, VERTEX_1, dstW, 0.f);
    set_xy(data, CBI_STRIDE, VERTEX_2, dstW, dstH);
    set_xy(data, CBI_STRIDE, VERTEX_3, 0.f,  dstH);

    // Modifies the coordinate data.    u     v
    set_uv(data, CBI_STRIDE, VERTEX_0, uvcX, uvcY);
    set_uv(data, CBI_STRIDE, VERTEX_1, uvcW, uvcY);
    set_uv(data, CBI_STRIDE, VERTEX_2, uvcW, uvcH);
    set_uv(data, CBI_STRIDE, VERTEX_3, uvcX, uvcH);

    m_needsUpdate = true;
}


void
cbImage::setBlendColor(const QColor& color)
{
    setBlendColors(color, color, color, color);
}


void
cbImage::setBlendColors(const QColor& topLeft,
                        const QColor& topRight,
                        const QColor& btmRight,
                        const QColor& btmLeft)
{
    // Retrieve C++ reference to vertices to use the set* macroes.
    std::vector<float>& data = *vertices();

    // Modifies the RGBA data.            rgba
    set_rgba(data, CBI_STRIDE, VERTEX_0, topLeft);
    set_rgba(data, CBI_STRIDE, VERTEX_1, topRight);
    set_rgba(data, CBI_STRIDE, VERTEX_2, btmRight);
    set_rgba(data, CBI_STRIDE, VERTEX_3, btmLeft);

    m_needsUpdate = true;
}


bool
cbImage::create(const QImage& img)
{
    if (!cbDrawable::create())
        return false;

    renderTarget()->makeCurrent();
    initializeOpenGLFunctions();
    m_texture = new QOpenGLTexture(img);

    if (!m_texture->create())
        return false;

    renderTarget()->doneCurrent();
    setSourceRectangle(QRectF(0, 0, m_texture->width(), m_texture->height()));

    return true;
}


void
cbImage::destroy()
{
    cbDrawable::destroy();

    if (renderTarget() == nullptr)
        return;

    renderTarget()->makeCurrent();
    m_texture->destroy();
    m_isValid = false;

    delete m_refCount;
    delete m_texture;

    renderTarget()->doneCurrent();
}


void
cbImage::update(const cbGameTime& time)
{
    //updateMovement(time);
    //updateTransform(time);
    //updateFading(time);
}


void
cbImage::render()
{
    m_mvpMatrix.setToIdentity();
    m_mvpMatrix.scale(1 /* scale() */);
    m_mvpMatrix.translate(0, 0 /* origin() * -1*/);
    m_mvpMatrix.rotate(0, /* angle() */ 0, 0, 1);
    m_mvpMatrix.translate(0, 0 /* origin() */);
    m_mvpMatrix.translate(0, 0 /* pos() */);

    // Determines which program to use.
    QOpenGLShaderProgram* program; /* = cbPrograms::image() */
    if (shaderProgram() != nullptr)
        program = shaderProgram();

    // Retrieves the vertex array for cbImage.
    QOpenGLVertexArrayObject array;

    // Binds necessary OpenGL objects.
    glDebug(array.bind());
    //glDebug(cbPrograms::imageIndexBuffer().bind());
    glDebug(vertexBuffer()->bind());
    glDebug(m_texture->bind());
    glDebug(program->bind());

    // If update was requested, update vertex data.
    if (m_needsUpdate)
        glDebug(vertexBuffer()->write(0, vertices()->data(), CBI_VERTEX_COUNT));

    // Forwards the MVP matrix and opacity to the shader.
    glDebug(program->setUniformValue("" /* cbPrograms::imageMatrix() */, m_mvpMatrix));
    glDebug(program->setUniformValue("" /* cbPrograms::imageOpacity()*/, 1 /* opacity() */));

    // Enables all the vertex attributes to use.
    glDebug(glEnableVertexAttribArray(ATTRIB_XYZ));
    glDebug(glEnableVertexAttribArray(ATTRIB_UV));
    glDebug(glEnableVertexAttribArray(ATTRIB_RGBA));

    // Fills the vertex attributes.
    glDebug(glVertexAttribPointer(
                ATTRIB_XYZ,
                ATTRIB_XYZ_SIZE,
                GL_FLOAT,
                GL_FALSE,
                CBI_STRIDE,
                nullptr));

    glDebug(glVertexAttribPointer(
                ATTRIB_UV,
                ATTRIB_UV_SIZE,
                GL_FLOAT,
                GL_FALSE,
                CBI_STRIDE,
                ATTRIB_UV_OFFSET));

    glDebug(glVertexAttribPointer(
                ATTRIB_RGBA,
                ATTRIB_RGBA_SIZE,
                GL_FLOAT,
                GL_FALSE,
                CBI_STRIDE,
                ATTRIB_RGBA_OFFSET));

    // Renders the texture.
    glDebug(glDrawElements(
                GL_TRIANGLES,
                INDEX_BUFFER_SIZE,
                GL_UNSIGNED_INT,
                nullptr));

    // Disables all the vertex attributes previously used.
    glDebug(glDisableVertexAttribArray(ATTRIB_XYZ));
    glDebug(glDisableVertexAttribArray(ATTRIB_UV));
    glDebug(glDisableVertexAttribArray(ATTRIB_RGBA));

    // Unbinds all previously used objects.
    glDebug(array.release());
    //glDebug(cbPrograms::imageIndexBuffer().release());
    glDebug(vertexBuffer()->release());
    glDebug(m_texture->release());
    glDebug(program->release());
}


CRANBERRY_END_NAMESPACE

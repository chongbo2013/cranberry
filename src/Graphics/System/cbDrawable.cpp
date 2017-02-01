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
#include <Cranberry/Graphics/System/cbDrawable.hpp>
#include <Cranberry/Window/cbWindow.hpp>

// Qt headers

// Standard headers


CRANBERRY_BEGIN_NAMESPACE


cbDrawable::
cbDrawable(cbWindow* renderTarget)
    : m_refCount(new uint32_t)
    , m_renderTarget(nullptr)
    , m_vertexBuffer(nullptr)
    , m_customProgram(nullptr)
    , m_vertices(nullptr)
    , m_isValid(true)
{
    setRenderTarget(renderTarget);
    m_vertices = new std::vector<float>();
    *m_refCount += 1;
}


cbDrawable::
cbDrawable(const cbDrawable& other)
    : m_refCount(other.m_refCount)
    , m_vertexBuffer(other.m_vertexBuffer)
    , m_customProgram(other.m_customProgram)
    , m_vertices(other.m_vertices)
{
    *m_refCount += 1;
}


cbDrawable&
cbDrawable::operator=(const cbDrawable& other)
{
    m_refCount = other.m_refCount;
    m_vertexBuffer = other.m_vertexBuffer;
    m_customProgram = other.m_customProgram;
    m_vertices = other.m_vertices;
   *m_refCount += 1;

    return *this;
}


cbDrawable::
~cbDrawable()
{
    int& ref = *m_refCount;
    if (ref == 1)
    {
        // This is the last instance, free heap.
        if (m_isValid)
            destroy();
    }
    else
    {
        ref -= 1;
    }
}


cbWindow*
cbDrawable::renderTarget() const
{
    return m_renderTarget;
}


QOpenGLShaderProgram*
cbDrawable::shaderProgram() const
{
    return m_customProgram;
}


BlendModes
cbDrawable::blendMode() const
{
    return m_blendMode;
}


void
cbDrawable::setRenderTarget(cbWindow* renderTarget)
{
    // Attempts to get the active window, if
    // no render target explicitly specified.
    if (renderTarget == nullptr)
        renderTarget = cbWindow::current();

    if (renderTarget == nullptr)
        throw "Cannot create a sprite without render target.";

    m_renderTarget = renderTarget;
}


void
cbDrawable::setShaderProgram(QOpenGLShaderProgram* program)
{
    delete m_customProgram;
    m_customProgram = program;
}


void
cbDrawable::setBlendMode(BlendModes mode)
{
    m_blendMode = mode;
}


void
cbDrawable::destroy()
{
    m_renderTarget->makeCurrent();
    m_vertexBuffer->destroy();
    m_isValid = false;

    delete m_refCount;
    delete m_vertexBuffer;
    delete m_customProgram;
    delete m_vertices;

    m_renderTarget->doneCurrent();
}


bool
cbDrawable::create()
{
    if (m_renderTarget == nullptr)
        return false;

    // Attempts to allocate a new vertex buffer.
    m_renderTarget->makeCurrent();
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::DynamicDraw);

    // If creating fails, you are most likely out of memory.
    if (!m_vertexBuffer->create())
        return false;

    m_renderTarget->doneCurrent();

    return true;
}


QOpenGLBuffer*
cbDrawable::vertexBuffer() const
{
    return m_vertexBuffer;
}


std::vector<float>*
cbDrawable::vertices() const
{
    return m_vertices;
}


CRANBERRY_END_NAMESPACE

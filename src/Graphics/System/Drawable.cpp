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
#include <Cranberry/Graphics/System/Drawable.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


CRANBERRY_BEGIN_NAMESPACE


Drawable::Drawable()
    : m_refCount(new uint32_t(1))
    , m_renderTarget(nullptr)
    , m_vertexBuffer(nullptr)
    , m_customProgram(nullptr)
    , m_blendModes(NoBlend)
    , m_isInit(false)
{
}


Drawable::Drawable(const Drawable& other)
    : QObject()
    , m_refCount(other.m_refCount)
    , m_renderTarget(other.m_renderTarget)
    , m_vertexBuffer(other.m_vertexBuffer)
    , m_customProgram(other.m_customProgram)
    , m_blendModes(other.m_blendModes)
    , m_isInit(other.m_isInit)
{
    *m_refCount += 1;
}


Drawable& Drawable::operator =(const Drawable& other)
{
    m_refCount = other.m_refCount;
    m_renderTarget = other.m_renderTarget;
    m_vertexBuffer = other.m_vertexBuffer;
    m_customProgram = other.m_customProgram;
    m_blendModes = other.m_blendModes;
    m_isInit = other.m_isInit;
   *m_refCount += 1;

    return *this;
}


Drawable::~Drawable()
{
    *m_refCount -= 1;

    if (*m_refCount == 0 && m_isInit)
        destroyInternal();
}


Window* Drawable::renderTarget() const
{
    return m_renderTarget;
}


BlendModes Drawable::blendMode() const
{
    return m_blendModes;
}


QOpenGLShaderProgram* Drawable::customProgram() const
{
    return m_customProgram;
}


void Drawable::setRenderTarget(Window* target)
{
    // If no target specified, gets active window.
    if (target == nullptr)
        target = Window::activeWindow();

    // If already created, change the functions too.
    if (m_isInit)
        gl = target->context()->functions();

    m_renderTarget = target;
}


void Drawable::setBlendMode(BlendModes mode)
{
    m_blendModes = mode;
}


void Drawable::setCustomProgram(QOpenGLShaderProgram* program)
{
    delete m_customProgram;
    m_customProgram = program;
}


bool Drawable::createInternal(Window* target, int32_t size)
{
    // Does not accept invalid target.
    if (target == nullptr)
        return false;

    target->makeCurrent();
    gl = target->context()->functions();
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_renderTarget = target;
    m_isInit = true;

    // Attempts to create the vertex buffer.
    if (!m_vertexBuffer->create())
        return false;

    m_vertexBuffer->bind();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vertexBuffer->allocate(size);

    return true;
}


void Drawable::destroyInternal()
{
    // Does not accept invalid target.
    if (m_renderTarget == nullptr)
        return;

    m_renderTarget->makeCurrent();
    m_vertexBuffer->destroy();
    m_isInit = false;

    delete m_refCount;
    delete m_vertexBuffer;
    delete m_customProgram;
}


QOpenGLBuffer* Drawable::vertexBuffer() const
{
    return m_vertexBuffer;
}


CRANBERRY_END_NAMESPACE

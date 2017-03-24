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
#include <Cranberry/Graphics/Primitive.hpp>
#include <Cranberry/Graphics/System/GraphicsConstants.hpp>
#include <Cranberry/System/GLDebug.hpp>
#include <Cranberry/System/GLShader.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QtMath>


CRANBERRY_BEGIN_NAMESPACE


Primitive::Primitive()
    : Drawable()
    , Transformable()
    , Fadable()
    , m_refCount(new uint32_t(1))
    , m_fillShape(false)
    , m_needsUpdate(false)
    , m_isInit(false)
{
    g_instanceCount += 1;
}


Primitive::Primitive(const Primitive& other)
    : Drawable(other)
    , Transformable(other)
    , Fadable(other)
    , m_refCount(other.m_refCount)
    , m_fillShape(other.m_fillShape)
    , m_needsUpdate(other.m_needsUpdate)
    , m_isInit(other.m_isInit)
{
    *m_refCount += 1;
}


Primitive& Primitive::operator =(const Primitive& other)
{
    m_refCount = other.m_refCount;
    m_fillShape = other.m_fillShape;
    m_needsUpdate = other.m_needsUpdate;
    m_isInit = other.m_isInit;
   *m_refCount += 1;

    return *this;
}


Primitive::~Primitive()
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


bool Primitive::isShapeFilled() const
{
    return m_fillShape;
}


void Primitive::setShapeFilled(bool filled)
{
    m_fillShape = filled;
}


bool Primitive::create(Window* target)
{
    if (target == nullptr)
        target = Window::activeWindow();

    // Attempts to allocate the vertex buffer.
    if (!createInternal(target, 0))
        return false;

    m_isInit = true;

    // Initializes static OpenGL resources.
    if (g_instanceCount <= 1)
        createOpenGL();

    return true;
}


void Primitive::destroy()
{
    destroyInternal();
    m_isInit = false;

    // Destroys static OpenGL resources.
    if (g_instanceCount <= 0)
        destroyOpenGL();
}


void Primitive::update(const GameTime& time)
{
    updateTransform(time);
    updateFade(time);
}


void Primitive::render()
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

    // Updates vertex data, if requested.
    if (m_needsUpdate)
    {
        glDebug(vertexBuffer()->write(0, m_vertices.data(), PRIMITIVE_VERTEX_SIZE));
        m_needsUpdate = false;
    }

    // Forwards the MVP matrix and opacity.
    glDebug(program->bind());
    glDebug(program->setUniformValue("uni_mvp", mvp));
    glDebug(program->setUniformValue("uni_opacity", opacity()));

    // Enables all vertex attributes to be used.
    glDebug(program->enableAttributeArray(PRIMITIVE_VERTEX_POS));
    glDebug(program->enableAttributeArray(PRIMITIVE_VERTEX_COLOR));

    // Specifies the attribute buffers.
    glDebug(gl->glVertexAttribPointer(
                PRIMITIVE_VERTEX_POS,
                PRIMITIVE_VERTEX_POS_SIZE,
                GL_FLOAT, GL_FALSE,
                PRIMITIVE_VERTEX_STRIDE,
                PRIMITIVE_VERTEX_POS_OFFSET));

    glDebug(gl->glVertexAttribPointer(
                PRIMITIVE_VERTEX_COLOR,
                PRIMITIVE_VERTEX_COLOR_SIZE,
                GL_FLOAT, GL_FALSE,
                PRIMITIVE_VERTEX_STRIDE,
                PRIMITIVE_VERTEX_COLOR_OFFSET));

    // Renders the primitive.
    int mode = (m_fillShape) ? renderModeFilled() : renderModeWired();
    glDebug(gl->glDrawArrays(mode, GL_ZERO, PRIMITIVE_VERTEX_COUNT));
}


void Primitive::createOpenGL()
{
    g_vao = new QOpenGLVertexArrayObject();
    g_program = new QOpenGLShaderProgram();
    g_vao->create();

    GLShader::load(g_program, PRIMITIVE_SHADER_VERT, PRIMITIVE_SHADER_FRAG);
}


void Primitive::destroyOpenGL()
{
    g_vao->destroy();
    g_program->removeAllShaders();

    delete g_vao;
    delete g_program;
}


void Primitive::requestUpdate()
{
    m_needsUpdate = true;
}


std::vector<VxPrimitive>& Primitive::vertices()
{
    return m_vertices;
}


uint32_t Primitive::renderModeWired() const
{
    return 0;
}


uint32_t Primitive::renderModeFilled() const
{
    return 0;
}


uint32_t Primitive::g_instanceCount = 0;
QOpenGLVertexArrayObject* Primitive::g_vao = nullptr;
QOpenGLShaderProgram* Primitive::g_program = nullptr;


CRANBERRY_END_NAMESPACE

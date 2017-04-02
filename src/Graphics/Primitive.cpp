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


///
/// Shared resources across all cran::Primitive instances.
///
int g_primitiveInstanceCount = 0;
QOpenGLVertexArrayObject* g_primitiveVao;
QOpenGLShaderProgram* g_primitiveProgram;

///
/// Functions to initialize/destroy the shared resources.
///
void primitiveCreateOpenGL();
void primitiveDestroyOpenGL();


Primitive::Primitive()
    : Drawable()
    , Transformable()
    , Fadable()
    , m_refCount(new uint32_t(1))
    , m_fillShape(false)
    , m_needsUpdate(false)
{
    g_primitiveInstanceCount++;
}


Primitive::Primitive(const Primitive& other)
    : Drawable(other)
    , Transformable(other)
    , Fadable(other)
    , m_refCount(other.m_refCount)
    , m_fillShape(other.m_fillShape)
    , m_needsUpdate(other.m_needsUpdate)
{
    *m_refCount += 1;
    g_primitiveInstanceCount++;
}


Primitive& Primitive::operator =(const Primitive& other)
{
    m_refCount = other.m_refCount;
    m_fillShape = other.m_fillShape;
    m_needsUpdate = other.m_needsUpdate;;
   *m_refCount += 1;
    g_primitiveInstanceCount++;

    return *this;
}


Primitive::~Primitive()
{
    *m_refCount -= 1;
    g_primitiveInstanceCount -= 1;

    // Eventually destroys local OpenGL resources.
    if (*m_refCount == 0)
    {
        this->destroy();
        delete m_refCount;
    }

    // Eventually destroys global OpenGL resources.
    if (g_primitiveInstanceCount <= 0)
    {
        primitiveDestroyOpenGL();
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

    // Initializes static OpenGL resources.
    if (g_primitiveInstanceCount <= 1)
        primitiveCreateOpenGL();

    return true;
}


void Primitive::destroy()
{
    if (isValid())
        destroyInternal();
}


void Primitive::update(const GameTime& time)
{
    updateTransform(time);
    updateFade(time);
}


void Primitive::render()
{
    if (Q_UNLIKELY(!isValid()))
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
    auto* program = g_primitiveProgram;
    if (customProgram() != nullptr)
        program = customProgram();

    // Binds the OpenGL objects necessary for rendering.
    glDebug(g_primitiveVao->bind());
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


void primitiveCreateOpenGL()
{
    g_primitiveVao = new QOpenGLVertexArrayObject();
    g_primitiveProgram = new QOpenGLShaderProgram();
    g_primitiveVao->create();

    GLShader::load(g_primitiveProgram, PRIMITIVE_SHADER_VERT, PRIMITIVE_SHADER_FRAG);
}


void primitiveDestroyOpenGL()
{
    g_primitiveVao->destroy();
    g_primitiveProgram->removeAllShaders();

    delete g_primitiveVao;
    delete g_primitiveProgram;
}


CRANBERRY_END_NAMESPACE

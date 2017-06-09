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
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QOpenGLFunctions>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - The given render target is invalid.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - There is no default shader program.")


IRenderable::IRenderable()
    : gl(nullptr)
    , m_renderTarget(nullptr)
    , m_defaultProgram(nullptr)
    , m_customProgram(nullptr)
{
}


IRenderable::~IRenderable()
{
    destroy();
}


bool IRenderable::isNull() const
{
    return m_renderTarget == nullptr;
}


bool IRenderable::create(Window* renderTarget)
{
    if (renderTarget == nullptr)
    {
        // TODO: Uncomment as soon as Window was coded.
        //if ((renderTarget = Window::activeWindow()) == nullptr)
        //{
        //    return cranError(ERRARG(e_01));
        //}
    }

    // TODO: Uncomment as soon as Window was coded.
    //gl = renderTarget->context()->functions();
    m_renderTarget = renderTarget;
    m_emitter.emitCreated();

    return true;
}


void IRenderable::destroy()
{
    //m_renderTarget->makeCurrent();
    m_emitter.emitDestroyed();
    delete m_customProgram;
}


Window* IRenderable::renderTarget() const
{
    return m_renderTarget;
}


OpenGLShader* IRenderable::shaderProgram() const
{
    if (Q_UNLIKELY(m_defaultProgram == nullptr))
    {
        cranError(ERRARG(e_02));
        return nullptr;
    }

    return (m_customProgram != nullptr) ? m_customProgram : m_defaultProgram;
}


const QString& IRenderable::name() const
{
    return m_name;
}


void IRenderable::setShaderProgram(OpenGLShader* program)
{
    m_customProgram = program;
}


void IRenderable::setDefaultShaderProgram(OpenGLShader* program)
{
    m_defaultProgram = program;
}


void IRenderable::setName(const QString& name)
{
    m_name = name;
}


RenderableEmitter* IRenderable::emitter()
{
    return &m_emitter;
}

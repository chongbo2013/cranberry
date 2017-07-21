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
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QApplication>
#include <QOpenGLFunctions>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - The given render target is invalid.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - There is no default shader program.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Cannot render invalid object.")


IRenderable::IRenderable()
    : gl(nullptr)
    , m_renderTarget(nullptr)
    , m_defaultProgram(nullptr)
    , m_customProgram(nullptr)
    , m_name("<no name>")
    , m_osRenderer(0)
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
        if ((renderTarget = Window::activeWindow()) == nullptr)
        {
            return cranError(ERRARG(e_01));
        }
    }

    gl = renderTarget->context()->functions();
    m_renderTarget = renderTarget;
    m_emitter.emitCreated();
    makeCurrent();

    return true;
}


void IRenderable::destroy()
{
    m_customProgram = nullptr;
    m_renderTarget = nullptr;
    m_emitter.emitDestroyed();
}


void IRenderable::makeCurrent()
{
    auto* cc = QOpenGLContext::currentContext();
    if (cc != renderTarget()->context() || cc->surface() != renderTarget())
    {
        renderTarget()->makeCurrent();
    }
}


bool IRenderable::prepareRendering()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_03));

#ifndef QT_DEBUG
        Game::instance()->exit(CRANBERRY_EXIT_FATAL);
#endif

        return false;
    }

    makeCurrent();
    return true;
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


uint IRenderable::offscreenRenderer() const
{
    return m_osRenderer;
}


void IRenderable::setOffscreenRenderer(uint fbo)
{
    m_osRenderer = fbo;
}


void IRenderable::setName(const QString& name)
{
    m_name = name;
}


RenderableEmitter* IRenderable::renderableEmitter()
{
    return &m_emitter;
}


IRenderable::operator QString() const
{
    QString s;

    s.append("--------------------------------------------------------\n");
    s.append("--- Cranberry object\n");
    s.append("-- Renderable\n");
    s.append(QString("Name: ") + m_name + "\n");
    s.append(QString("Is valid: ") + ((isNull())? "false\n" : "true\n"));

    if (!isNull())
    {
        s.append(QString("Render target: " + m_renderTarget->settings().title() + "\n\n"));
    }

    return s;
}


QString cran::cranResourcePath(const QString& src)
{
    if (src.startsWith(":/"))
    {
        return src;
    }

    return qApp->applicationFilePath() + "/" + src;
}

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


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
namespace
{
const QString e_01("%0 [%1] - The given render target is invalid.");
const QString e_02("%0 [%1] - There is no default shader program.");
}


IRenderable::IRenderable(QString name, QObject* parent)
    : QObject(parent)
    , gl(nullptr)
    , m_renderTarget(nullptr)
    , m_defaultProgram(nullptr)
    , m_customProgram(nullptr)
{
    setObjectName(name);
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
        //    return cranError(e_01.arg(CRANBERRY_FUNC, objectName()));
        //}
    }

    // TODO: Uncomment as soon as Window was coded.
    //gl = renderTarget->context()->functions();
    m_renderTarget = renderTarget;

    emit created();
    return true;
}


void IRenderable::destroy()
{
    delete m_customProgram;
}


OpenGLShader* IRenderable::shaderProgram() const
{
    if (Q_UNLIKELY(m_defaultProgram == nullptr))
    {
        cranError(e_02.arg(CRANBERRY_FUNC, objectName()));
        return nullptr;
    }

    return (m_customProgram != nullptr) ? m_customProgram : m_defaultProgram;
}


void IRenderable::setShaderProgram(OpenGLShader* program)
{
    m_customProgram = program;
}


void IRenderable::setDefaultShaderProgram(OpenGLShader* program)
{
    m_defaultProgram = program;
}

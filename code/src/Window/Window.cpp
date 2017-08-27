////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/Window/WindowPrivate.hpp>


CRANBERRY_USING_NAMESPACE


Window::Window()
    : m_priv(new priv::WindowPrivate(this))
{
}


Window::~Window()
{
    delete m_priv;
}


bool Window::isValid() const
{
    return m_priv->isValid();
}


bool Window::isActive() const
{
    return m_priv->isActive();
}


const WindowSettings& Window::settings() const
{
    return m_priv->settings();
}


const GameTime& Window::currentTime() const
{
    return m_priv->currentTime();
}


float Window::width() const
{
    return m_priv->width();
}


float Window::height() const
{
    return m_priv->height();
}


const QSize Window::size() const
{
    return m_priv->size();
}


QOpenGLFunctions* Window::functions() const
{
    return m_priv->functions();
}


QOpenGLContext* Window::context() const
{
    return m_priv->context();
}


QSurface* Window::surface() const
{
    return m_priv;
}


uint Window::vao() const
{
    return m_priv->vao();
}


void Window::restoreOpenGLSettings()
{
    m_priv->restoreOpenGLSettings();
}


void Window::makeCurrent()
{
    m_priv->makeCurrent();
}


void Window::showDebugOverlay(RenderBase* obj)
{
    m_priv->showDebugOverlay(obj);
}


void Window::hideDebugOverlay()
{
    m_priv->hideDebugOverlay();
}


void Window::setSettings(const WindowSettings& settings)
{
    m_priv->setSettings(settings);
}


void Window::saveScreenshot(const QString& path)
{
    m_priv->takeScreenshot().save(path);
}


void Window::exitGame()
{
    Game::instance()->exit();
}


Window* Window::activeWindow()
{
    return priv::WindowPrivate::activeWindow()->m_window;
}


void Window::registerQmlWindow(GuiManager* gm)
{
    m_priv->registerQmlWindow(gm);
}


void Window::unregisterQmlWindow(GuiManager* gm)
{
    m_priv->unregisterQmlWindow(gm);
}

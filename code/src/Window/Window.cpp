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
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>

// Qt headers
#include <QOpenGLFunctions>
#include <QtEvents>


CRANBERRY_USING_NAMESPACE


CRANBERRY_GLOBAL_VAR(Window*, g_window)
CRANBERRY_CONST_VAR(uint, c_clearMask, GL_COLOR_BUFFER_BIT|
                                       GL_STENCIL_BUFFER_BIT|
                                       GL_DEPTH_BUFFER_BIT)


Window::Window(Window* parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , m_gl(nullptr)
    , m_keyCount(0)
    , m_padCount(0)
    , m_btnCount(0)
{
    QSurfaceFormat fmt = format();
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    fmt.setSamples(8);

    // Determines between OpenGL and GLES API.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        fmt.setVersion(3, 3);
    }
    else
    {
        fmt.setVersion(3, 0);
    }

    setFormat(fmt);
    setSurfaceType(QOpenGLWindow::OpenGLSurface);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}


Window::~Window()
{
}


bool Window::isValid() const
{
    return context() != nullptr &&
           m_gl != nullptr;
}


bool Window::isActive() const
{
    return activeWindow() == this;
}


const WindowSettings& Window::settings() const
{
    return m_settings;
}


const GameTime& Window::currentTime() const
{
    return m_time;
}


QOpenGLFunctions* Window::functions() const
{
    return m_gl;
}


void Window::setSettings(const WindowSettings& settings)
{
    m_settings = settings;
}


Window* Window::activeWindow()
{
    return g_window;
}


void Window::initializeGL()
{
    const QColor& cc = m_settings.clearColor();

    m_gl = context()->functions();
    m_gl->initializeOpenGLFunctions();

    glDebug(m_gl->glClearColor(cc.redF(), cc.greenF(), cc.blueF(), cc.alphaF()));
    glDebug(m_gl->glEnable(GL_BLEND));
    glDebug(m_gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glDebug(m_gl->glEnable(GL_MULTISAMPLE));
    glDebug(m_gl->glEnable(GL_LINE_SMOOTH));

    onInit();
}


void Window::paintGL()
{
    // Forward key-down, mouse-down and pad-down events when needed.
    // Qt (logically) can not repeat sending events when e.g. a key is
    // still being held, therefore we have to do it manually every frame.
    if (m_keyCount > 0) onKeyDown(m_keyState);
    if (m_padCount > 0) onGamepadButtonDown(m_padState);
    if (m_btnCount > 0) onMouseButtonDown(m_mouseState);

    // Updating & rendering.
    m_time.update();
    onUpdate(m_time);
    glDebug(m_gl->glClear(c_clearMask));
    onRender();
}


void Window::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    MouseMoveEvent e(m_lastCursorPos, pos);

    onMouseMoved(e);
    m_lastCursorPos = pos;
}


void Window::mousePressEvent(QMouseEvent* event)
{
    m_mouseState.setButtonState(event->button(), true);
    m_btnCount++;

    onMouseButtonDown(m_mouseState);
}


void Window::mouseReleaseEvent(QMouseEvent* event)
{
    m_mouseState.setButtonState(event->button(), false);
    m_btnCount--;

    onMouseButtonReleased(MouseReleaseEvent(event->pos(), event->button()));
}


void Window::mouseDoubleClickEvent(QMouseEvent* event)
{
    onMouseDoubleClicked(MouseReleaseEvent(event->pos(), event->button()));
}


void Window::keyPressEvent(QKeyEvent* event)
{
    if (!event->text().isEmpty())
    {
        // Emits also a key character event if key has not been pressed before
        // or if the given key was repeated.
        if (!m_keyState.isKeyDown(event->key()) || event->isAutoRepeat())
        {
            onKeyCharacter(event->text());
        }
    }

    m_keyState.setKeyState(event->key(), true);
    m_keyState.setModifiers(event->modifiers());
    m_keyCount++;

    onKeyDown(m_keyState);
}


void Window::keyReleaseEvent(QKeyEvent* event)
{
    m_keyState.setKeyState(event->key(), false);
    m_keyCount--;

    onKeyReleased(KeyReleaseEvent(event->key(), event->modifiers()));
}


void Window::wheelEvent(QWheelEvent* event)
{
    onScrolled(*event);
}


void Window::touchEvent(QTouchEvent* event)
{
    onTouched(*event);
}


void Window::resizeEvent(QResizeEvent* event)
{
    onWindowResized(event->oldSize());
}


void Window::focusInEvent(QFocusEvent*)
{
    g_window = this;
    onWindowActivated();

    // Activates rendering.
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    m_time.update();
    update();
}


void Window::focusOutEvent(QFocusEvent*)
{
    if (g_window == this) g_window = nullptr;
    onWindowDeactivated();
    disconnect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}


bool Window::event(QEvent* event)
{
    if (event->type() == QEvent::Close)
    {
        makeCurrent();
        onExit();
        doneCurrent();
    }
    else if (event->type() == QEvent::Show)
    {
        g_window = this;
    }

    return QOpenGLWindow::event(event);
}

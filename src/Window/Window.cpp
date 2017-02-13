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
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/System/GLDebug.hpp>

// Qt headers
#include <QtEvents>
#include <QMatrix4x4>


CRANBERRY_BEGIN_NAMESPACE


// Global variables
Window* g_activeWindow;
GameTime g_currentTime;


Window::Window(Window* parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , m_hasVsync(false)
    , m_isInit(false)
    , m_isActive(false)
    , m_btnCount(0)
    , m_keyCount(0)
{
    QSurfaceFormat surfaceFormat = format();
    surfaceFormat.setDepthBufferSize(24);

    // Determines between the OpenGL and OpenGL/ES API.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        surfaceFormat.setVersion(3, 3);
        surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
    }
    else
    {
        surfaceFormat.setVersion(3, 0);
    }

    // Specifies the format and surface type.
    setFormat(surfaceFormat);
    setSurfaceType(QOpenGLWindow::OpenGLSurface);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}


Window::~Window()
{
}


bool Window::isInitialized() const
{
    return m_isInit;
}


bool Window::isActive() const
{
    return m_isActive;
}


bool Window::hasVerticalSync() const
{
    return m_hasVsync;
}


const QColor& Window::clearColor() const
{
    return m_clearColor;
}


QOpenGLFunctions* Window::functions() const
{
    return (QOpenGLFunctions*) &m_funcs;
}


void Window::applyOrtho(QMatrix4x4* mat)
{
    mat->ortho(0.f, (float) width(), (float) height(), 0.f, -1.f, 1.f);
}


Window* Window::activeWindow()
{
    return g_activeWindow;
}


GameTime* Window::currentTime()
{
    return &g_currentTime;
}


void Window::setVerticalSync(bool enable)
{
    QSurfaceFormat sf = format();

    if (enable)
        sf.setSwapInterval(1);
    else
        sf.setSwapInterval(0);

    m_hasVsync = enable;
    setFormat(sf);
}


void Window::setClearColor(const QColor& color)
{
    m_clearColor = color;

    // Immediately specifies the clear color
    // if the OpenGL window is initialized.
    if (m_isInit)
    {
        m_funcs.glClearColor(
                color.redF(),
                color.greenF(),
                color.blueF(),
                color.alphaF());
    }
}


void Window::initializeGL()
{
    // Initializes the OpenGL functions.
    m_funcs.initializeOpenGLFunctions();
    m_isInit = true;
    init();

    // Specifies the clear color for the first time.
    m_funcs.glClearColor(
            m_clearColor.redF(),
            m_clearColor.greenF(),
            m_clearColor.blueF(),
            m_clearColor.alphaF());

    // Enables blending
    m_funcs.glEnable(GL_BLEND);
    m_funcs.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Window::paintGL()
{
    // Performs updates for held down keys and buttons.
    if (m_keyCount > 0)
        keyDown(m_keyState);
    if (m_btnCount > 0)
        mouseDown(m_mouseState);

    // Performs updating and rendering
    g_currentTime.update();
    update(g_currentTime);
    glDebug(m_funcs.glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    render();
}


void Window::mouseMoveEvent(QMouseEvent* event)
{
    QPoint currentPos = event->pos();
    MouseMoveEvent e(m_lastCursorPos, currentPos);
    m_lastCursorPos = currentPos;
    mouseMove(e);
}


void Window::mousePressEvent(QMouseEvent* event)
{
    // Changes the given button state to true.
    m_mouseState.m_buttons[event->button()] = true;
    m_btnCount++;

    mouseDown(m_mouseState);
}


void Window::mouseReleaseEvent(QMouseEvent* event)
{
    // Changes the button state to false.
    m_mouseState.m_buttons[event->button()] = false;
    m_btnCount--;

    MouseReleaseEvent e(event->pos(), event->button());
    mouseUp(e);
}


void Window::mouseDoubleClickEvent(QMouseEvent* event)
{
    MouseReleaseEvent e(event->pos(), event->button());
    mouseDoubleClick(e);
}


void Window::keyPressEvent(QKeyEvent* event)
{
    if (!event->text().isEmpty())
    {
        // Emits a key-char event if key has not been pressed
        // before or if the given key was repeated.
        if (!m_keyState.m_keys.value(event->key()) || event->isAutoRepeat())
            keyChar(event->text());
    }

    // Changes the key state to true.
    m_keyState.m_keys[event->key()] = true;
    m_keyState.m_mods = event->modifiers();
    m_keyCount++;

    keyDown(m_keyState);
}


void Window::keyReleaseEvent(QKeyEvent* event)
{
    // Changes the key state to false.
    m_keyState.m_keys[event->key()] = false;
    m_keyCount--;

    KeyReleaseEvent e(event->key(), event->modifiers());
    keyUp(e);
}


void Window::wheelEvent(QWheelEvent* event)
{
    scroll(*event);
}


void Window::touchEvent(QTouchEvent* event)
{
    touch(*event);
}


void Window::resizeEvent(QResizeEvent* event)
{
    windowResize(event->oldSize());
}


void Window::focusInEvent(QFocusEvent*)
{
    m_isActive = true;
    g_activeWindow = this;
    windowActivate();

    // Activates rendering again.
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    QOpenGLWindow::update();

    // TODO: Resume sound playing.
}


void Window::focusOutEvent(QFocusEvent*)
{
    m_isActive = false;
    windowDeactivate();

    // Deactivates rendering.
    disconnect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    // TODO: Pause sound playing.
}


bool Window::event(QEvent* event)
{
    if (event->type() == QEvent::Close)
    {
        makeCurrent();
        exit();
        doneCurrent();
    }

    return QOpenGLWindow::event(event);
}


void Window::init() { }
void Window::exit() { }
void Window::update(const GameTime&) { }
void Window::render() { }
void Window::mouseMove(const MouseMoveEvent&) { }
void Window::mouseDown(const MouseState&) { }
void Window::mouseUp(const MouseReleaseEvent&) { }
void Window::mouseDoubleClick(const MouseReleaseEvent&) { }
void Window::keyDown(const KeyboardState&) { }
void Window::keyUp(const KeyReleaseEvent&) { }
void Window::keyChar(const QString&) { }
void Window::scroll(const QWheelEvent&) { }
void Window::touch(const QTouchEvent&) { }
void Window::windowResize(const QSize&) { }
void Window::windowActivate() { }
void Window::windowDeactivate() { }


CRANBERRY_END_NAMESPACE

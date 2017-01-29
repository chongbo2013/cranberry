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
#include <Cranberry/Window/cbWindow.hpp>

// Qt headers
#include <QtEvents>


CRANBERRY_BEGIN_NAMESPACE


cbWindow::
cbWindow(cbWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , QOpenGLFunctions()
    , m_isInitialized(false)
    , m_isActive(false)
    , m_keyCount(0)
{
    setSurfaceType(QOpenGLWindow::OpenGLSurface);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}


bool
cbWindow::isInitialized() const
{
    return m_isInitialized;
}


bool
cbWindow::isActive() const
{
    return m_isActive;
}


const QColor&
cbWindow::clearColor() const
{
    return m_clearColor;
}


void
cbWindow::setClearColor(const QColor& color)
{
    m_clearColor = color;

    if (m_isInitialized)
    {
        glClearColor(color.redF(),
                     color.greenF(),
                     color.blueF(),
                     color.alphaF());
    }
}


void
cbWindow::enableVerticalSync(bool enable)
{
    QSurfaceFormat sf = format();

    if (enable)
        sf.setSwapInterval(1);
    else
        sf.setSwapInterval(0);

    setFormat(sf);
}


void
cbWindow::initializeGL()
{
    // Initializes the OpenGL functions and other resources.
    initializeOpenGLFunctions();
    onInit();
    m_isInitialized = true;

    // Specifies the clear color for the first time.
    glClearColor(m_clearColor.redF(),
                 m_clearColor.greenF(),
                 m_clearColor.blueF(),
                 m_clearColor.alphaF());
}


void
cbWindow::paintGL()
{
    // Performs updates for held down keys.
    if (m_keyCount > 0)
        onKeyDown(m_keyState);

    // Performs updates and rendering.
    onUpdate();
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    onRender();
}


void
cbWindow::mouseMoveEvent(QMouseEvent* event)
{
    cbMouseMoveEvent ev(m_lastCursorPos, event->pos());
    onMouseMove(ev);
    m_lastCursorPos = event->pos();
}


void
cbWindow::mousePressEvent(QMouseEvent* event)
{
    cbMouseButtonEvent ev(event->pos(), event->buttons());
    onMouseDown(ev);
}


void
cbWindow::mouseReleaseEvent(QMouseEvent* event)
{
    cbMouseButtonEvent ev(event->pos(), ~event->buttons());
    onMouseDown(ev);
}


void
cbWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    cbMouseButtonEvent ev(event->pos(), event->buttons());
    onMouseDoubleClick(ev);
}


void
cbWindow::keyPressEvent(QKeyEvent* event)
{
    if (!event->text().isEmpty())
    {
        // Emits a key-char event if key has not been pressed before or
        // if the given key was repeated.
        if (!m_keyState.m_keys.value(event->key()) || event->isAutoRepeat())
            onKeyChar(event->text());
    }

    // Changes the key state to true.
    m_keyState.m_keys[event->key()] = true;
    m_keyState.m_mods = event->modifiers();
    m_keyCount++;

    // Calls the pure virtual onKeyDown function.
    onKeyDown(m_keyState);
}


void
cbWindow::keyReleaseEvent(QKeyEvent* event)
{
    // Changes the key state to false.
    m_keyState.m_keys[event->key()] = false;
    m_keyCount--;

    // Calls the pure virtual onKeyUp function.
    cbKeyUpEvent ev(event->key(), event->modifiers());
    onKeyUp(ev);
}


void
cbWindow::wheelEvent(QWheelEvent* event)
{
    onScroll(*event);
}


void
cbWindow::touchEvent(QTouchEvent* event)
{
    onTouch(*event);
}


void
cbWindow::resizeEvent(QResizeEvent* event)
{
    onResize(event->size());
}


void
cbWindow::focusInEvent(QFocusEvent*)
{
    m_isActive = true;
    onActivate();

    // Activates rendering again.
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    update();

    // Resumes sound playing.
}


void
cbWindow::focusOutEvent(QFocusEvent*)
{
    m_isActive = false;
    onDeactivate();

    // Deactivates rendering.
    disconnect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    // Pauses sound playing.
}


bool
cbWindow::event(QEvent* event)
{
    if (event->type() == QEvent::Close)
    {
        makeCurrent();
        onExit();
        doneCurrent();
    }

    return QOpenGLWindow::event(event);
}


CRANBERRY_END_NAMESPACE

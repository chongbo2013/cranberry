////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#include <Cranberry/Gui/GuiManager.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/Window/WindowPrivate.hpp>

// Qt headers
#include <QApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QScreen>
#include <QtEvents>


CRANBERRY_USING_NAMESPACE


CRANBERRY_GLOBAL_VAR(priv::WindowPrivate*, g_window)
CRANBERRY_CONST_VAR(uint, c_clearMask, GL_COLOR_BUFFER_BIT   |
                                       GL_STENCIL_BUFFER_BIT |
                                       GL_DEPTH_BUFFER_BIT   )


priv::WindowPrivate::WindowPrivate(cran::Window* w)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
    , m_gl(nullptr)
    , m_window(w)
    , m_activeGui(nullptr)
    , m_keyCount(0)
    , m_padCount(0)
    , m_btnCount(0)
    , m_isMainWindow(false)
    , m_fakeFocusOut(false)
{
    QSurfaceFormat fmt = format();
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);

    // Determines between OpenGL and GLES API.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    }
    else
    {
        fmt.setVersion(3, 0);
    }

    setFormat(fmt);
    setSurfaceType(QOpenGLWindow::OpenGLSurface);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}


priv::WindowPrivate::~WindowPrivate()
{
}


bool priv::WindowPrivate::isValid() const
{
    return context() != nullptr && m_gl != nullptr;
}


bool priv::WindowPrivate::isActive() const
{
    return activeWindow() == this;
}


const WindowSettings& priv::WindowPrivate::settings() const
{
    return m_settings;
}


const GameTime& priv::WindowPrivate::currentTime() const
{
    return m_time;
}


QOpenGLFunctions* priv::WindowPrivate::functions() const
{
    return m_gl;
}


uint priv::WindowPrivate::vao() const
{
    return m_vao;
}


void priv::WindowPrivate::restoreOpenGLSettings()
{
    const QColor& cc = m_settings.clearColor();

    glDebug(m_gl->glViewport(0, 0, width(), height()));
    glDebug(m_gl->glClearColor(cc.redF(), cc.greenF(), cc.blueF(), cc.alphaF()));
    glDebug(m_gl->glEnable(GL_BLEND));
    glDebug(m_gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glDebug(m_gl->glEnable(GL_MULTISAMPLE));
    glDebug(m_gl->glEnable(GL_LINE_SMOOTH));
    glDebug(m_gl->glDisable(GL_DEPTH_TEST));
    glDebug(m_gl->glDepthMask(GL_FALSE));
    glDebug(context()->extraFunctions()->glBindVertexArray(m_vao));
}


void priv::WindowPrivate::setSettings(const WindowSettings& settings)
{
    m_settings = settings;
    parseSettings();
}


QPixmap priv::WindowPrivate::takeScreenshot()
{
    return screen()->grabWindow(winId());
}


priv::WindowPrivate* priv::WindowPrivate::activeWindow()
{
    return g_window;
}


void priv::WindowPrivate::initializeGL()
{
    m_gl = context()->functions();
    m_gl->initializeOpenGLFunctions();

    // Create a single VAO which will be bound all the time.
    auto* vao = new QOpenGLVertexArrayObject(this);
    if (!vao->create())
    {
        cranError("Window: OpenGL VAOs are not supported.");
    }
    else
    {
        vao->bind();
        m_vao = vao->objectId();
    }

    restoreOpenGLSettings();

    // Load shaders only once - for the main window.
    if (m_isMainWindow)
    {
        OpenGLDefaultShaders::cranberryLoadDefaultShaders();
        OpenGLDefaultShaders::cranberryInitDefaultShaders();
    }

    m_window->onInit();
}


void priv::WindowPrivate::registerQmlWindow(GuiManager* qw)
{
    m_guiWindows.append(qw);
}


void priv::WindowPrivate::unregisterQmlWindow(GuiManager* qw)
{
    m_guiWindows.removeOne(qw);
}


void priv::WindowPrivate::dispatchEvents(QEvent* event)
{
    auto localMousePos   = mapFromGlobal(QCursor::pos());
    bool canReceiveFocus = event->type() == QEvent::MouseButtonPress;

    for (GuiManager* w : m_guiWindows)
    {
        if (canReceiveFocus)
        {
            QRectF hitbox = w->rect();
            hitbox.moveTo(hitbox.topLeft() + w->topLeft());

            // Is current manager in range of the mouse cursor?
            if (hitbox.contains(localMousePos.x(), localMousePos.y()))
            {
                m_activeGui = w;
                m_fakeFocusOut = true;
                m_activeGui->window()->requestActivate();

                setFlags(flags() | Qt::WindowDoesNotAcceptFocus);
            }
            else
            {
                if (m_activeGui == w)
                {
                    m_activeGui = nullptr;
                    m_fakeFocusOut = false;

                    setFlags(flags() & ~Qt::WindowDoesNotAcceptFocus);
                    requestActivate();
                }
            }
        }

        // Only receive events if in focus.
        if (m_activeGui == w)
        {
            QCoreApplication::sendEvent(w->window(), event);
        }
    }
}


void priv::WindowPrivate::parseSettings()
{
    QSurfaceFormat sf = format();
    sf.setSwapInterval((m_settings.useVerticalSync()) ? 1 : 0);
    sf.setSwapBehavior((m_settings.isDoubleBuffered())
                       ? QSurfaceFormat::DoubleBuffer
                       : QSurfaceFormat::SingleBuffer
                       );

    // Fix: Must recreate window after format change.
    destroy();
    setFormat(sf);

    setTitle(m_settings.title());
    setPosition(m_settings.position());
    resize(m_settings.size());

    if (!m_settings.isResizable())
    {
        setMinimumSize(m_settings.size());
        setMaximumSize(m_settings.size());
    }
    if (m_settings.isFullscreen())
    {
        setWindowState(Qt::WindowFullScreen);
    }

    create();
}


void priv::WindowPrivate::destroyGL()
{
    // Unloads all the default shader programs only once - for the main window.
    if (m_isMainWindow)
    {
        OpenGLDefaultShaders::cranberryFreeDefaultShaders();
    }

    m_window->onExit();
}


void priv::WindowPrivate::paintGL()
{
    // Forward key-down, mouse-down and pad-down events when needed.
    // Qt (logically) can not repeat sending events when e.g. a key is
    // still being held, therefore we have to do it manually every frame.
    if (m_keyCount > 0) m_window->onKeyDown(m_keyState);
    if (m_padCount > 0) m_window->onGamepadButtonDown(m_padState);
    if (m_btnCount > 0) m_window->onMouseButtonDown(m_mouseState);

    // Updating & rendering.
    m_time.update();

#ifdef QT_DEBUG
    calculateFramerate();
#endif

    // Update shaders that require time for noise.
    OpenGLDefaultShaders::cranberryUpdateDefaultShaders();

    m_window->onUpdate(m_time);
    glDebug(m_gl->glClear(c_clearMask));
    m_window->onRender();
}


void priv::WindowPrivate::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    MouseMoveEvent e(m_lastCursorPos, pos);

    m_window->onMouseMoved(e);
    m_lastCursorPos = pos;

    dispatchEvents(event);
}


void priv::WindowPrivate::mousePressEvent(QMouseEvent* event)
{
    m_mouseState.setButtonState(event->button(), true);
    m_btnCount++;
    m_window->onMouseButtonDown(m_mouseState);

    dispatchEvents(event);
}


void priv::WindowPrivate::mouseReleaseEvent(QMouseEvent* event)
{
    m_mouseState.setButtonState(event->button(), false);
    m_btnCount--;

    m_window->onMouseButtonReleased(MouseReleaseEvent(event->pos(), event->button()));
    dispatchEvents(event);
}


void priv::WindowPrivate::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_window->onMouseDoubleClicked(MouseReleaseEvent(event->pos(), event->button()));
    dispatchEvents(event);
}


void priv::WindowPrivate::keyPressEvent(QKeyEvent* event)
{
    // Does not send key events if a GuiManager is in focus.
    if (m_activeGui == nullptr)
    {
        if (!event->text().isEmpty())
        {
            // Emits also a key character event if key has not been pressed before
            // or if the given key was repeated.
            if (!m_keyState.isKeyDown(event->key()) || event->isAutoRepeat())
            {
                m_window->onKeyCharacter(event->text());
            }
        }

        m_keyState.setKeyState(event->key(), true);
        m_keyState.setModifiers(event->modifiers());
        m_keyCount++;
        m_window->onKeyDown(m_keyState);

        dispatchEvents(event);
    }
}


void priv::WindowPrivate::keyReleaseEvent(QKeyEvent* event)
{
    // Does not send key events if a GuiManager is in focus.
    if (m_activeGui == nullptr)
    {
        m_keyState.setKeyState(event->key(), false);
        m_keyCount--;

        if (!event->isAutoRepeat())
        {
            m_window->onKeyReleased(KeyReleaseEvent(event->key(), event->modifiers()));
            dispatchEvents(event);
        }
    }
}


void priv::WindowPrivate::wheelEvent(QWheelEvent* event)
{
    //m_window->onScrolled(*event);
    dispatchEvents(event);
}


void priv::WindowPrivate::touchEvent(QTouchEvent* event)
{
    //m_window->onTouched(*event);
    dispatchEvents(event);
}


void priv::WindowPrivate::resizeEvent(QResizeEvent* event)
{
    m_window->onWindowResized(event->oldSize());
}


void priv::WindowPrivate::focusInEvent(QFocusEvent*)
{
    g_window = this;
    m_window->onWindowActivated();

    // Activates rendering.
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    m_time.update();
    update();
}


void priv::WindowPrivate::focusOutEvent(QFocusEvent*)
{
    if (!m_fakeFocusOut)
    {
        if (g_window == this)
        {
            g_window = nullptr;
        }

        m_window->onWindowDeactivated();
        disconnect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    }

    m_fakeFocusOut = false;
}


bool priv::WindowPrivate::event(QEvent* event)
{
    if (event->type() == QEvent::Close)
    {
        makeCurrent();
        destroyGL();
        doneCurrent();
    }
    else if (event->type() == QEvent::Show)
    {
        g_window = this;
    }

    return QOpenGLWindow::event(event);
}


#ifdef QT_DEBUG
void priv::WindowPrivate::calculateFramerate()
{
    const static QString format = "%0 (%1 fps)";
    double ms = m_time.deltaTime() * 1000.0;
    double fps = 1000.0 / ms;

    setTitle(format.arg(m_settings.title(), QString::number(fps)));
}
#endif

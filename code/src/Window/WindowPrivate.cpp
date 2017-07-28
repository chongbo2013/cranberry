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
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/System/Models/TreeModelPrivate.hpp>
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/Window/WindowPrivate.hpp>

// Qt headers
#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
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
CRANBERRY_CONST_VAR(uint, c_dbgInterval, 16)
CRANBERRY_CONST_VAR(uint, c_clearMask, GL_COLOR_BUFFER_BIT   |
                                       GL_STENCIL_BUFFER_BIT |
                                       GL_DEPTH_BUFFER_BIT   )


priv::WindowPrivate::WindowPrivate(cran::Window* w)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
    , m_gl(nullptr)
    , m_window(w)
    , m_dbgOverlay(nullptr)
    , m_guiOverlay(new GuiManager)
    , m_debugModel(new TreeModel)
    , m_activeGui(nullptr)
    , m_keyCount(0)
    , m_padCount(0)
    , m_btnCount(0)
    , m_dbgFrames(0)
    , m_isMainWindow(false)
    , m_fakeFocusOut(false)
{
    qRegisterMetaType<cran::priv::TreeModelPrivate*>();

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
    delete m_debugModel;
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


void priv::WindowPrivate::showDebugOverlay(RenderBase* obj)
{
    if (m_dbgOverlay == nullptr || obj == nullptr)
    {
        m_dbgOverlay = obj;

        if (obj != nullptr)
        {
            m_guiOverlay->setVisible(true);
            m_debugModel->removeAllItems();

            obj->createProperties(m_debugModel);

            m_debugModel->model()->finalizeInsertion();

            resizeDebugOverlay();
            setActiveGui(m_guiOverlay);
        }
        else
        {
            hideDebugOverlay();
        }
    }
}


void priv::WindowPrivate::hideDebugOverlay()
{
    if (m_dbgOverlay != nullptr)
    {
        m_guiOverlay->setVisible(false);
        m_debugModel->removeAllItems();
        m_debugModel->update();
        m_dbgOverlay = nullptr;
    }
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

    // Tries to find a monospace font for our overlay.
    QFont font, font2;
    font.setKerning(true);
    font.setStyleHint(QFont::TypeWriter, QFont::PreferAntialias);
    font.setPointSizeF(11.5);
    font2 = font;
    font2.setItalic(true);

    // Load the debug overlay Gui.
    m_guiOverlay->context()->setContextProperty("monospace", font);
    m_guiOverlay->context()->setContextProperty("monospaceItalic", font2);
    m_guiOverlay->context()->setContextProperty("debugModel", m_debugModel->model());
    m_guiOverlay->create("qrc:/cb/qml/DebugOverlay.qml", m_window);

    // Initialize some event logic for the overlay.
    m_guiOverlay->window()->installEventFilter(this);
    m_guiOverlay->setVisible(false); // Invisible by default
    m_guiOverlay->setTransparentToKeyInput(true);

    // Adjust the size of the Gui to the current window.
    resizeDebugOverlay();
}


void priv::WindowPrivate::registerQmlWindow(GuiManager* qw)
{
    m_guiWindows.append(qw);
    qw->window()->installEventFilter(this);
}


void priv::WindowPrivate::unregisterQmlWindow(GuiManager* qw)
{
    m_guiWindows.removeOne(qw);
    qw->window()->removeEventFilter(this);
}


void priv::WindowPrivate::resizeDebugOverlay()
{
    if (!m_guiOverlay->isNull() &&
        (width()  != m_guiOverlay->window()->width() ||
         height() != m_guiOverlay->window()->height()))
    {
        m_guiOverlay->setSize(width(), height());
        m_guiOverlay->window()->setGeometry(0, 0, width(), height());
        m_guiOverlay->window()->contentItem()->setSize(QSizeF(width(), height()));
        m_guiOverlay->rootItem()->setSize(QSizeF(width(), height()));
    }
}


void priv::WindowPrivate::setActiveGui(GuiManager* g)
{
    m_activeGui = g;
    m_fakeFocusOut = true;
    m_activeGui->window()->requestActivate();

    setFlags(flags() | Qt::WindowDoesNotAcceptFocus);
}


void priv::WindowPrivate::unsetActiveGui()
{
    m_activeGui = nullptr;
    m_fakeFocusOut = false;

    setFlags(flags() & ~Qt::WindowDoesNotAcceptFocus);
    requestActivate();
}


GuiManager* priv::WindowPrivate::findGuiManager(QQuickWindow* pQQ)
{
    if (pQQ == nullptr)
    {
        return nullptr;
    }

    for (GuiManager* g : m_guiWindows)
    {
        if (g->window() == pQQ)
        {
            return g;
        }
    }

    return nullptr;
}


void priv::WindowPrivate::dispatchEvents(QEvent* event)
{
    auto localMousePos   = mapFromGlobal(QCursor::pos());
    bool canReceiveFocus = event->type() == QEvent::MouseButtonPress;

    for (GuiManager* g : m_guiWindows)
    {
        if (g->isVisible())
        {
            if (canReceiveFocus)
            {
                QRectF hitbox = g->rect();
                hitbox.moveTo(hitbox.topLeft() + g->topLeft());

                // Is current manager in range of the mouse cursor?
                if (hitbox.contains(localMousePos.x(), localMousePos.y()))
                {
                    setActiveGui(g);
                }
                else
                {
                    if (m_activeGui == g)
                    {
                        unsetActiveGui();
                    }
                }
            }

            // Only receive events if in focus.
            if (m_activeGui == g)
            {
                QCoreApplication::sendEvent(g->window(), event);
            }
        }
    }
}


void priv::WindowPrivate::renderDebugOverlay()
{
    m_guiOverlay->render();
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

    if (m_dbgOverlay != nullptr)
    {
        if (m_dbgFrames >= c_dbgInterval)
        {
            m_dbgFrames = 0;
            m_dbgOverlay->updateProperties();
            m_debugModel->update();
        }
        else
        {
            m_dbgFrames++;
        }

        renderDebugOverlay();
    }
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

    //dispatchEvents(event);
}


void priv::WindowPrivate::keyReleaseEvent(QKeyEvent* event)
{
    m_keyState.setKeyState(event->key(), false);
    m_keyCount--;

    if (!event->isAutoRepeat())
    {
        m_window->onKeyReleased(KeyReleaseEvent(event->key(), event->modifiers()));
        // dispatchEvents(event);
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

    // Update the size of the debug overlay.
    resizeDebugOverlay();
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


bool priv::WindowPrivate::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_guiOverlay->window() && m_guiOverlay->isVisible())
    {
        if (event->type() == QEvent::KeyPress)
        {
            // Hook: Escape debug overlay by pressing ESCAPE.
            if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape)
            {
                hideDebugOverlay();
                unsetActiveGui();
            }
        }
    }

    GuiManager* manager = findGuiManager(static_cast<QQuickWindow*>(watched));
    if (manager != nullptr)
    {
        if ((event->type() == QEvent::KeyPress    ||
             event->type() == QEvent::KeyRelease) &&
             manager->isTransparentToKeyInput())
        {
            // Transparent to key input; send event to this window instead.
            QCoreApplication::sendEvent(this, event);
        }
    }

    return QOpenGLWindow::eventFilter(watched, event);
}


if_debug
(
void priv::WindowPrivate::calculateFramerate()
{
    const static QString format = "%0 (%1 fps)";
    double ms = m_time.deltaTime() * 1000.0;
    double fps = 1000.0 / ms;

    setTitle(format.arg(m_settings.title(), QString::number(fps)));
}
)

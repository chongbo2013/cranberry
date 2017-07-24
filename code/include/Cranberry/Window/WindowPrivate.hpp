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


#pragma once
#ifndef CRANBERRY_WINDOW_WINDOWPRIVATE_HPP
#define CRANBERRY_WINDOW_WINDOWPRIVATE_HPP


// Cranberry headers
#include <Cranberry/Input/KeyboardState.hpp>
#include <Cranberry/Input/KeyReleaseEvent.hpp>
#include <Cranberry/Input/MouseMoveEvent.hpp>
#include <Cranberry/Input/MouseReleaseEvent.hpp>
#include <Cranberry/Input/MouseState.hpp>
#include <Cranberry/Input/GamepadReleaseEvent.hpp>
#include <Cranberry/Input/GamepadState.hpp>
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/Window/WindowSettings.hpp>

// Qt headers
#include <QOpenGLWindow>

// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLFunctions)
CRANBERRY_FORWARD_C(GuiManager)
CRANBERRY_FORWARD_C(OpenGLShader)
CRANBERRY_FORWARD_C(RenderBase)
CRANBERRY_FORWARD_C(TreeModel)
CRANBERRY_FORWARD_C(Window)
CRANBERRY_ALIAS(QList<cran::GuiManager*>, GuiWindows)


CRANBERRY_BEGIN_PRIV_NAMESPACE


class WindowPrivate final : public QOpenGLWindow
{
public:

    CRANBERRY_DISABLE_COPY(WindowPrivate)
    CRANBERRY_DISABLE_MOVE(WindowPrivate)

    WindowPrivate(cran::Window*);
   ~WindowPrivate();


    bool isValid() const;
    bool isActive() const;
    const WindowSettings& settings() const;
    const GameTime& currentTime() const;
    QOpenGLFunctions* functions() const;
    uint vao() const;
    void restoreOpenGLSettings();
    void setSettings(const WindowSettings& settings);
    void showDebugOverlay(RenderBase* obj);
    void hideDebugOverlay();
    QPixmap takeScreenshot();
    static WindowPrivate* activeWindow();


protected:

    void initializeGL() override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void touchEvent(QTouchEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void registerQmlWindow(GuiManager*);
    void unregisterQmlWindow(GuiManager*);
    void resizeDebugOverlay();
    void setActiveGui(GuiManager* g);
    void unsetActiveGui();
    void dispatchEvents(QEvent*);
    void renderDebugOverlay();
    void parseSettings();
    void destroyGL();
    if_debug(void calculateFramerate())

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* m_gl;
    cran::Window*     m_window;
    cran::RenderBase* m_dbgOverlay;
    GuiManager*       m_guiOverlay;
    TreeModel*        m_debugModel;
    GuiWindows        m_guiWindows;
    GuiManager*       m_activeGui;
    WindowSettings    m_settings;
    GameTime          m_time;
    KeyboardState     m_keyState;
    GamepadState      m_padState;
    MouseState        m_mouseState;
    QPoint            m_lastCursorPos;
    qint32            m_keyCount;
    qint32            m_padCount;
    qint32            m_btnCount;
    uint              m_vao;
    bool              m_isMainWindow;
    bool              m_fakeFocusOut;

    friend class Game;
    friend class GuiManager;
    friend class Window;
};


CRANBERRY_END_PRIV_NAMESPACE


#endif

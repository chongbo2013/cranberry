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


#pragma once
#ifndef CRANBERRY_WINDOW_HPP
#define CRANBERRY_WINDOW_HPP


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
CRANBERRY_FORWARD_C(OpenGLShader)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// The base class for all game windows. Upon subclassing, make sure to override
/// the events (in reality you want to override all).
///
/// \class Window
/// \author Nicolas Kogler
/// \date June 12, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_WINDOW_EXPORT Window : public QOpenGLWindow
{
public:

    CRANBERRY_DEFAULT_COPY(Window)
    CRANBERRY_DEFAULT_MOVE(Window)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new window with an optional parent.
    ///
    /// \param parent Parent window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Window(Window* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~Window();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this window is valid.
    ///
    /// \returns true if valid.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isValid() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this window is active.
    ///
    /// \returns true if active.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isActive() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the window settings.
    ///
    /// \returns the current settings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const WindowSettings& settings() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current time.
    ///
    /// \returns the current time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const GameTime& currentTime() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the OpenGL functions of the window's context.
    ///
    /// \returns the OpenGL functions.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* functions() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specify the settings for this window. Call this before you call start().
    ///
    /// \param settings New window settings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSettings(const WindowSettings& settings);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the currently active window.
    ///
    /// \returns the active window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static Window* activeWindow();


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

    virtual void onInit() { }
    virtual void onExit() { }
    virtual void onCrash() { }
    virtual void onUpdate(const GameTime&) { }
    virtual void onRender() { }
    virtual void onMouseMoved(const MouseMoveEvent&) { }
    virtual void onMouseButtonDown(const MouseState&) { }
    virtual void onMouseButtonReleased(const MouseReleaseEvent&) { }
    virtual void onMouseDoubleClicked(const MouseReleaseEvent&) { }
    virtual void onKeyDown(const KeyboardState&) { }
    virtual void onKeyReleased(const KeyReleaseEvent&) { }
    virtual void onKeyCharacter(const QString&) { }
    virtual void onGamepadButtonDown(const GamepadState&) { }
    virtual void onGamepadButtonReleased(const GamepadReleaseEvent&) { }
    virtual void onScrolled(const QWheelEvent&) { }
    virtual void onTouched(const QTouchEvent&) { }
    virtual void onWindowResized(const QSize&) { }
    virtual void onWindowActivated() { }
    virtual void onWindowDeactivated() { }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    OpenGLShader* loadShader(const char* name);
    void destroyGL();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* m_gl;
    WindowSettings    m_settings;
    GameTime          m_time;
    KeyboardState     m_keyState;
    GamepadState      m_padState;
    MouseState        m_mouseState;
    QPoint            m_lastCursorPos;
    qint32            m_keyCount;
    qint32            m_padCount;
    qint32            m_btnCount;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Window
/// \ingroup Window
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

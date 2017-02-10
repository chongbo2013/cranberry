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


#pragma once
#ifndef CRANBERRY_WINDOW_HPP
#define CRANBERRY_WINDOW_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/Window/Input/KeyboardState.hpp>
#include <Cranberry/Window/Input/KeyReleaseEvent.hpp>
#include <Cranberry/Window/Input/MouseState.hpp>
#include <Cranberry/Window/Input/MouseMoveEvent.hpp>
#include <Cranberry/Window/Input/MouseReleaseEvent.hpp>

// Qt headers
#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>


CRANBERRY_BEGIN_NAMESPACE


///
/// The base class for all game windows. When subclassing,
/// it is advised (but not necessary) to implement all the
/// virtual functions that inform about important events.
/// Reimplement sizeHint() to neatly specify the window size.
///
/// \class Window
/// \author Nicolas Kogler
/// \date February 8, 2017
///
class CRANBERRY_EXPORT Window : public QOpenGLWindow
{
public:

    CRANBERRY_DISABLE_COPY(Window)
    CRANBERRY_DISABLE_MOVE(Window)

    ///
    /// Initializes a new instance of Window.
    ///
    /// \param parent The parent window.
    ///
    Window(Window* parent = nullptr);

    ///
    /// Destructor
    ///
    ~Window();


    ///
    /// Determines whether this OpenGL window is initialized.
    /// It is considered to be initialized if initializeGL()
    /// succeeds.
    ///
    /// \returns true if initialized.
    ///
    bool isInitialized() const;

    ///
    /// Determines whether this OpenGL window is active.
    /// It is considered to be active if it has the input focus.
    ///
    /// \returns true if active.
    ///
    bool isActive() const;

    ///
    /// Determines whether this window uses vertical
    /// synchronisation.
    ///
    /// \returns true if it uses vsync.
    ///
    bool hasVerticalSync() const;

    ///
    /// Retrieves the clear color of this OpenGL window.
    /// By default, this value is CornFlowerBlue.
    ///
    /// \returns the clear color.
    ///
    const QColor& clearColor() const;

    ///
    /// Retrieves the OpenGL functions of this window.
    /// This returns a nullptr if not yet initialized.
    ///
    /// \returns a reference to the OpenGL functions.
    ///
    QOpenGLFunctions* functions() const;


    ///
    /// Enables vertical sync by setting the swap interval
    /// to 1. FPS will be capped and that results in a
    /// smooth game experience.
    ///
    /// \param enable True to enable vertical sync.
    ///
    void setVerticalSync(bool enable);

    ///
    /// Specifies the clear color for this OpenGL window.
    /// If the OpenGL window is already created, it will
    /// directly call glClearColor(). Copies the color into
    /// a private member and changes it later otherwise.
    ///
    /// \param color New clear color.
    ///
    void setClearColor(const QColor& color);


    ///
    /// Retrieves the currently active window. If no
    /// window has been shown yet, a nullptr is returned.
    ///
    /// \returns the currently active window.
    ///
    static Window* activeWindow();

    ///
    /// Retrieves the game time since the first window
    /// that was created within this application.
    ///
    /// \returns the current game time.
    ///
    static GameTime* currentTime();


protected:

    ///
    /// Initializes all the resources for this window.
    ///
    void initializeGL() override;

    ///
    /// Renders the contents of the entire game.
    ///
    void paintGL() override;

    ///
    /// Fires when the mouse pointer is moved.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseMoveEvent(QMouseEvent* event) override;

    ///
    /// Fires when a mouse button is pressed.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mousePressEvent(QMouseEvent* event) override;

    ///
    /// Fires when a mouse button was just released.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseReleaseEvent(QMouseEvent* event) override;

    ///
    /// Fires when a mouse button was double clicked.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    ///
    /// Fires when a key button is pressed.
    ///
    /// \param event Contains information about the key event.
    ///
    void keyPressEvent(QKeyEvent* event) override;

    ///
    /// Fires when a key button was just released.
    ///
    /// \param event Contains information about the key event.
    ///
    void keyReleaseEvent(QKeyEvent* event) override;

    ///
    /// Fires when the mouse wheel is scrolled.
    ///
    /// \param event Contains information about the scroll event.
    ///
    void wheelEvent(QWheelEvent* event) override;

    ///
    /// Fires when the user interacted with the touch
    /// screen on mobile and other touch screen devices.
    ///
    /// \param event Contains information about the touch event.
    ///
    void touchEvent(QTouchEvent* event) override;

    ///
    /// Fires when the user resizes the window.
    ///
    /// \param event Contains the old and new size.
    ///
    void resizeEvent(QResizeEvent* event) override;

    ///
    /// Fires when the window is focused. Activates the
    /// rendering/updating timer and resumes playing sounds.
    ///
    /// \param event Contains no useful information for us.
    ///
    void focusInEvent(QFocusEvent* event) override;

    ///
    /// Fires when the window is defocused. Deactivates the
    /// rendering/updating timer and pauses playing sounds.
    ///
    /// \param event Contains no useful information for us.
    ///
    void focusOutEvent(QFocusEvent* event) override;

    ///
    /// Processes the close event in order to free our
    /// OpenGL resources in the parent class.
    ///
    /// \param event Holds the event type.
    ///
    bool event(QEvent* event) override;


    // Functions to override
    virtual void init();
    virtual void exit();
    virtual void update(const GameTime& time);
    virtual void render();
    virtual void mouseMove(const MouseMoveEvent& event);
    virtual void mouseDown(const MouseState& mouse);
    virtual void mouseUp(const MouseReleaseEvent& event);
    virtual void mouseDoubleClick(const MouseReleaseEvent& event);
    virtual void keyDown(const KeyboardState& keyboard);
    virtual void keyUp(const KeyReleaseEvent& event);
    virtual void keyChar(const QString& text);
    virtual void scroll(const QWheelEvent& event);
    virtual void touch(const QTouchEvent& event);
    virtual void windowResize(const QSize& old);
    virtual void windowActivate();
    virtual void windowDeactivate();


private:

    // Members
    MouseState          m_mouseState;
    KeyboardState       m_keyState;
    QOpenGLFunctions    m_funcs;
    QPoint              m_lastCursorPos;
    QColor              m_clearColor;
    bool                m_hasVsync;
    bool                m_isInit;
    bool                m_isActive;
    int                 m_btnCount;
    int                 m_keyCount;

    // Metadata
    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_WINDOW_HPP

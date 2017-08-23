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
#ifndef CRANBERRY_WINDOW_WINDOW_HPP
#define CRANBERRY_WINDOW_WINDOW_HPP


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
#include <QObject>

// Forward declarations
CRANBERRY_FORWARD_Q(QSurface)
CRANBERRY_FORWARD_Q(QOpenGLContext)
CRANBERRY_FORWARD_Q(QOpenGLFunctions)
CRANBERRY_FORWARD_C(GuiManager)
CRANBERRY_FORWARD_C(OpenGLShader)
CRANBERRY_FORWARD_C(RenderBase)
CRANBERRY_FORWARD_P(WindowPrivate)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a game window with useful functions to override.
///
/// \class Window
/// \author Nicolas Kogler
/// \date June 23, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_WINDOW_EXPORT Window : public QObject
{
public:

    CRANBERRY_DECLARE_CTOR(Window)
    CRANBERRY_DECLARE_DTOR(Window)
    CRANBERRY_DISABLE_COPY(Window)
    CRANBERRY_DISABLE_MOVE(Window)

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
    /// Retrieves the width of this window.
    ///
    /// \returns the width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float width() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the height of this window.
    ///
    /// \returns the height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float height() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the size of this window.
    ///
    /// \returns the size.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QSize size() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the OpenGL functions of the window's context.
    ///
    /// \returns the OpenGL functions.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLFunctions* functions() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the window's OpenGL context.
    ///
    /// \returns the OpenGL context.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLContext* context() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the window's surface.
    ///
    /// \returns the surface.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QSurface* surface() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Returns the default VAO for this render target.
    ///
    /// \returns this render target's VAO.
    ///
    ////////////////////////////////////////////////////////////////////////////
    uint vao() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Restores all OpenGL settings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void restoreOpenGLSettings();

    ////////////////////////////////////////////////////////////////////////////
    /// Makes this context current.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void makeCurrent();

    ////////////////////////////////////////////////////////////////////////////
    /// Shows the debug overlay for the given RenderBase object.
    ///
    /// \param obj Object to show debug information from.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void showDebugOverlay(RenderBase* obj);

    ////////////////////////////////////////////////////////////////////////////
    /// Hides any currently active debug overlay. Equivalent to calling
    /// Window::showDebugOverlay(nullptr).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void hideDebugOverlay();

    ////////////////////////////////////////////////////////////////////////////
    /// Specify the settings for this window. Call this before you call start().
    ///
    /// \param settings New window settings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSettings(const WindowSettings& settings);

    ////////////////////////////////////////////////////////////////////////////
    /// Takes a screenshot of this window and saves it to \p path.
    ///
    /// \param path Path to save screenshot to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void saveScreenshot(const QString& path);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the currently active window.
    ///
    /// \returns the active window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static Window* activeWindow();


protected overridable:

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
    //virtual void onScrolled(const QWheelEvent&) { }
    //virtual void onTouched(const QTouchEvent&) { }
    virtual void onWindowResized(const QSize&) { }
    virtual void onWindowActivated() { }
    virtual void onWindowDeactivated() { }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void registerQmlWindow(GuiManager*);
    void unregisterQmlWindow(GuiManager*);

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    priv::WindowPrivate* m_priv;

    friend class Game;
    friend class GuiManager;
    friend class priv::WindowPrivate;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Window
/// \ingroup Window
///
/// This class is the base for all custom game windows. Override all the methods
/// you need for your game and put your game logic inside them.
///
/// \code
/// class MyWindow final : public cran::Window
/// {
/// protected:
///
///     void onInit() override;
///     void onExit() override;
///     void onRender() override;
///
///     ...
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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


#ifndef CRANBERRY_CBWINDOW_HPP
#define CRANBERRY_CBWINDOW_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>
#include <Cranberry/Window/Events/cbKeyUpEvent.hpp>
#include <Cranberry/Window/Events/cbKeyboardState.hpp>
#include <Cranberry/Window/Events/cbMouseMoveEvent.hpp>
#include <Cranberry/Window/Events/cbMouseButtonEvent.hpp>

// Qt headers
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QColor>


CRANBERRY_BEGIN_NAMESPACE


///
/// This is the base class for all game windows.
/// When subclassing it, all the pure virtual on*()
/// functions and others need to be implemented.
///
/// \class cbWindow
/// \author Nicolas Kogler
/// \date January 29th, 2017
///
class CRANBERRY_EXPORT cbWindow :
        public QOpenGLWindow,
        protected QOpenGLFunctions
{
public:

    ///
    /// Constructor
    ///
    /// \param parent The parent window. Should be
    ///        a null pointer for the main window.
    ///
    cbWindow(cbWindow* parent = nullptr);

    ///
    /// Copy constructor (disabled)
    ///
    /// \param other The other cbWindow instance.
    ///
    cbWindow(const cbWindow& other) = delete;

    ///
    /// Move constructor (disabled)
    ///
    /// \param other The other cbWindow instance.
    ///
    cbWindow(cbWindow&& other) = delete;

    ///
    /// Assignment operator (disabled)
    ///
    /// \param other The other cbWindow instance.
    ///
    cbWindow& operator=(const cbWindow& other) = delete;

    ///
    /// Destructor
    ///
    ~cbWindow() = default;


    ///
    /// Determines whether this OpenGL window is initialized.
    ///
    /// \returns true if it is initialized.
    ///
    bool isInitialized() const;

    ///
    /// Determines whether this OpenGL window is active.
    ///
    /// \returns true if it is active.
    ///
    bool isActive() const;

    ///
    /// Retrieves the clear color of this OpenGL window.
    ///
    /// \returns the clear color.
    ///
    const QColor& clearColor() const;


    ///
    /// Specifies the clear color for this window.
    ///
    /// \param color The new clear color.
    ///
    void setClearColor(const QColor& color);

    ///
    /// Enables vertical sync, if desired. If vertical sync
    /// is enabled, FPS will be capped to the refresh rate
    /// of your display device (usually 60) and results in
    /// a smooth gameplay experience.
    ///
    void enableVerticalSync(bool enable);


protected:

    ///
    /// Initializes all the resources for this game window.
    /// Calls the pure virtual function cbWindow::onInit().
    ///
    void initializeGL() override;

    ///
    /// Issues a repaint of all game content.
    /// Calls the pure virtual function cbWindow::onRender().
    ///
    void paintGL() override;

    ///
    /// Is fired when the mouse pointer is moved.
    /// Calls the pure virtual function cbWindow::onMouseMove()
    /// but removes unnecessary information from the QMouseEvent.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseMoveEvent(QMouseEvent* event) override;

    ///
    /// Is fired when a mouse button is pressed.
    /// Calls the pure virtual function cbWindow::onMouseDown()
    /// but removes unnecessary information from the QMouseEvent.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mousePressEvent(QMouseEvent* event) override;

    ///
    /// Is fired when a mouse button was released.
    /// Calls the pure virtual function cbWindow::onMouseUp()
    /// but removes unnecessary information from the QMouseEvent.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseReleaseEvent(QMouseEvent* event) override;

    ///
    /// Is fired when a mouse button was double clicked.
    /// Calls the pure virtual function cbWindow::onMouseDoubleClick()
    /// but removes unnecessary information from the QMouseEvent.
    ///
    /// \param event Contains information about the mouse event.
    ///
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    ///
    /// Is fired when a key button is pressed.
    /// Calls the pure virtual function cbWindow::onKeyDown()
    /// but removes unnecessary information from the QKeyEvent.
    ///
    /// \param event Contains information about the key event.
    ///
    void keyPressEvent(QKeyEvent* event) override;

    ///
    /// Is fired when a key button was released.
    /// Calls the pure virtual function cbWindow::onKeyUp()
    /// but removes unnecessary information from the QKeyEvent.
    ///
    /// \param event Contains information about the key event.
    ///
    void keyReleaseEvent(QKeyEvent* event) override;

    ///
    /// Is fired when the mouse wheel is scrolled.
    /// Calls the pure virtual function cbWindow::onScroll().
    ///
    /// \param event Contains information about the scroll event.
    ///
    void wheelEvent(QWheelEvent* event) override;

    ///
    /// Is fired when the user interacted with the touch
    /// screen on mobile and other touch screen devices.
    /// Calls the pure virtual function cbWindow::onTouch().
    ///
    /// \param event Contains information about the touch event.
    ///
    void touchEvent(QTouchEvent* event) override;

    ///
    /// Is fired when the user resizes the window.
    ///
    /// \param event Contains the old and new size.
    ///
    void resizeEvent(QResizeEvent* event) override;

    ///
    /// Is fired when the window is focused. Activates the
    /// rendering/updating timer and resumes playing sounds.
    ///
    /// \param event Contains no useful information for us.
    ///
    void focusInEvent(QFocusEvent* event) override;

    ///
    /// Is fired when the window is defocused. Deactivates the
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


    ///
    /// Should contain initialization of cranberry objects.
    ///
    virtual void onInit() = 0;

    ///
    /// Should contain calls to update() of cranberry objects.
    ///
    virtual void onUpdate() = 0;

    ///
    /// Should contain calls to render() of cranberry objects.
    ///
    virtual void onRender() = 0;

    ///
    /// Should contain code to free cranberry objects.
    ///
    virtual void onExit() = 0;

    ///
    /// Should contain logic to reorder the GUI or cranberry objects.
    ///
    /// \param size The new size of the window.
    ///
    virtual void onResize(const QSize& size) = 0;

    ///
    /// Should contain calls to onMouseMove() of cranberry objects.
    ///
    /// \param event Contains new position and delta, calculated
    ///        from the new cursor position minus the last one.
    ///
    virtual void onMouseMove(const cbMouseMoveEvent& event) = 0;

    ///
    /// Should contain calls to onMouseDown() of cranberry objects.
    ///
    /// \param event Contains all the buttons that are pressed.
    ///
    virtual void onMouseDown(const cbMouseButtonEvent& event) = 0;

    ///
    /// Should contain calls to onMouseUp() of cranberry objects.
    ///
    /// \param event Contains all the recently released buttons.
    ///
    virtual void onMouseUp(const cbMouseButtonEvent& event) = 0;

    ///
    /// Should contain calls to onMouseDoubleClick() of cranberry objects.
    ///
    /// \param event Contains the button that was double clicked.
    ///
    virtual void onMouseDoubleClick(const cbMouseButtonEvent& event) = 0;

    ///
    /// Should contain calls to onKeyDown() of cranberry objects.
    ///
    /// \param event Contains the key that was pressed.
    ///
    virtual void onKeyDown(const cbKeyboardState& state) = 0;

    ///
    /// Should contain calls to onKeyUp() of cranberry objects.
    ///
    /// \param event Contains the key that was released.
    ///
    virtual void onKeyUp(const cbKeyUpEvent& event) = 0;

    ///
    /// Should contain calls to onKeyChar() of cranberry objects.
    ///
    /// \param str Contains the text of the pressed key.
    ///
    virtual void onKeyChar(const QString& str) = 0;

    ///
    /// Should contain calls to onScroll() of cranberry objects.
    ///
    /// \param event Contains the amount of scrolled pixels or degrees.
    ///
    virtual void onScroll(const QWheelEvent& event) = 0;

    ///
    /// Should contain calls to onTouch() of cranberry objects.
    ///
    /// \param event Contains the original QTouchEvent.
    ///
    virtual void onTouch(const QTouchEvent& event) = 0;

    ///
    /// This function is called when the window received focus.
    ///
    virtual void onActivate() = 0;

    ///
    /// This function is called when the user removed focus
    /// from this window.
    ///
    virtual void onDeactivate() = 0;


private:

    // Members
    QPoint      m_lastCursorPos;
    QColor      m_clearColor;
    bool        m_isInitialized;
    bool        m_isActive;
    int         m_keyCount;

    cbKeyboardState m_keyState;

    // Metadata
    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBWINDOW_HPP

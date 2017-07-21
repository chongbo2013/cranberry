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
#ifndef CRANBERRY_WINDOW_WINDOWSETTINGS_HPP
#define CRANBERRY_WINDOW_WINDOWSETTINGS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QColor>
#include <QPoint>
#include <QSize>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds several window settings that can be applied by calling
/// Window::useSettings(mySettings).
///
/// \class WindowSettings
/// \author Nicolas Kogler
/// \date June 11, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_WINDOW_EXPORT WindowSettings
{
public:

    CRANBERRY_DEFAULT_DTOR(WindowSettings)
    CRANBERRY_DEFAULT_COPY(WindowSettings)
    CRANBERRY_DEFAULT_MOVE(WindowSettings)

    ////////////////////////////////////////////////////////////////////////////
    /// Sets all members to their default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    WindowSettings();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this window is resizable. By default, this value is
    /// \em false.
    ///
    /// \returns true if the window is resizable.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isResizable() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the app should be run in fullscreen mode. By default,
    /// this value is \em false.
    ///
    /// \returns true if requesting fullscreen mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isFullscreen() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the window is double buffered. By default, this is
    /// \em true, since it offers a smoother gaming experience.
    ///
    /// \returns true if requesting double buffer mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isDoubleBuffered() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the app uses vertical synchronisation. By default,
    /// this value is \em false, since VSync is not always desired.
    ///
    /// \return true if requesting VSync.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool useVerticalSync() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the title of the window. By default, this value is random.
    ///
    /// \returns the window title.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& title() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the size of the window. By default, this value is 800x600.
    ///
    /// \returns the window size.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QSize& size() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the position of the window. By default, this value is the
    /// default spawning location of your OS. If you want to set a relative
    /// position, use WindowSettings::setPosition(Qt::Alignment).
    ///
    /// \returns the window position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QPoint& position() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the clear color of this window. By default, this value is the
    /// CornflowerBlue color.
    ///
    /// \returns the clear color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QColor& clearColor() const;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies whether the window should be resizable.
    ///
    /// \param value True if the window should be resizable.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setResizable(bool value);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies whether the window should be fullscreen.
    ///
    /// \param value True if the window should be fullscreen.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setFullscreen(bool value);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies whether the window should be double buffered.
    ///
    /// \param value True if the window should be double buffered.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setDoubleBuffered(bool value);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies whether or not to enable vertical sync.
    ///
    /// \param value True to enable vertical sync.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setVerticalSync(bool value);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the window's title.
    ///
    /// \param title The title of the window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTitle(const QString& title);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the window's size.
    ///
    /// \param size The size of the window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSize(const QSize& size);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the window's absolute position.
    ///
    /// \param pos Absolute window position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setPosition(const QPoint& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the window's relative position. Examples: Qt::AlignCenter
    /// centers the window on the screen; Qt::AlignLeft | Qt::AlignVCenter puts
    /// the window to the very left of the screen and centers it vertically.
    ///
    /// \warning WindowSettings::setSize must be called beforehand.
    /// \param relative One or more Qt::Alignment flags.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setPosition(Qt::Alignment relative);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the clear color of the window.
    ///
    /// \param color Clear color of the window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setClearColor(const QColor& color);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    bool    m_isResizable;      ///< Able to resize window?
    bool    m_isFullscreen;     ///< Running fullscreen?
    bool    m_isDoubleBuffered; ///< Double-buffering window?
    bool    m_useVerticalSync;  ///< Use vertical synchronisation?
    QString m_title;            ///< Window title
    QSize   m_size;             ///< Window size
    QPoint  m_pos;              ///< Window position
    QColor  m_clearColor;       ///< OpenGL clear color
};


////////////////////////////////////////////////////////////////////////////////
/// \class WindowSettings
/// \ingroup Window
///
/// This is a convenient class to specify a lot of settings at once. You can
/// either use it in the constructor of your custom window or outside the class
/// before the window is shown (i.e. the game is run).
///
/// \code
/// WindowSettings settings;
/// settings.setResizable(false);
/// settings.setVerticalSync(true);
/// settings.setTitle("Hello world");
/// settings.setSize(600, 400);
/// settings.setPosition(Qt::AlignCenter);
///
/// MyWindow* window = new MyWindow;
/// window->setSettings(settings);
/// game.run(window);
///
/// return game.exitCode();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

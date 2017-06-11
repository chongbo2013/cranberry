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


// Cranberry headers
#include <Cranberry/System/Random.hpp>
#include <Cranberry/Window/WindowSettings.hpp>

// Qt headers
#include <QDesktopWidget>


CRANBERRY_USING_NAMESPACE


WindowSettings::WindowSettings()
    : m_isResizable(true)
    , m_isFullscreen(false)
    , m_isDoubleBuffered(true)
    , m_useVerticalSync(false)
    , m_size(800, 600)
    , m_pos(-1, -1)
    , m_clearColor(100, 149, 237)
{
    m_title = Random().nextString(8);
}


bool WindowSettings::isResizable() const
{
    return m_isResizable;
}


bool WindowSettings::isFullscreen() const
{
    return m_isFullscreen;
}


bool WindowSettings::isDoubleBuffered() const
{
    return m_isDoubleBuffered;
}


bool WindowSettings::useVerticalSync() const
{
    return m_useVerticalSync;
}


const QSize& WindowSettings::size() const
{
    return m_size;
}


const QPoint& WindowSettings::position() const
{
    return m_pos;
}


const QColor& WindowSettings::clearColor() const
{
    return m_clearColor;
}


void WindowSettings::setResizable(bool value)
{
    m_isResizable = value;
}


void WindowSettings::setFullscreen(bool value)
{
    m_isFullscreen = value;
}


void WindowSettings::setDoubleBuffered(bool value)
{
    m_isDoubleBuffered = value;
}


void WindowSettings::setVerticalSync(bool value)
{
    m_useVerticalSync = value;
}


void WindowSettings::setSize(const QSize& size)
{
    m_size = size;
}


void WindowSettings::setPosition(const QPoint& pos)
{
    m_pos = pos;
}


void WindowSettings::setPosition(Qt::Alignment relative)
{
    if (relative == Qt::AlignAbsolute) return;
    int x = 0, y = 0;

    // Retrieves size of screen.
    QDesktopWidget dw;
    QRect screen = dw.availableGeometry(dw.primaryScreen());

    // Determines the absolute position from the relative one.
    if ((relative & Qt::AlignRight) != 0) x = screen.width() - m_size.width();
    if ((relative & Qt::AlignBottom) != 0) y = screen.height() - m_size.height();
    if ((relative & Qt::AlignHCenter) != 0) x = screen.width() / 2 - m_size.width() / 2;
    if ((relative & Qt::AlignVCenter) != 0) y = screen.height() / 2 - m_size.height() / 2;

    m_pos = QPoint(x, y);
}


void WindowSettings::setClearColor(const QColor& color)
{
    m_clearColor = color;
}

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


// Example headers
#include <GameWindow.hpp>

// Macroes
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)
#define CENTER_X(w)   (WINDOW_WIDTH / 2 - w / 2)
#define CENTER_Y(h)   (WINDOW_HEIGHT / 2 - h / 2)
#define CENTER(w, h)  (QVector2D(CENTER_X(w), CENTER_Y(h)))


GameWindow::GameWindow()
    : Window()
    , m_hexagon(nullptr)
    , m_gui(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("07_UserInterface");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


void GameWindow::onInit()
{
    // m_hexagon
    m_hexagon = new Polygon;
    m_hexagon->create(WINDOW_WIDTH / 2, 6, this);
    m_hexagon->setColor(QColor(183, 18, 95));
    m_hexagon->setShapeFilled(true);
    m_hexagon->setRotateMode(RotateForever);
    m_hexagon->startRotating();

    // m_gui
    m_gui = new GuiManager;
    m_gui->create("qrc:/gui.qml", this);
}


void GameWindow::onExit()
{
    // m_hexagon
    if (m_hexagon != nullptr)
    {
        delete m_hexagon;
        m_hexagon = nullptr;
    }

    // m_gui
    if (m_gui != nullptr)
    {
        delete m_gui;
        m_gui = nullptr;
    }
}


void GameWindow::onCrash()
{
    onExit();
}


void GameWindow::onUpdate(const GameTime& time)
{
    m_hexagon->update(time);
    m_gui->update(time);
}


void GameWindow::onRender()
{
    m_hexagon->render();
    m_gui->render();
}

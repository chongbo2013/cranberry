////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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

// Qt headers
#include <QDebug>

// Constants
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)


GameWindow::GameWindow() : Window()
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("01_CreateWindow");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


void GameWindow::onInit()
{
    qDebug() << "GameWindow initialized!";
}


void GameWindow::onExit()
{
    qDebug() << "GameWindow destroyed!";
    qDebug() << "Purposely throwing exception to test onCrash().";
    throw 0; // Unhandled exception
}


void GameWindow::onCrash()
{
    qDebug() << "GameWindow onCrash() called!";
}


void GameWindow::onUpdate(const GameTime&)
{
}


void GameWindow::onRender()
{
}

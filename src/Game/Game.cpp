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


// Cranberry headers
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QGuiApplication>


CRANBERRY_BEGIN_NAMESPACE


///
/// The actual Qt GUI application.
///
QGuiApplication* g_application;


Game::Game(int& argc, char* argv[])
{
    // Creates the GUI application.
    g_application = new QGuiApplication(argc, argv);
}


Game::~Game()
{
    for (auto* window : m_windows)
        delete window;
}


void Game::addWindow(Window* window)
{
    if (window != nullptr)
    {
        m_windows.push_back(window);
        window->show();
    }
    else
    {
        printf("Cranberry: Given window is invalid!");
    }
}


void Game::removeWindow(Window* window)
{
    if (window != nullptr)
    {
        // Attempts to find the window and erases it.
        for (size_t i = 0; i < m_windows.size(); i++)
        {
            if (m_windows.at(i) == window)
                m_windows.erase(m_windows.begin() + i);
        }
    }
    else
    {
        printf("Cranberry: Given window is invalid!");
    }
}


int Game::run(Window* mainWindow)
{
    addWindow(mainWindow);
    return g_application->exec();
}


CRANBERRY_END_NAMESPACE

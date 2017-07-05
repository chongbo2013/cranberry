﻿////////////////////////////////////////////////////////////////////////////////
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
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QApplication>
#include <QMessageBox>

// Standard headers
#include <csignal>
#include <exception>


CRANBERRY_USING_NAMESPACE


CRANBERRY_GLOBAL_VAR_A(QApplication*, g_application, nullptr)
CRANBERRY_GLOBAL_VAR_A(Game*, g_instance, nullptr)


void cranberryGlobalSignalHandler(int)
{
    // Shows a messagebox with the error.
    QMessageBox box;
    box.setWindowTitle("Cranberry");
    box.setIcon(QMessageBox::Critical);
    box.setStandardButtons(QMessageBox::Close);
    box.setText("A fatal error occured, trying to save game data.");
    box.exec();

    // Terminates the game.
    Game::instance()->exit(CRANBERRY_EXIT_UNHANDLED);
}


Game::Game(int& argc, char* argv[])
{
    // Creates the GUI application, if not already.
    if (g_application == nullptr)
    {
        g_application = new QApplication(argc, argv);
    }

    g_instance = this;

    // Registers the global signal handlers.
    std::signal(SIGABRT, &cranberryGlobalSignalHandler);
    std::signal(SIGSEGV, &cranberryGlobalSignalHandler);
    std::signal(SIGTERM, &cranberryGlobalSignalHandler);
    std::signal(SIGFPE, &cranberryGlobalSignalHandler);
    std::signal(SIGILL, &cranberryGlobalSignalHandler);
}


Game::~Game()
{
    if (m_isRunning)
    {
        g_application->closeAllWindows();
        g_application->exit(CRANBERRY_EXIT_NORMAL);
        m_isRunning = false;
    }

    delete g_application;
}


bool Game::addWindow(Window* window)
{
    if (window != nullptr)
    {
        if (std::find(m_windows.begin(), m_windows.end(), window) == m_windows.end())
        {
            m_windows.push_back(window);
            window->show();
            return true;
        }
    }

    return false;
}


bool Game::removeWindow(Window* window)
{
    if (window != nullptr)
    {
        auto it = m_windows.end();
        if ((it = std::find(m_windows.begin(), m_windows.end(), window)) != m_windows.end())
        {
            m_windows.erase(it);
            window->hide();
            return true;
        }
    }

    return false;
}


int Game::run(Window* mainWindow)
{
    addWindow(mainWindow);
    m_isRunning = true;

    return g_application->exec();
}


void Game::exit(int exitCode)
{
    // Informs all windows about the potential crash.
    if (exitCode != 0)
    {
        for (Window* window : m_windows)
        {
            window->onCrash();
        }
    }

    m_windows.clear();

    if (m_isRunning)
    {
        g_application->closeAllWindows();
        g_application->exit(exitCode);
        m_isRunning = false;
    }
}


Game* Game::instance()
{
    return g_instance;
}
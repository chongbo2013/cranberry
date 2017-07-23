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


// Cranberry headers
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/Window/WindowPrivate.hpp>

// Qt headers
#include <QApplication>


CRANBERRY_USING_NAMESPACE


// Global variables
CRANBERRY_GLOBAL_VAR_A(QApplication*, g_application, nullptr)
CRANBERRY_GLOBAL_VAR_A(Game*,         g_instance,    nullptr)


Game::Game(int& argc, char* argv[])
{
    // Creates the GUI application, if not already.
    if (g_application == nullptr)
    {
        QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
        QApplication::setAttribute(Qt::AA_Use96Dpi);

        g_application = new QApplication(argc, argv);
    }

    g_instance = this;
}


Game::~Game()
{
    if (m_isRunning)
    {
        g_application->closeAllWindows();
        g_application->exit(CRANBERRY_EXIT_NORMAL);
    }

    delete g_application;
}


bool Game::addWindow(Window* window)
{
    if (window != nullptr)
    {
        if (!m_windows.contains(window))
        {
            m_windows.append(window);
            window->m_priv->show();
            return true;
        }
    }

    return false;
}


bool Game::removeWindow(Window* window)
{
    if (window != nullptr)
    {
        if (m_windows.contains(window))
        {
            m_windows.removeOne(window);
            window->m_priv->hide();
            return true;
        }
    }

    return false;
}


int Game::run(Window* mainWindow)
{
    addWindow(mainWindow);
    mainWindow->m_priv->m_isMainWindow = true;
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

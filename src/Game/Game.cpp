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
#include <QMessageBox>

// Standard headers
#include <exception>
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


///
/// The actual Qt GUI application.
///
QGuiApplication* g_application;

///
/// The unique game instance.
///
Game* g_instance;


///
/// The global exception handler for unhandled exceptions.
/// Issues a termination of the game while destroying all
/// the OpenGL resources still occupied by the application.
///
void cranberryExceptionHandler()
{
    // Attempts to get exception text.
    std::string cause = "Unknown exception";
    try
    {
        auto* eptr = std::current_exception();
        if (eptr)
            std::rethrow_exception(eptr);
    }
    catch (const std::exception& e)
    {
        cause = e.what();
    }


    if (Window::activeWindow() != nullptr)
    {
        // Shows the messagebox.
        QMessageBox box(Window::activeWindow());
        box.setWindowTitle("Cranberry Error");
        box.setIcon(QMessageBox::Critical);
        box.setStandardButtons(QMessageBox::Close);
        box.setText(cause.data());
        box.exec();
    }

    // Terminates the game.
    Game::instance()->exit(CRANBERRY_EXIT_UNHANDLED);
}


Game::Game(int& argc, char* argv[])
{
    // Creates the GUI application.
    g_application = new QGuiApplication(argc, argv);
    g_instance = this;

    // Registers the global exception handler.
    std::set_terminate(cranberryExceptionHandler);
}


Game::~Game()
{
    for (auto* window : m_windows)
    {
        // Last resort OpenGL resource destruction.
        window->close();
        delete window;
    }
}


void Game::addWindow(Window* window)
{
    if (window != nullptr)
    {
        if (std::find(m_windows.begin(), m_windows.end(), window) == m_windows.end())
            m_windows.push_back(window);

        window->show();
    }
    else
    {
        std::cout << "Cranberry: Given window is invalid!" << std::endl;
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
        std::cout << "Cranberry: Given window is invalid!" << std::endl;
    }
}


int Game::run(Window* mainWindow)
{
    addWindow(mainWindow);
    return g_application->exec();
}


void Game::exit(int exitCode)
{
    for (auto* window : m_windows)
    {
        if (exitCode)
            window->crash();

        // Destructs all OpenGL resources.
        window->close();
        delete window;
    }

    m_windows.clear();
    g_application->exit(exitCode);
}


Game* Game::instance()
{
    return g_instance;
}


CRANBERRY_END_NAMESPACE

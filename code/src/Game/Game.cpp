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
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QApplication>
#include <QDebug>
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
        QApplication::setAttribute(Qt::AA_Use96Dpi);
        g_application = new QApplication(argc, argv);
    }

    g_instance = this;

    // Registers the global signal handlers.
    std::signal(SIGABRT, &cranberryGlobalSignalHandler);
    std::signal(SIGSEGV, &cranberryGlobalSignalHandler);
    std::signal(SIGTERM, &cranberryGlobalSignalHandler);
    std::signal(SIGFPE, &cranberryGlobalSignalHandler);
    std::signal(SIGILL, &cranberryGlobalSignalHandler);

#ifdef QT_DEBUG
    cranberryLogo();
#endif
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
        if (!m_windows.contains(window))
        {
            m_windows.append(window);
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
        if (m_windows.contains(window))
        {
            m_windows.removeOne(window);
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


#ifdef QT_DEBUG
void Game::cranberryLogo()
{
    qDebug() << "-------------------------------------------------------";
    qDebug() << "Cranberry - C++ game engine based on the Qt5 framework.";
    qDebug() << "Copyright (C) 2017 Nicolas Kogler";
    qDebug() << "License - Lesser General Public License (LGPL) 3.0";
    qDebug() << "Version" << CRANBERRY_VERSION;
    qDebug() << "\n";
    qDebug() << "                       d888P";
    qDebug() << "             d8b d8888P:::P";
    qDebug() << "            d:::888b::::::P";
    qDebug() << "           d:::dP8888b:d8P";
    qDebug() << "          d:::dP 88b  Yb   .d8888b.";
    qDebug() << "         d::::P  88Yb  Yb .P::::::Y8b";
    qDebug() << "         8:::8   88`Yb  YbP::::::::::b";
    qDebug() << "         8:::P   88 `8   8!:::::::::::b";
    qDebug() << "         8:dP    88  Yb d!!!::::::::::8";
    qDebug() << "         8P    ..88   Yb8!!!::::::::::P";
    qDebug() << "          .d8:::::Yb  d888VKb:!:!::!:8";
    qDebug() << "         d::::::::::dP:::::::::b!!!!8";
    qDebug() << "        8!!::::::::P::::::::::::b!8P";
    qDebug() << "        8:!!::::::d::::::::::::::b";
    qDebug() << "        8:!:::::::8!:::::::::::::8";
    qDebug() << "        8:!!!:::::8!:::::::::::::8";
    qDebug() << "        Yb:!!:::::8!!::::::::::::8";
    qDebug() << "         8b:!!!:!!8!!!:!:::::!!:dP";
    qDebug() << "          `8b:!!!:Yb!!!!:::::!d88";
    qDebug() << "              \"\"\"  Y88!!!!!!!d8P";
    qDebug() << "                      \"\"\"\"\"\"\"\n\n";
}
#endif

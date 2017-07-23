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
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Game/Game.hpp>
#include <Cranberry/Game/GamePrivate.hpp>

// Qt headers
#include <QDebug>
#include <QMessageBox>

// Platform headers
#ifdef Q_OS_WIN
    #include <Windows.h>
    #include <Cranberry/System/StackWalkers/StackWalkerWin32.hpp>
#endif

// Standard headers
#include <csignal>
#include <exception>


CRANBERRY_USING_NAMESPACE


// Constants and globals
CRANBERRY_GLOBAL_VAR_A(priv::StackWalker*, g_walker, nullptr)
CRANBERRY_CONST_VAR(QString, e_01,
    "The operating system requested a termination "
    "of this program while the %0 signal was emitted. "
    "Please forward the stack trace to the developers. "
    "Cranberry tries to save any game data now.")


priv::GamePrivate::GamePrivate()
{
    // Windows: Suppress message boxes.
    #ifdef Q_OS_WIN
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    #endif

    // Registers the global signal handlers.
    std::signal(SIGABRT, &signalHandler);
    std::signal(SIGSEGV, &signalHandler);
    std::signal(SIGTERM, &signalHandler);
    std::signal(SIGFPE, &signalHandler);
    std::signal(SIGILL, &signalHandler);

    // Initializes the platform-dependent stack walker.
    if (g_walker == nullptr)
    {
    #if defined(Q_OS_WIN)
        g_walker = new StackWalkerWin32;
    #elif defined(Q_OS_UNIX)
        g_walker = new StackWalkerUnix;
    #else
        g_walker = new StackWalker;
    #endif
    }

    // Prints the cranberry logo.
    if_debug(printLogo());
}


priv::GamePrivate::~GamePrivate()
{
    if (g_walker != nullptr)
    {
        delete g_walker;
        g_walker = nullptr;
    }
}


void priv::GamePrivate::signalHandler(int signal)
{
    const char* type = "Unknown";
    switch (signal)
    {
    case SIGABRT:
        type = "SIGABRT";
        break;

    case SIGSEGV:
        type = "SIGSEGV";
        break;

    case SIGTERM:
        type = "SIGTERM";
        break;

    case SIGFPE:
        type = "SIGFPE";
        break;

    case SIGILL:
        type = "SIGILL";
        break;
    }

    // Shows the error message.
    QMessageBox box;
    box.setWindowTitle("Cranberry - Signal received");
    box.setIcon(QMessageBox::Critical);
    box.setStandardButtons(QMessageBox::Close);
    box.setText(e_01.arg(QString(type)));
    box.setDetailedText(g_walker->stackTrace());
    box.exec();

    // Exits the game.
    Game::instance()->exit(CRANBERRY_EXIT_UNHANDLED);
}


if_debug
(
void priv::GamePrivate::printLogo()
{
    qDebug() << "----------------------------------------------------------";
    qDebug() << "Cranberry - C++ game engine based on the Qt 5.8 framework.";
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
)

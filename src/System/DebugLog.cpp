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
#include <Cranberry/System/DebugLog.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QMessageBox>

// Standard headers
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


void DebugLog::write(int32_t line,
                     std::string file,
                     std::string func,
                     std::string msg,
                     bool isError)
{
#ifdef QT_DEBUG
    std::string type = (isError) ? "an error" : "a warning";
    std::cout << "Cranberry threw " << type     << std::endl
              << "in file \"" << file << "\""   << std::endl
              << "in func \"" << func << "\""   << std::endl
              << "in line \"" << line << "\""   << std::endl
              << "with the following message: " << std::endl
              << "\"" << msg << "\"\n"          << std::endl;
#else
    if (isError)
    {
        if (Window::activeWindow() != nullptr)
        {
            // Prepares the strings.
            QString strLine = QString::number(line);
            QString strFile(file.data());
            QString strFunc(func.data());
            QString strMsg(msg.data());

            // Prepares the quick info.
            QString strInfo("The Cranberry game engine reported an error "
                            "thrown in module \"%0\" within function \"%1\" "
                            "located in line %2. Please contact the author "
                            "of this game and forward the detailed message.");

            // Shows the messagebox.
            QMessageBox box(Window::activeWindow());
            box.setWindowTitle("Cranberry Error");
            box.setIcon(QMessageBox::Critical);
            box.setStandardButtons(QMessageBox::Close);
            box.setText(strInfo.arg(strFile, strFunc, strLine));
            box.setDetailedText(strMsg);
            box.exec();
        }

        // Terminates the game.
        Game::instance()->exit(CRANBERRY_EXIT_FATAL);
    }
#endif
}


CRANBERRY_END_NAMESPACE

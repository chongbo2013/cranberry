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


#pragma once
#ifndef CRANBERRY_GAME_HPP
#define CRANBERRY_GAME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Standard headers
#include <vector>


CRANBERRY_BEGIN_NAMESPACE


// Forward declarations
class Window;


///
/// Handles all windows in the game.
///
/// \class Game
/// \author Nicolas Kogler
/// \date March 7, 2017
///
class CRANBERRY_EXPORT Game
{
public:

    CRANBERRY_DISABLE_COPY(Game)
    CRANBERRY_DISABLE_MOVE(Game)

    ///
    /// Initializes a new instance of Game.
    ///
    /// \param argc Argument count.
    /// \param argv Argument strings.
    ///
    Game(int& argc, char* argv[]);

    ///
    /// Destroys this Game instance. Also
    /// cleans up the resources of all windows.
    ///
    ~Game();


    ///
    /// Adds the given window to the window manager
    /// and attempts to show it immediately.
    ///
    /// \param window Window to add.
    ///
    void addWindow(Window* window);

    ///
    /// Removes the given window from the window manager
    /// and hides it immediately. The memory allocated
    /// for this window will not be free'd by this function.
    ///
    /// \param window Window to remove.
    ///
    void removeWindow(Window* window);

    ///
    /// Runs the game and shows the main window.
    ///
    /// \param mainWindow Valid main window instance.
    /// \returns the exit code of the application.
    ///
    int run(Window* mainWindow);

    ///
    /// Exits the game and closes all windows.
    ///
    void exit();


    ///
    /// Retrieves the current game instance.
    ///
    /// \returns the current game instance.
    ///
    static Game* instance();


private:

    // Members
    std::vector<Window*> m_windows;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GAME_HPP

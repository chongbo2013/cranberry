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


#pragma once
#ifndef CRANBERRY_GAME_GAME_HPP
#define CRANBERRY_GAME_GAME_HPP


// Cranberry headers
#include <Cranberry/Game/GamePrivate.hpp>

// Qt headers
#include <QVector>

// Forward declarations
CRANBERRY_FORWARD_C(Window)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Handles all windows in the game and more.
///
/// \class Game
/// \author Nicolas Kogler
/// \date June 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT Game
{
public:

    CRANBERRY_DECLARE_DTOR(Game)
    CRANBERRY_DISABLE_COPY(Game)
    CRANBERRY_DISABLE_MOVE(Game)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new Game instance.
    ///
    /// \param argc Argument count.
    /// \param argv Argument strings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Game(int& argc, char* argv[]);

    ////////////////////////////////////////////////////////////////////////////
    /// Adds the given window to the manager and shows it immediately. Fails if
    /// window is not valid or if game is not running yet.
    ///
    /// \param window Window to add.
    /// \returns if adding succeeded.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool addWindow(Window* window);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given window from the manager and hides it immediately.
    /// Fails if window is not valid, does not exist in the manager or if the
    /// game is not running yet.
    ///
    /// \note The window will not be deleted.
    /// \param window Window to remove.
    /// \returns true if removal succeeded.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool removeWindow(Window* window);

    ////////////////////////////////////////////////////////////////////////////
    /// Runs the game and shows the main window. From now on, other child
    /// windows can be added via Game::addWindow().
    ///
    /// \param mainWindow Main window to use.
    /// \returns the exit code of the game.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int run(Window* mainWindow);

    ////////////////////////////////////////////////////////////////////////////
    /// Exits the game, closes all the windows and cleans up all the resources.
    ///
    /// \param exitCode The code to exit with.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void exit(int exitCode = 0);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current game instance.
    ///
    /// \returns the game instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static Game* instance();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    priv::GamePrivate m_private;
    QVector<Window*>  m_windows;
    bool              m_isRunning;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Game
/// \ingroup Game
///
/// The Game class handles all windows.
///
/// \code
/// Game game(argc, argv);
/// MyWindow mainWindow;
///
/// return game.run(&mainWindow);
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

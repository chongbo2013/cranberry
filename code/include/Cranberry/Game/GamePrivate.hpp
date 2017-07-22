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
#ifndef CRANBERRY_GAME_GAMEPRIVATE_HPP
#define CRANBERRY_GAME_GAMEPRIVATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_PRIV_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Registers the signal handlers and builds the stack trace.
///
/// \class GamePrivate
/// \author Nicolas Kogler
/// \date July 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT GamePrivate
{
public:

    CRANBERRY_DEFAULT_DTOR(GamePrivate)
    CRANBERRY_DEFAULT_COPY(GamePrivate)
    CRANBERRY_DEFAULT_MOVE(GamePrivate)

    GamePrivate();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    static void signalHandler(int signal);
    if_debug(static void printLogo());
};


CRANBERRY_END_PRIV_NAMESPACE


#endif

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
#ifndef CRANBERRY_GAMETIME_HPP
#define CRANBERRY_GAMETIME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QDateTime>


CRANBERRY_BEGIN_NAMESPACE


///
/// Stores the total- and delta time.
///
/// \class GameTime
/// \author Nicolas Kogler
/// \date Feburary 9, 2017
///
class CRANBERRY_EXPORT GameTime
{
public:

    CRANBERRY_DISABLE_COPY(GameTime)
    CRANBERRY_DISABLE_MOVE(GameTime)
    CRANBERRY_DEFAULT_DTOR(GameTime)

    ///
    /// Initializes a new instance of GameTime.
    ///
    GameTime();


    ///
    /// Retrieves the total time since the game started.
    ///
    /// \returns the total game time.
    ///
    const QDateTime& totalTime() const;

    ///
    /// Retrieves the delta time between two consecutive
    /// update() calls.
    ///
    /// \returns the delta time.
    ///
    double deltaTime() const;


    ///
    /// Updates the total game time and computes
    /// the time between the last and current call.
    ///
    void update();


private:

    // Members
    QDateTime   m_total;
    double      m_delta;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GAMETIME_HPP

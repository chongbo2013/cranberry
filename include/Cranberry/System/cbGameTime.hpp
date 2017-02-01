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


#ifndef CRANBERRY_CBGAMETIME_HPP
#define CRANBERRY_CBGAMETIME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QTime>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds total game time and last delta time.
///
/// \class cbGameTime
/// \author Nicolas Kogler
/// \date February 1st, 2017
///
class CRANBERRY_EXPORT cbGameTime
{
public:

    ///
    /// Constructor
    ///
    cbGameTime() = default;

    ///
    /// Copy constructor (disabled)
    ///
    /// \param other The other cbGameTime instance.
    ///
    cbGameTime(const cbGameTime& other) = delete;

    ///
    /// Move constructor (disabled)
    ///
    /// \param other The other cbGameTime instance.
    ///
    cbGameTime(cbGameTime&& other) = delete;

    ///
    /// Assignment operator (disabled)
    ///
    /// \param other The other cbGameTime instance.
    ///
    cbGameTime& operator=(const cbGameTime& other) = delete;

    ///
    /// Destructor
    ///
    ~cbGameTime() = default;


    ///
    /// Retrieves the total game time since the start.
    ///
    /// @returns the total game time.
    ///
    const QTime& totalTime() const;

    ///
    /// Retrieves the time between two updates.
    /// The delta time is provided in seconds.
    ///
    /// @returns the delta time in seconds.
    ///
    double deltaTime() const;


    ///
    /// Updates the game's total time and
    /// computes the delta time.
    ///
    void updateTime();


private:

    // Helper functions
    double computeDelta();

    // Members
    QTime  m_totalTime;
    double m_deltaTime;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBGAMETIME_HPP

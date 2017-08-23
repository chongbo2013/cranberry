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
#ifndef CRANBERRY_GAMETIME_HPP
#define CRANBERRY_GAMETIME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Standard headers
#include <chrono>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Stores the total time since the game started and the delta time that
/// specifies the amount of time spent between two updates.
///
/// \class GameTime
/// \author Nicolas Kogler
/// \date May 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT GameTime final
{
public:

    CRANBERRY_DECLARE_CTOR(GameTime)
    CRANBERRY_DEFAULT_DTOR(GameTime)
    CRANBERRY_DEFAULT_COPY(GameTime)
    CRANBERRY_DEFAULT_MOVE(GameTime)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of hours since the game started.
    ///
    /// \returns the total amount of hours.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int totalHours() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of minutes. If you want to get the minutes
    /// within the current hour, do:
    ///
    /// \code
    /// double hours = totalMinutes() / 60.0;
    /// int minutes = (int) ((hours - (int) hours) * 60);
    /// \endcode
    /// \returns the total amount of minutes.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int totalMinutes() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of seconds. If you want to get the minutes
    /// within the current minute, do:
    ///
    /// \code
    /// double minutes = totalSeconds() / 60.0;
    /// int seconds = (int) ((minutes - (int) minutes) * 60);
    /// \returns the total amount of seconds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int totalSeconds() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the delta time between two consecutive update() calls. It is
    /// measured in seconds, that means 0.016 equals 16 milliseconds.
    ///
    /// \returns the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    double deltaTime() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the total game time and computes the time between the last
    /// and the current call.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update();


private:

    typedef std::chrono::milliseconds ms;
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::high_resolution_clock::time_point timepoint;

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    timepoint       m_total;    ///< Total game time
    timepoint       m_previous; ///< Previous amount of ticks
    timepoint       m_current;  ///< Current amount of ticks
    timepoint       m_null;     ///< NULL timepoint
    qreal           m_delta;    ///< Current delta
};


////////////////////////////////////////////////////////////////////////////////
/// \class GameTime
/// \ingroup System
///
/// You will most likely only need to use the delta time. The delta time
/// specifies the time span between the previous and the current update. It is
/// important since the game will usually not run with a constant framerate on
/// all systems for all users. When multiplying it with a variable speed, it is
/// assured that objects will be rotated/moved/scaled the same visible speed
/// under all conditions.
///
/// \code
/// void update(const GameTime& time)
/// {
///     float advance = time.deltaTime() * m_speed;
///     m_position += advance;
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

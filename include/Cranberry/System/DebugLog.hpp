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
#ifndef CRANBERRY_DEBUGLOG_HPP
#define CRANBERRY_DEBUGLOG_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Standard headers
#include <string>


CRANBERRY_BEGIN_NAMESPACE


///
/// Logs all errors and warnings that occur
/// within cranberry classes. As for errors
/// in release mode, shows a message box informing
/// the player about the problem.
///
/// \class DebugLog
/// \author Nicolas Kogler
/// \date March 25, 2017
///
class CRANBERRY_EXPORT DebugLog
{
public:

    ///
    /// Writes a detailed message to the console.
    /// Shows a message box in release mode instead.
    ///
    /// \param line Line in which the error occured.
    /// \param file Code file in which the error occured.
    /// \param func Function in which the error occured.
    /// \param msg Error message.
    /// \param isError Is it an actual error or just a warning?
    ///
    static void write(int32_t line,
                      std::string file,
                      std::string func,
                      std::string msg,
                      bool isError);
};


///
/// Debug mode: Logs message to console.
/// Release mode: Show messagebox containing the error.
///
/// @def cranError
///
#define cranError(msg) (DebugLog::write(__LINE__, __FILE__, CRANBERRY_FUNC, msg, true))

///
/// Debug mode: Logs message to console.
/// Release mode: Does absolutely nothing.
///
/// @def cranWarning
///
#define cranWarning(msg) (DebugLog::write(__LINE__, __FILE__, CRANBERRY_FUNC, msg, false))


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_DEBUGLOG_HPP

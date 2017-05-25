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


#pragma once
#ifndef CRANBERRY_DEBUG_HPP
#define CRANBERRY_DEBUG_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers

// Standard headers


CRANBERRY_BEGIN_PRIV_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// The Debug class provides functions to show errors an warnings. In release
/// mode, the errors will not be printed to the console (because there is none)
/// but will be instead shown in a message box.
///
/// \class Debug
/// \author Nicolas Kogler
/// \date May 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class Debug
{
public:

    CRANBERRY_DEFAULT_CTOR(Debug)
    CRANBERRY_DEFAULT_DTOR(Debug)
    CRANBERRY_DEFAULT_COPY(Debug)
    CRANBERRY_DEFAULT_MOVE(Debug)


    ////////////////////////////////////////////////////////////////////////////
    /// Shows an error. Behaves differently, depending on whether the library
    /// was built in debug or release mode.
    ///
    /// \param msg The message string.
    /// \param line Line in which the error occured.
    /// \param file Source file in which the error occured.
    /// \param func Function in which the error occured.
    /// \returns false in order to directly use 'return cranError()'.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static bool showError(
            const QString& msg,
            const uint line,
            const std::string file,
            const std::string func
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Shows a warning. Behaves differently, depending on whether the library
    /// was built in debug or release mode.
    ///
    /// \param msg The message string.
    /// \param line Line in which the error occured.
    /// \param file Source file in which the error occured.
    /// \param func Function in which the error occured.
    /// \returns false in order to directly use 'return cranWarning()'.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static bool showWarning(
            const QString& msg,
            const uint line,
            const std::string file,
            const std::string func
            );
};


////////////////////////////////////////////////////////////////////////////////
/// In debug mode, prints the error to the console. In release mode, shows a
/// message box containing the error message.
///
/// \def cranError
///
////////////////////////////////////////////////////////////////////////////////
#define cranError(msg) (Debug::showError(QString(msg), __LINE__, __FILE__, CRANBERRY_FUNC))

////////////////////////////////////////////////////////////////////////////////
/// In debug mode, prints the warning to the console. In release mode, does
/// absolutely nothing.
///
/// \def cranWarning
///
////////////////////////////////////////////////////////////////////////////////
#define cranWarning(msg) (Debug::showWarning(QString(msg), __LINE__, __FILE__, CRANBERRY_FUNC))


////////////////////////////////////////////////////////////////////////////////
/// \class Debug
/// \ingroup System
///
/// You will not need to interact with the Debug class directly, since the two
/// macroes cranError and cranWarning do it for you. The following code example
/// shows how to use them:
///
/// \code
/// if (something_bad_happened)
/// {
///     cranError("Uh-oh.");
///     return false;
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_PRIV_NAMESPACE


#endif

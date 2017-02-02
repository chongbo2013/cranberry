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


#ifndef CRANBERRY_CBGLDEBUG_HPP
#define CRANBERRY_CBGLDEBUG_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QOpenGLFunctions>


CRANBERRY_BEGIN_NAMESPACE


///
/// Contains a function to check for OpenGL errors.
///
/// \class cbGLDebug
/// \author Nicolas Kogler
/// \date February 2nd, 2017
///
class CRANBERRY_EXPORT cbGLDebug
{
public:

    ///
    /// Determines whether the latest OpenGL function
    /// threw and error. If yes, outputs a message with
    /// all the details about the error message.
    ///
    /// \param file The file in which the error occured.
    /// \param func The function in which the error occured.
    /// \param line The line in which the error occured.
    /// \param expr The expression which caused the error.
    ///
    static void performCheck(
            std::string file,
            std::string func,
            std::string line,
            std::string expr);

private:

    static QOpenGLFunctions* g_Functions;
};


///
/// This macro should be used hand-in-hand with every
/// call to an OpenGL function in order to immediately
/// determine whether this specific function call caused
/// an error. If one happens to forget glDebug(), it is
/// not clear which of the two last functions threw the
/// error.
///
/// \def glDebug
///
#ifdef QT_DEBUG
    #define glDebug(expr) do            \
    {                                   \
        expr;                           \
        cbGLDebug::performCheck(        \
            std::string(__FILE__),      \
            CRANBERRY_FUNC,             \
            std::to_string(__LINE__),   \
            #expr);                     \
    } while (0)
#else
    #define glDebug(expr) { expr; }
#endif


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_CBGLDEBUG_HPP

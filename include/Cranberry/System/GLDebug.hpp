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
#ifndef CRANBERRY_GLDEBUG_HPP
#define CRANBERRY_GLDEBUG_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Provides a function to print OpenGL errors.
///
/// \class GLDebug
/// \author Nicolas Kogler
/// \date Feburary 10, 2017
///
class CRANBERRY_EXPORT GLDebug
{
public:

    ///
    /// Prints any previous OpenGL errors.
    ///
    /// \param file Potentially erroneous file.
    /// \param func Potentially erroneous function.
    /// \param line Potentially erroneous line.
    /// \param expr Potentially erroneous expression.
    ///
    static void print(
            const char* file,
            const char* func,
            long long   line,
            const char* expr);
};


#ifdef QT_DEBUG
    #define glDebug(x)      \
    do                      \
    {                       \
        x;                  \
        GLDebug::print(     \
            __FILE__,       \
            CRANBERRY_FUNC, \
            __LINE__        \
            #x              \
        );                  \
    } while(0)

#else
    #define glDebug(x) x
#endif


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GLDEBUG_HPP

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
#ifndef CRANBERRY_OPENGL_OPENGLDEBUG_HPP
#define CRANBERRY_OPENGL_OPENGLDEBUG_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_PRIV_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Provides functions for printing OpenGL errors.
///
/// \class GLDebug
/// \author Nicolas Kogler
/// \date May 23, 2017
///
////////////////////////////////////////////////////////////////////////////////
class GLDebug final
{
public:

    ////////////////////////////////////////////////////////////////////////////
    /// Prints any previous OpenGL errors.
    ///
    /// \param file Erroneous file.
    /// \param func Erroneous function.
    /// \param line Erroneous line.
    /// \param expr Erroneous expression.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static void print(
            std::string expr,
            std::string file,
            std::string func,
            long long   line
            );
};


#ifdef QT_DEBUG
    #define glDebug(x)              \
    do                              \
    {                               \
        x;                          \
        priv::GLDebug::print(       \
            #x,                     \
            __FILE__,               \
            CRANBERRY_FUNC,         \
            __LINE__                \
        );                          \
    } while(0)
#else
    #define glDebug(x) x
#endif


////////////////////////////////////////////////////////////////////////////////
/// \class GLDebug
/// \ingroup OpenGL
///
/// The glDebug macro should be used along with every call to an OpenGL
/// function. In release mode, no overhead is added.
///
/// \code
/// glDebug(glGenTextures(1, &id));
/// glDebug(glBindTexture(GL_TEXTURE_2D, id));
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_PRIV_NAMESPACE


#endif

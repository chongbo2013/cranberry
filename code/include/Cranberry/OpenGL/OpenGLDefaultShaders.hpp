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
#ifndef CRANBERRY_OPENGLDEFAULTSHADERS_HPP
#define CRANBERRY_OPENGLDEFAULTSHADERS_HPP


// Cranberry headers
#include <Cranberry/OpenGL/OpenGLShader.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds default shaders for graphics objects.
///
/// \class OpenGLDefaultShaders
/// \author Nicolas Kogler
/// \date June 7, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_OPENGL_EXPORT OpenGLDefaultShaders
{
public:


    ////////////////////////////////////////////////////////////////////////////
    /// Adds a shader to the default shader collection. Given key must be unique
    /// and the given program valid. If a call to OpenGLDefaultShaders::remove()
    /// is never issued, you are required to manage the memory of \p program.
    ///
    /// \param name Key with which to store program.
    /// \param program Program to store.
    /// \returns false if shader could not be added.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static bool add(const QString& name, OpenGLShader* program);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given program called \p name. Attention: The program added
    /// via OpenGLDefaultShaders::add must reside on the heap - do not call this
    /// function if otherwise.
    ///
    /// \param name Key of the program to remove.
    /// \returns false if shader could not be removed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static bool remove(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the program called \p name. Returns a null pointer if unable
    /// to find the shader.
    ///
    /// \param name Key of the program to retrieve.
    /// \returns the associated shader program.
    ///
    ////////////////////////////////////////////////////////////////////////////
    static OpenGLShader* get(const QString& name);
};


typedef QHash<QString, OpenGLShader*> ShaderMap;


////////////////////////////////////////////////////////////////////////////////
/// \class OpenGLDefaultShaders
/// \ingroup OpenGL
///
/// A static class that holds default shaders for shapes and sprites. They can
/// then be accessed anywhere, anytime. The OpenGLDefaultShaders::get() function
/// is furthermore thread-safe, meaning you can initialize graphics objects from
/// multiple threads.
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

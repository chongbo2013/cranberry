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
#ifndef CRANBERRY_GLSHADER_HPP
#define CRANBERRY_GLSHADER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Forward declarations
class QOpenGLShaderProgram;


CRANBERRY_BEGIN_NAMESPACE


///
/// Provides functions to dynamically load
/// shaders that can either be GL or GLES.
///
/// \class GLShader
/// \author Nicolas Kogler
/// \date February 11, 2017
///
class CRANBERRY_EXPORT GLShader
{
public:

    ///
    /// Adds shaders to the specified shader program.
    ///
    /// \param program The program to give shaders to.
    /// \param vert The file path to the vertex shader.
    /// \param frag The file path to the fragment shader.
    ///
    static void load(
            QOpenGLShaderProgram* program,
            const char* vert,
            const char* frag);


private:

    ///
    /// Retrieves and modifies the GLSL source code.
    ///
    /// \param path The file path to the shader.
    ///
    static QString getSource(const char* path);
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_GLSHADER_HPP

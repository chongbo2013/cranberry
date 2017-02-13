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


// Cranberry headers
#include <Cranberry/System/GLDebug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLFunctions>

// Std headers
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


void GLDebug::print(
        std::string file,
        std::string func,
        long long line,
        std::string expr)
{
    // Retrieves the GL functions in the current context.
    QOpenGLFunctions* funcs = Window::activeWindow()->functions();
    if (funcs == nullptr)
        return;

    // Prints OpenGL errors until GL_NO_ERROR is returned.
    GLenum error = funcs->glGetError();
    while (error != GL_NO_ERROR)
    {
        const char* type;
        const char* desc;

        switch (error)
        {
            case GL_INVALID_ENUM:
                type = "GL_INVALID_ENUM";
                desc = "An invalid enum value was passed.";
                break;
            case GL_INVALID_VALUE:
                type = "GL_INVALID_VALUE";
                desc = "An invalid value was passed.";
                break;
            case GL_INVALID_INDEX:
                type = "GL_INVALID_INDEX";
                desc = "An invalid index was passed.";
                break;
            case GL_INVALID_OPERATION:
                type = "GL_INVALID_OPERATION";
                desc = "Can not succeed due to invalid OpenGL states.";
                break;
            case GL_STACK_OVERFLOW:
                type = "GL_STACK_OVERFLOW";
                desc = "Can not push due to the stack being full.";
                break;
            case GL_STACK_UNDERFLOW:
                type = "GL_STACK_UNDERFLOW";
                desc = "Can not pop due to the stack being empty.";
                break;
            case GL_OUT_OF_MEMORY:
                type = "GL_OUT_OF_MEMORY";
                desc = "Can not allocate OpenGL object. Memory is full.";
                break;
            default:
                type = "Unknown error";
                desc = "Error could not be resolved.";
                break;
        }

        // Prints the message.
        std::cout << "OpenGL error occured" << std::endl
                  << "of type: "    << type << std::endl
                  << "in file: "    << file << std::endl
                  << "in func: "    << func << std::endl
                  << "in line: "    << line << std::endl
                  << "by call: "    << expr << std::endl
                  << "details: "    << desc << std::endl
                  << std::endl;

        error = funcs->glGetError();
    }
}


CRANBERRY_END_NAMESPACE

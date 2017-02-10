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


CRANBERRY_BEGIN_NAMESPACE


void GLDebug::print(
        const char* file,
        const char* func,
        long long line,
        const char* expr)
{
    // Retrieves the GL functions in the current context.
    QOpenGLFunctions* funcs = Window::activeWindow()->functions();
    if (funcs == nullptr)
        return;

    // Prints OpenGL errors until GL_NO_ERROR is returned.
    GLenum error = funcs->glGetError();
    while (error != GL_NO_ERROR)
    {
        const char* etype;
        const char* cause;

        switch (error)
        {
            case GL_INVALID_ENUM:
                etype = "GL_INVALID_ENUM";
                cause = "An invalid enum value was passed.";
                break;
            case GL_INVALID_VALUE:
                etype = "GL_INVALID_VALUE";
                cause = "An invalid value was passed.";
                break;
            case GL_INVALID_INDEX:
                etype = "GL_INVALID_INDEX";
                cause = "An invalid index was passed.";
                break;
            case GL_INVALID_OPERATION:
                etype = "GL_INVALID_OPERATION";
                cause = "Can not succeed due to invalid OpenGL states.";
                break;
            case GL_STACK_OVERFLOW:
                etype = "GL_STACK_OVERFLOW";
                cause = "Can not push due to the stack being full.";
                break;
            case GL_STACK_UNDERFLOW:
                etype = "GL_STACK_UNDERFLOW";
                cause = "Can not pop due to the stack being empty.";
                break;
            case GL_OUT_OF_MEMORY:
                etype = "GL_OUT_OF_MEMORY";
                cause = "Can not allocate OpenGL object. Memory is full.";
                break;
            default:
                etype = "Unknown error";
                cause = "Error could not be resolved.";
                break;
        }

        // Prints the message.

        error = funcs->glGetError();
    }
}


CRANBERRY_END_NAMESPACE

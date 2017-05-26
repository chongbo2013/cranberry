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


// Cranberry headers
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>

// Qt headers
#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


CRANBERRY_USING_NAMESPACE


void priv::GLDebug::print(
        std::string expr,
        std::string file,
        std::string func,
        long long line)
{
    auto* gl = QOpenGLContext::currentContext()->functions();
    if (gl == nullptr)
    {
        cranError("No active OpenGL context.");
        return;
    }

    // Prints all OpenGL errors.
    auto error = gl->glGetError();
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
        qDebug().noquote() << "OpenGL error occured";
        qDebug().noquote() << "of type: " << type;
        qDebug().noquote() << "in file: " << file.c_str();
        qDebug().noquote() << "in func: " << func.c_str();
        qDebug().noquote() << "in line: " << line;
        qDebug().noquote() << "by call: " << expr.c_str();
        qDebug().noquote() << "details: \"" << desc << "\"";

        error = gl->glGetError();
    }
}

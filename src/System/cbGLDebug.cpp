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
#include <Cranberry/System/cbGLDebug.hpp>
#include <Cranberry/Window/cbWindow.hpp>

// Standard headers
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


void
cbGLDebug::performCheck(std::string file,
                        std::string func,
                        std::string line,
                        std::string expr)
{
    // Tries to load the functions, if not already.
    if (g_Functions == nullptr)
        g_Functions = cbWindow::current()->context()->functions();

    // Determines whether an error occured.
    auto error = g_Functions->glGetError();
    if (error == GL_NO_ERROR)
    {
        // Retrieves type and description of the error.
        const char *type, *desc;
        switch (error)
        {
        case GL_INVALID_ENUM:
            type = "GL_INVALID_ENUM";
            desc = "Specified enum value is not valid within this function.";
            break;
        case GL_INVALID_VALUE:
            type = "GL_INVALID_VALUE";
            desc = "Specified value is not legal within this function.";
            break;
        case GL_INVALID_OPERATION:
            type = "GL_INVALID_OPERATION";
            desc = "The function failed due to invalid OpenGL states.";
            break;
        case GL_STACK_OVERFLOW:
            type = "GL_STACK_OVERFLOW";
            desc = "Pushing failed. Exceeded maximum stack size.";
            break;
        case GL_STACK_UNDERFLOW:
            type = "GL_STACK_UNDERFLOW";
            desc = "Popping failed. Stack is at lowest point.";
            break;
        case GL_OUT_OF_MEMORY:
            type = "GL_OUT_OF_MEMORY";
            desc = "Requested space could not be allocated.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            type = "GL_INVALID_FRAMEBUFFER_OPERATION";
            desc = "Reading or writing to the frame buffer failed.";
            break;
        default:
            type = "Unknown error";
            desc = "Error could not be resolved.";
            break;
        };

        // Outputs the error message.
        std::cout << "OpenGL error occured" << std::endl
                  << "of type: "    << type << std::endl
                  << "in file: "    << file << std::endl
                  << "in func: "    << func << std::endl
                  << "in line: "    << line << std::endl
                  << "by call: "    << expr << std::endl
                  << "details: "    << desc << std::endl
                  << std::endl;
    }
}


// Static variable definitions
QOpenGLFunctions* cbGLDebug::g_Functions = nullptr;


CRANBERRY_END_NAMESPACE

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
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>

// Qt headers
#include <QHash>
#include <QMutex>


CRANBERRY_USING_NAMESPACE


CRANBERRY_GLOBAL_VAR(ShaderMap, g_programs)
CRANBERRY_GLOBAL_VAR(QMutex, g_mutex)


bool OpenGLDefaultShaders::add(const QString& name, OpenGLShader* program)
{
    if (g_programs.contains(name)) return false;
    g_programs.insert(name, program);

    return true;
}


bool OpenGLDefaultShaders::remove(const QString& name)
{
    if (!g_programs.contains(name)) return false;
    auto* program = g_programs.take(name);
    delete program;
    return true;
}


OpenGLShader* OpenGLDefaultShaders::get(const QString& name)
{
    OpenGLShader* result = nullptr;

    g_mutex.lock();
    result = g_programs[name];
    g_mutex.unlock();

    return result;
}

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
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>

// Qt headers
#include <QHash>
#include <QMutex>
#include <QVector>
#include <QOpenGLShaderProgram>

// Standard headers
#include <ctime>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, c_path, ":/cb/glsl/%0_%1.glsl")
CRANBERRY_GLOBAL_VAR(ShaderMap, g_programs)
CRANBERRY_GLOBAL_VAR(ShaderUpdateList, g_updateList)
CRANBERRY_GLOBAL_VAR(QMutex, g_mutex)


bool OpenGLDefaultShaders::add(const QString& name, OpenGLShader* program, bool update)
{
    if (g_programs.contains(name)) return false;
    if (update) g_updateList.append(program);

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


OpenGLShader* OpenGLDefaultShaders::cranberryGetShader(const char* name)
{
    QString vpath = c_path.arg(name, "vert");
    QString fpath = c_path.arg(name, "frag");
    OpenGLShader* s = new OpenGLShader;

    s->setVertexShaderFromFile(vpath);
    s->setFragmentShaderFromFile(fpath);

    return s;
}


void OpenGLDefaultShaders::cranberryLoadDefaultShaders()
{
    // Normal shaders
    add("cb.glsl.texture", cranberryGetShader("texture"));
    add("cb.glsl.shape", cranberryGetShader("shape"));

    // Updatable shaders
    add("cb.glsl.film", cranberryGetShader("film"), true);
}


void OpenGLDefaultShaders::cranberryFreeDefaultShaders()
{
    remove("cb.glsl.texture");
    remove("cb.glsl.shape");
    remove("cb.glsl.film");
}


void OpenGLDefaultShaders::cranberryInitDefaultShaders()
{
    // Film
    QOpenGLShaderProgram* p = get("cb.glsl.film")->program();
    {
        p->bind();
        p->setUniformValue("u_time", (float) clock());
        p->setUniformValue("u_noise", 0.5f);
        p->setUniformValue("u_lines", 0.05f);
        p->setUniformValue("u_count", 4096.0f);
        p->release();
    }
}


void OpenGLDefaultShaders::cranberryUpdateDefaultShaders()
{
    float t = static_cast<float>(clock() % 10000);
    for (OpenGLShader* s : g_updateList)
    {
        glDebug(s->program()->bind());
        glDebug(s->program()->setUniformValue("u_time", t));
    }
}

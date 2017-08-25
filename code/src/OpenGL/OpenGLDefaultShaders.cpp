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


bool OpenGLDefaultShaders::add(
        const QString& name,
        OpenGLShader* program,
        bool update
        )
{
    if (g_programs.contains(name))
    {
        return false;
    }
    else if (update)
    {
        g_updateList.append(program);
    }

    g_programs.insert(name, program);

    return true;
}


bool OpenGLDefaultShaders::remove(const QString& name)
{
    if (!g_programs.contains(name))
    {
        return false;
    }

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
    add("cb.glsl.hatch", cranberryGetShader("hatch"));
    add("cb.glsl.lens", cranberryGetShader("lens"));
    add("cb.glsl.kaleido", cranberryGetShader("kaleido"));
    add("cb.glsl.spiral", cranberryGetShader("spiral"));
    add("cb.glsl.fisheye", cranberryGetShader("fisheye"));
    add("cb.glsl.radialblur", cranberryGetShader("radialblur"));
    add("cb.glsl.blur", cranberryGetShader("blur"));
    add("cb.glsl.pixel", cranberryGetShader("pixel"));
    add("cb.glsl.tilemap", cranberryGetShader("tilemap"));
    add("cb.glsl.text", cranberryGetShader("text"));

    // Updatable shaders
    add("cb.glsl.film", cranberryGetShader("film"), true);
}


void OpenGLDefaultShaders::cranberryFreeDefaultShaders()
{
    g_updateList.clear();

    remove("cb.glsl.texture");
    remove("cb.glsl.shape");
    remove("cb.glsl.film");
    remove("cb.glsl.blur");
    remove("cb.glsl.pixel");
    remove("cb.glsl.hatch");
    remove("cb.glsl.lens");
    remove("cb.glsl.kaleido");
    remove("cb.glsl.spiral");
    remove("cb.glsl.fisheye");
    remove("cb.glsl.radialblur");
    remove("cb.glsl.tilemap");
    remove("cb.glsl.text");
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
    }

    // Blur
    p = get("cb.glsl.blur")->program();
    {
        p->bind();
        p->setUniformValue("u_blurH", 1.0f);
        p->setUniformValue("u_blurV", 0.0f);
    }

    // Pixel
    p = get("cb.glsl.pixel")->program();
    {
        p->bind();
        p->setUniformValue("u_pixelW", 8.0f);
        p->setUniformValue("u_pixelH", 8.0f);
    }

    // Hatch
    p = get("cb.glsl.hatch")->program();
    {
        p->bind();
        p->setUniformValue("u_offset", 5.0f);
        p->setUniformValue("u_threshold_1", 1.0f);
        p->setUniformValue("u_threshold_2", 0.7f);
        p->setUniformValue("u_threshold_3", 0.5f);
        p->setUniformValue("u_threshold_4", 0.3f);
    }

    // Lens
    p = get("cb.glsl.lens")->program();
    {
        p->bind();
        p->setUniformValue("u_radiusX", 0.50f);
        p->setUniformValue("u_radiusY", 0.30f);
        p->setUniformValue("u_color", QVector4D(0.f, 0.f, 0.f, 1.f));
    }

    // Kaleido
    p = get("cb.glsl.kaleido")->program();
    {
        p->bind();
        p->setUniformValue("u_sides", 6.0f);
        p->setUniformValue("u_angle", 0.0f);
    }

    // Spiral
    p = get("cb.glsl.spiral")->program();
    {
        p->bind();
        p->setUniformValue("u_angle", 0.8f);

        // radius and origin heavily depend on the texture being used on,
        // therefore update those values yourself in update().
    }

    // Fisheye
    p = get("cb.glsl.fisheye")->program();
    {
        p->bind();
        p->setUniformValue("u_radius", 3.0f);
        p->setUniformValue("u_bend", 10.0f);
    }

    // Radial blur
    p = get("cb.glsl.radialblur")->program();
    {
        p->bind();
        p->setUniformValue("u_blur", 0.1f);
        p->setUniformValue("u_bright", 1.0f);
        p->setUniformValue("u_offset", 30);
    }
}


void OpenGLDefaultShaders::cranberryUpdateDefaultShaders()
{
    int t = static_cast<int>(clock());
    for (OpenGLShader* s : g_updateList)
    {
        glDebug(s->program()->bind());
        glDebug(s->program()->setUniformValue("u_time", t));
    }
}

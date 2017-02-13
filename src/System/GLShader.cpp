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
#include <Cranberry/System/GLShader.hpp>

// Qt headers
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>


CRANBERRY_BEGIN_NAMESPACE


void GLShader::load(
        QOpenGLShaderProgram* program,
        const char* vert,
        const char* frag)
{
    if (!program->create())
        return;

    QString vertexCode = getSource(vert);
    QString fragmentCode = getSource(frag);

    // Registers both shaders.
    if (!vertexCode.isEmpty() && !fragmentCode.isEmpty())
    {
        if (!program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexCode))
            qDebug() << program->log();
        if (!program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentCode))
            qDebug() << program->log();

        if (!program->link())
            qDebug() << program->log();
    }
}


QString GLShader::getSource(const char* path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return QString();

    QTextStream textStream(&file);
    QString code = textStream.readAll();

    // Specifies a version string depending on the used GL API.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
        return code.replace("%0", "330");
    else // GLES 3.0
        return code.replace("%0", "300 es");
}


CRANBERRY_END_NAMESPACE

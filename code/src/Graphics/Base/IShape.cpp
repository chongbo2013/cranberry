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
#include <Cranberry/Graphics/Base/IShape.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Vertex buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Color count does not match vertex count.")


IShape::IShape()
    : m_vertexBuffer(nullptr)
    , m_filled(false)
    , m_colorUpdate(false)
    , m_update(false)
{
}


IShape::~IShape()
{
    destroy();
}


bool IShape::isNull() const
{
    return IRenderable::isNull()     ||
           m_vertexBuffer == nullptr ||
          !m_vertexBuffer->isCreated();
}


void IShape::destroy()
{
    if (m_vertexBuffer != nullptr)
    {
        m_vertexBuffer->destroy();
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }

    m_colorBuffer.clear();
    m_update = false;

    IRenderable::destroy();
}


void IShape::update(const GameTime& time)
{
    updateTransform(time);
}


void IShape::render()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_02));
        return;
    }
    else if (QOpenGLContext::currentContext() != renderTarget()->context())
    {
        renderTarget()->makeCurrent();
    }

    bindObjects();
    writeVertices();
    modifyProgram();
    modifyAttribs();
    drawElements();
    releaseObjects();
}


uint IShape::vertexCount() const
{
    return m_vertices.size();
}


bool IShape::isShapeFilled() const
{
    return m_filled;
}


void IShape::setShapeFilled(bool filled)
{
    m_filled = filled;
}


void IShape::setColor(const QColor& color)
{
    m_colorBuffer.clear();
    m_colorBuffer.append(color);
    m_colorUpdate = true;
    m_update = true;
}


void IShape::setColor(const QVector<QColor>& colors)
{
    if (colors.size() != (int) m_vertices.size())
    {
        cranError(ERRARG(e_03));
        return;
    }

    m_colorBuffer = colors;
    m_colorUpdate = true;
    m_update = true;
}


bool IShape::createInternal(const QVector<QVector2D>& points, Window* rt)
{
    if (!IRenderable::create(rt)) return false;
    if (!createBuffer()) return false;

    // Allocates as much data as we need to store all points.
    m_vertexBuffer->allocate(priv::Vertex::size() * points.size());
    m_vertexBuffer->release();
    m_vertices.clear();
    m_update = true;

    auto size = findSize(points);
    for (const QVector2D& p : points)
    {
        priv::Vertex v;
        v.xyz(p.x(), p.y(), 0);
        m_vertices.push_back(v);
    }

    setOrigin(findCenter(points));
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.shape"));
    setSize(size.x(), size.y());

    return true;
}


bool IShape::createBuffer()
{
    // Attempts to create the buffer holding the vertex data.
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_vertexBuffer->create() || !m_vertexBuffer->bind())
    {
        return cranError(ERRARG(e_01));
    }

    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    return true;
}


QVector2D IShape::findCenter(const QVector<QVector2D>& points)
{
    QVector2D center;
    qreal signedArea = 0.0, x0 = 0.0, y0 = 0.0, x1 = 0.0, y1 = 0.0, a = 0.0;

    // Computes the centroid for each vertex except the last.
    int size = points.size() - 1, i;
    for (i = 0; i < size; i++)
    {
        x0 = points.at(i).x();
        y0 = points.at(i).y();
        x1 = points.at(i+1).x();
        y1 = points.at(i+1).y();
        a = x0 * y1 - x1 * y0;

        signedArea += a;
        center.setX(center.x() + (x0 + x1) * a);
        center.setY(center.y() + (y0 + y1) * a);
    }

    // Compute last vertex separately.
    x0 = points.at(i).x();
    y0 = points.at(i).y();
    x1 = points.at(0).x();
    y1 = points.at(0).y();
    a = x0 * y1 - x1 * y0;

    signedArea += a;
    center.setX(center.x() + (x0 + x1) * a);
    center.setY(center.y() + (y0 + y1) * a);

    signedArea *= 0.5;
    center.setX(center.x() / (signedArea * 6.0));
    center.setY(center.y() / (signedArea * 6.0));

    return center;
}


QVector2D IShape::findSize(const QVector<QVector2D>& points)
{
    auto extremeX = std::minmax_element(
                points.begin(), points.end(),
                [] (const QVector2D& l, const QVector2D& r) {
                    return l.x() < r.x();
                });

    auto extremeY = std::minmax_element(
                points.begin(), points.end(),
                [] (const QVector2D& l, const QVector2D& r) {
                    return l.y() < r.y();
                });

    QVector2D upperLeft(extremeX.first->x(), extremeY.first->y());
    QVector2D lowerRight(extremeX.second->x(), extremeY.second->y());

    return lowerRight - upperLeft;
}


QMatrix4x4 IShape::buildMatrix()
{
    float fw = static_cast<float>(renderTarget()->width());
    float fh = static_cast<float>(renderTarget()->height());

    // TODO: Not cheap - maybe optimize this?!
    QMatrix4x4 proj, tran, rot, scale, orig, norig;
    proj.ortho(0.f, fw, fh, 0.f, -1.f, 1.f);
    tran.translate(x(), y());
    rot.rotate(angleX(), 1.f, 0.f, 0.f);
    rot.rotate(angleY(), 0.f, 1.f, 0.f);
    rot.rotate(angleZ(), 0.f, 0.f, 1.f);
    scale.scale(scaleX(), scaleY());
    orig.translate(origin());
    norig.translate(origin() * -1);

    return proj * tran * orig * rot * norig * orig * scale * norig;
}


void IShape::bindObjects()
{
    glDebug(m_vertexBuffer->bind());
    glDebug(shaderProgram()->program()->bind());
}


void IShape::releaseObjects()
{
    glDebug(m_vertexBuffer->release());
    glDebug(shaderProgram()->program()->release());
}


void IShape::writeVertices()
{
    if (m_update || m_colorUpdate)
    {
        if (m_colorUpdate)
        {
            bool singleColor = m_colorBuffer.size() == 1;
            int size = (int) vertexCount();

            // Colors every vertex.
            for (int i = 0; i < size; i++)
            {
                if (singleColor) m_vertices[i].rgba(m_colorBuffer.at(0));
                else m_vertices[i].rgba(m_colorBuffer.at(i));
            }
        }

        // Uploads the vertex data.
        glDebug(m_vertexBuffer->write(
                    0, m_vertices.data(),
                    priv::Vertex::size() * vertexCount())
                    );

        m_update = false;
        m_colorUpdate = false;
    }
}


void IShape::modifyProgram()
{
    QOpenGLShaderProgram* program = shaderProgram()->program();

    glDebug(program->setUniformValue("u_mvp", buildMatrix()));
    glDebug(program->setUniformValue("u_opac", opacity()));
    glDebug(program->enableAttributeArray(priv::Vertex::xyzAttrib()));
    glDebug(program->enableAttributeArray(priv::Vertex::rgbaAttrib()));
}


void IShape::modifyAttribs()
{
    glDebug(gl->glVertexAttribPointer(
                priv::Vertex::xyzAttrib(),
                priv::Vertex::xyzLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::Vertex::size(),
                priv::Vertex::xyzOffset()
                ));

    glDebug(gl->glVertexAttribPointer(
                priv::Vertex::rgbaAttrib(),
                priv::Vertex::rgbaLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::Vertex::size(),
                priv::Vertex::rgbaOffset()
                ));
}


void IShape::drawElements()
{
    uint mode = (m_filled) ? renderModeFilled() : renderModeWired();
    glDebug(gl->glDrawArrays(mode, GL_ZERO, vertexCount()));
}

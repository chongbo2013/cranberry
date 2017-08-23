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
#include <Cranberry/Graphics/Base/ShapeBase.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Vertex buffer creation failed.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Color count does not match vertex count.")
CRANBERRY_CONST_VAR(float, c_magic, 0.375f)


CRANBERRY_USING_NAMESPACE


ShapeBase::ShapeBase()
    : m_vertexBuffer(nullptr)
    , m_lineWidth(1)
    , m_filled(false)
    , m_colorUpdate(false)
    , m_smooth(true)
    , m_update(false)
{
}


ShapeBase::~ShapeBase()
{
    destroy();
}


bool ShapeBase::isNull() const
{
    return RenderBase::isNull()      ||
           m_vertexBuffer == nullptr ||
          !m_vertexBuffer->isCreated();
}


void ShapeBase::destroy()
{
    delete m_vertexBuffer;

    m_vertexBuffer = nullptr;
    m_colorBuffer.clear();
    m_vertices.clear();
    m_update = false;

    RenderBase::destroy();
}


void ShapeBase::update(const GameTime& time)
{
    updateTransform(time);
}


void ShapeBase::render()
{
    if (!prepareRendering())
    {
        return;
    }

    bindObjects();
    writeVertices();
    modifyProgram();
    modifyAttribs();
    drawElements();
    releaseObjects();
}


uint ShapeBase::vertexCount() const
{
    return m_vertices.size();
}


bool ShapeBase::isShapeFilled() const
{
    return m_filled;
}


int ShapeBase::lineWidth() const
{
    return m_lineWidth;
}


bool ShapeBase::isSmooth() const
{
    return m_smooth;
}


void ShapeBase::setShapeFilled(bool filled)
{
    m_filled = filled;
}


void ShapeBase::setSmooth(bool smooth)
{
    m_smooth = smooth;
}


void ShapeBase::setLineWidth(int width)
{
    m_lineWidth = qMax(width, 1);
}


void ShapeBase::setColor(const QColor& color)
{
    m_colorBuffer.clear();
    m_colorBuffer.append(color);
    m_colorUpdate = true;
    m_update = true;
}


void ShapeBase::setColor(const QVector<QColor>& colors)
{
    if (colors.size() != (int) m_vertices.size())
    {
        cranError(ERRARG(e_02));
        return;
    }

    m_colorBuffer = colors;
    m_colorUpdate = true;
    m_update = true;
}


bool ShapeBase::createInternal(const QVector<QPointF>& points, Window* rt)
{
    if (!RenderBase::create(rt))
    {
        return false;
    }
    else if (!createBuffer())
    {
        return false;
    }

    auto size = findSize(points);
    for (int i = 0; i < points.size(); i++)
    {
        const QPointF& p = points.at(i);
        if (m_lineWidth == 1)
        {
            priv::Vertex v;
            v.xyz(p.x() + c_magic, p.y() + c_magic, 0);
            m_vertices.push_back(v);
        }
        else
        {
            int b = i;
            int a, c, d;

            if (isShapeClosed())
            {
                a = (i - 1 < 0) ? points.size() - 1 : i - 1;

                // If shape is closed, we need to fetch the first two elements
                // again, otherwise it will be missing a side afterwards.
                c = (i + 1 >= points.size()) ? qAbs(points.size() - (i + 1)) : i + 1;
                d = (i + 2 >= points.size()) ? qAbs(points.size() - (i + 2)) : i + 2;
            }
            else
            {
                a = (i - 1 < 0) ? 0 : i - 1;
                c = (i + 1 >= points.size()) ? points.size() - 1 : i + 1;
                d = (i + 2 >= points.size()) ? points.size() - 1 : i + 2;
            }

            extrudeSegment(points.at(a), points.at(b), points.at(c), points.at(d));
        }
    }

    m_vertexBuffer->allocate(priv::Vertex::size() * m_vertices.size());
    m_vertexBuffer->release();
    m_points = points;
    m_update = true;

    setOrigin(findCenter(points));
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.shape"));
    setSize(size.x(), size.y());

    return true;
}


bool ShapeBase::createBuffer()
{
    // Attempts to create the buffer holding the vertex data.
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    if (!m_vertexBuffer->create() || !m_vertexBuffer->bind())
    {
        return cranError(ERRARG(e_01));
    }

    return true;
}


QPointF ShapeBase::findCenter(const QVector<QPointF>& points)
{
    QPointF center;
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


QPointF ShapeBase::findSize(const QVector<QPointF>& points)
{
    auto extremeX = std::minmax_element(
            points.begin(), points.end(),
            [] (const QPointF& l, const QPointF& r) {
                return l.x() < r.x();
            });

    auto extremeY = std::minmax_element(
            points.begin(), points.end(),
            [] (const QPointF& l, const QPointF& r) {
                return l.y() < r.y();
            });

    QPointF upperLeft(extremeX.first->x(), extremeY.first->y());
    QPointF lowerRight(extremeX.second->x(), extremeY.second->y());

    return lowerRight - upperLeft;
}


void ShapeBase::bindObjects()
{
    glDebug(m_vertexBuffer->bind());
    glDebug(shaderProgram()->program()->bind());

    if (!m_smooth)
    {
        glDebug(gl->glDisable(GL_MULTISAMPLE));
        glDebug(gl->glDisable(GL_LINE_SMOOTH));
    }
}


void ShapeBase::releaseObjects()
{
    glDebug(m_vertexBuffer->release());
    glDebug(shaderProgram()->program()->release());

    if (!m_smooth)
    {
        glDebug(gl->glEnable(GL_MULTISAMPLE));
        glDebug(gl->glEnable(GL_LINE_SMOOTH));
    }
}


void ShapeBase::writeVertices()
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
                if (singleColor)
                {
                    m_vertices[i].rgba(m_colorBuffer.at(0));
                }
                else
                {
                    m_vertices[i].rgba(m_colorBuffer.at(i));
                }
            }
        }

        // Uploads the vertex data.
        glDebug(m_vertexBuffer->write(
                GL_ZERO,
                m_vertices.data(),
                priv::Vertex::size() * vertexCount())
                );

        m_update = false;
        m_colorUpdate = false;
    }
}


void ShapeBase::modifyProgram()
{
    OpenGLShader* program = shaderProgram();

    glDebug(program->setMvpMatrix(matrix(this)));
    glDebug(program->setOpacity(opacity()));
    glDebug(program->setWindowSize(renderTarget()->size()));
}


void ShapeBase::modifyAttribs()
{
    glDebug(gl->glEnableVertexAttribArray(priv::Vertex::xyzAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::Vertex::rgbaAttrib()));

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


void ShapeBase::drawElements()
{
    uint mode = (m_filled)
              ? renderModeFilled()
              : renderModeWired();

    // We need to draw triangles for line widths bigger than 1.
    if (m_lineWidth > 1)
    {
        mode = GL_TRIANGLES;
    }

    glDebug(gl->glDrawArrays(mode, GL_ZERO, vertexCount()));
}


TreeModelItem* ShapeBase::rootModelItem()
{
    return m_rootModelItem;
}


void ShapeBase::createProperties(TreeModel* model)
{
    TreeModelItem* tmiColo = new TreeModelItem("Color", m_colorBuffer[0]);
    TreeModelItem* tmiFill = new TreeModelItem("Is filled?", m_filled);
    TreeModelItem* tmiUpda = new TreeModelItem("Requires update?", m_update || m_colorUpdate);
    TreeModelItem* tmiOpGL = new TreeModelItem("OpenGL");
    TreeModelItem* tmiVBuf = new TreeModelItem("Vertexbuffer", m_vertexBuffer->bufferId());

    m_rootModelItem = new TreeModelItem("ShapeBase");
    m_rootModelItem->appendChild(tmiColo);
    m_rootModelItem->appendChild(tmiFill);
    m_rootModelItem->appendChild(tmiUpda);
    m_rootModelItem->appendChild(tmiOpGL);

    tmiOpGL->appendChild(tmiVBuf);
    model->addItem(m_rootModelItem);

    RenderBase::createProperties(model);
}


void ShapeBase::updateProperties()
{
    m_rootModelItem->childAt(0)->setValue(m_colorBuffer[0]);
    m_rootModelItem->childAt(1)->setValue(m_filled);
    m_rootModelItem->childAt(2)->setValue(m_update || m_colorUpdate);
    m_rootModelItem->childAt(3)->childAt(0)->setValue(m_vertexBuffer->bufferId());

    RenderBase::updateProperties();
}


void ShapeBase::extrudeSegment(
    const QPointF& p0,
    const QPointF& p1,
    const QPointF& p2,
    const QPointF& p3
    )
{
    priv::Vertex v1, v2, v3, v4, v5, v6;
    QVector2D line = QVector2D(p2 - p1).normalized();
    QVector2D norm = QVector2D(-line.y(), line.x()).normalized();
    QVector2D tan1 = (p0 == p1) ? line : (QVector2D(p1 - p0).normalized() + line).normalized();
    QVector2D tan2 = (p2 == p3) ? line : (QVector2D(p3 - p2).normalized() + line).normalized();
    QVector2D mit1 = QVector2D(-tan1.y(), tan1.x());
    QVector2D mit2 = QVector2D(-tan2.y(), tan2.x());

    // Calculate the length of the miter.
    qreal len1 = m_lineWidth / QVector2D::dotProduct(norm, mit1);
    qreal len2 = m_lineWidth / QVector2D::dotProduct(norm, mit2);

    // Calculates the new points.
    QVector2D a1 = QVector2D(p1) - len1 * mit1;
    QVector2D a2 = QVector2D(p2) - len2 * mit2;
    QVector2D a3 = QVector2D(p1) + len1 * mit1;
    QVector2D a4 = QVector2D(p2) + len2 * mit2;

    // Generate the vertices.
    v1.xyz(a1.x() + c_magic, a1.y() + c_magic);
    v2.xyz(a3.x() + c_magic, a3.y() + c_magic);
    v3.xyz(a2.x() + c_magic, a2.y() + c_magic);
    v4.xyz(a2.x() + c_magic, a2.y() + c_magic);
    v5.xyz(a3.x() + c_magic, a3.y() + c_magic);
    v6.xyz(a4.x() + c_magic, a4.y() + c_magic);

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    m_vertices.push_back(v4);
    m_vertices.push_back(v5);
    m_vertices.push_back(v6);
}

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
#include <Cranberry/Graphics/Line.hpp>

// Qt headers
#include <QOpenGLBuffer>


CRANBERRY_BEGIN_NAMESPACE


const QVector3D& Line::lineStart() const
{
    return m_start;
}


const QVector3D& Line::lineEnd() const
{
    return m_end;
}


void Line::setLine(const QPointF& start, const QPointF& end)
{
    m_start = QVector3D(start);
    m_end = QVector3D(end);
    vertexBuffer()->allocate(sizeof(float) * 28);

    setPosition(QVector2D(start));
    setOrigin(QVector2D(end - start));

    vertices().clear();
    vertices().push_back(VxPrimitive());
    vertices().push_back(VxPrimitive());
    vertices().at(0).xyz(0.f, 0.f, 0.f);
    vertices().at(1).xyz(end.x() - start.x(), end.y() - start.y(), 0.f);
    requestUpdate();
}


void Line::setLine(const QVector2D& vector, const QPointF& start)
{
    setLine(QVector3D(vector), start);
}


void Line::setLine(const QVector3D& vector, const QPointF& start)
{
    m_start = vector;
    m_end = QVector3D(vector.x() + start.x(), vector.y() + start.y(), 0.f);
    vertexBuffer()->allocate(sizeof(float) * 28);

    setPosition(QVector2D(start));
    setOrigin(vector.toVector2D() / 2);

    if (vertices().size() == 0)
    {
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
    }

    vertices().at(0).xyz(0.f, 0.f, 0.f);
    vertices().at(1).xyz(vector.x(), vector.y(), vector.z());
    requestUpdate();
}


void Line::setColor(const QColor& c1, const QColor& c2)
{
    auto cf = c1, ce = c2;
    if (!c2.isValid())
    {
        ce = cf;
    }

    if (vertices().size() == 0)
    {
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
    }

    vertices().at(0).rgba(cf);
    vertices().at(1).rgba(ce);
    requestUpdate();
}


uint32_t Line::renderModeWired() const
{
    return DrawLines;
}


uint32_t Line::renderModeFilled() const
{
    return DrawLines;
}


CRANBERRY_END_NAMESPACE

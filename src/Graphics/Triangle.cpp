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
#include <Cranberry/Graphics/Triangle.hpp>
#include <Cranberry/Graphics/System/GraphicsConstants.hpp>

// Qt headers
#include <QOpenGLBuffer>


CRANBERRY_BEGIN_NAMESPACE


const QPointF& Triangle::pointA() const
{
    return m_a;
}


const QPointF& Triangle::pointB() const
{
    return m_b;
}


const QPointF& Triangle::pointC() const
{
    return m_c;
}


void Triangle::setTriangle(const QPointF& a, const QPointF& b, const QPointF& c)
{
    m_a = a;
    m_b = b;
    m_c = c;
    vertexBuffer()->allocate(PRIMITIVE_ALLOCATE(3));

    if (vertices().empty())
    {
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
    }

    vertices().at(0).xyz(a.x(), a.y(), 0.f);
    vertices().at(1).xyz(b.x(), b.y(), 0.f);
    vertices().at(2).xyz(c.x(), c.y(), 0.f);
    requestUpdate();
}


void Triangle::setColor(const QColor& color)
{
    setColor(color, color, color);
}


void Triangle::setColor(const QColor& ca, const QColor& cb, const QColor& cc)
{
    if (vertices().empty())
    {
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
        vertices().push_back(VxPrimitive());
    }

    vertices().at(0).rgba(ca);
    vertices().at(1).rgba(cb);
    vertices().at(2).rgba(cc);
    requestUpdate();
}


uint32_t Triangle::renderModeWired() const
{
    return DrawLineLoop;
}


uint32_t Triangle::renderModeFilled() const
{
    return DrawTriangles;
}


CRANBERRY_END_NAMESPACE

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
#include <Cranberry/Graphics/Shapes/Polygon.hpp>
#include <Cranberry/Graphics/System/GraphicsConstants.hpp>

// Qt headers
#include <QOpenGLBuffer>

// Standard headers
#include <cmath>
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


const std::vector<QPointF>& Polygon::points() const
{
    return m_points;
}


void Polygon::setVertices(const std::vector<QPointF>& points)
{
    m_points = points;
    vertices().clear();

    //  The following variables hold the extent of the object.
    float eX = 0, eY = 0;
    int n = points.size();

    // Specifies all vertices.
    for (int i = 0; i < n; i++)
    {
        float x = points.at(i).x();
        float y = points.at(i).y();

        vertices().push_back(VxPrimitive());
        vertices().at(i).xyz(x, y, 0.f);
        eX += x;
        eY += y;
    }

    setOrigin(QVector2D(eX, eY));
    vertexBuffer()->allocate(PRIMITIVE_ALLOCATE(vertices().size()));
    requestUpdate();
}


void Polygon::setVertices(float size, uint32_t sides)
{
    float radius = (size / 2.f);
    float cX = radius, cY = radius;
    float eX = cX, eY = cY;

    m_points.clear();
    vertices().clear();
    vertices().push_back(VxPrimitive());
    vertices().at(0).xyz(cX + radius * cosf(0), cY + radius * sinf(0), 0.f);

    // Generates the vertices.
    for (uint32_t i = 1; i <= sides; i++)
    {
        float cos = cX + radius * cosf((2*i) * (M_PI / sides));
        float sin = cY + radius * sinf((2*i) * (M_PI / sides));

        m_points.push_back(QPointF(cos, sin));
        vertices().push_back(VxPrimitive());
        vertices().at(i).xyz(cos, sin, 0.f);
        eX += cos;
        eY += sin;
    }

    setOrigin(QVector2D(eX / sides, eY / sides));
    vertexBuffer()->allocate(PRIMITIVE_ALLOCATE(vertices().size()));
    requestUpdate();
}


void Polygon::setColor(const QColor& color)
{
    if (vertices().empty())
    {
        std::cout << "Cranberry Polygon: Can't set colors. No vertices." << std::endl;
        return;
    }

    // Sets the given color for all vertices.
    for (size_t i = 0; i < vertices().size(); i++)
        vertices().at(i).rgba(color);
}


void Polygon::setColor(const std::vector<QColor>& colors)
{
    if (vertices().empty())
    {
        std::cout << "Cranberry Polygon: Can't set colors. No vertices." << std::endl;
        return;
    }
    else if (vertices().size() != colors.size())
    {
        std::cout << "Cranberry Polygon: Can't set colors. "
                  << "Vertex count is not the same as color count." << std::endl;
        return;
    }

    // One color for one vertex.
    for (size_t i = 0; i < vertices().size(); i++)
        vertices().at(i).rgba(colors.at(i));
}


uint32_t Polygon::renderModeWired() const
{
    return DrawLineLoop;
}


uint32_t Polygon::renderModeFilled() const
{
    return DrawTriangleFan;
}


CRANBERRY_END_NAMESPACE

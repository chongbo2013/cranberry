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
#include <Cranberry/Graphics/Shapes/Ellipse.hpp>
#include <Cranberry/Graphics/System/GraphicsConstants.hpp>

// Qt headers
#include <QOpenGLBuffer>

// Standard headers
#include <cmath>
#include <iostream>


CRANBERRY_BEGIN_NAMESPACE


const std::vector<QPointF>& Ellipse::points() const
{
    return m_points;
}


void Ellipse::setEllipse(float radius1, float radius2, float beg, float end)
{
    // Converts the degrees to radians.
    float move = end - beg;
    float srad = beg  * (M_PI / 180.f);
    float arad = move * (M_PI / 180.f);
    float radii = radius1 + radius2;

    // Calculates the amount of segments (vertices).
    uint32_t segments = static_cast<uint32_t>(radii * (move / 100.f));
    float radSegments = arad / segments;

    // Pre-calculates variables for the algorithm.
    float tan = 0.f;
    float csin = sinf(radSegments);
    float ccos = cosf(radSegments);
    float x = radii * cosf(srad);
    float y = radii * sinf(srad);
    float eX = 0.f, eY = 0.f;

    // Computes the vertices. Rotates the shape instead of
    // calculating the sine and cosine again and again.
    for (uint32_t i = 0; i < segments; i++)
    {
        VxPrimitive v;
        v.xyz(x+radii, y+radii, 0.f);
        vertices().push_back(v);
        m_points.push_back(QPointF(x+radii, y+radii));

        eX = qMax(eX, x+radii);
        eY = qMax(eY, y+radii);

        tan = x;
        x = (ccos*x) - (csin*y);
        y = (csin*tan) + (ccos*y);
    }

    setOrigin(QVector2D(eX / 2, eY / 2));
    vertexBuffer()->allocate(PRIMITIVE_ALLOCATE(segments));
    m_isArc = (move != 360);
    requestUpdate();
}


void Ellipse::setColor(const QColor& color)
{
    if (vertices().empty())
    {
        std::cout << "Cranberry Ellipse: Can't set colors. No vertices." << std::endl;
        return;
    }

    // Sets the given color for all vertices.
    for (size_t i = 0; i < vertices().size(); i++)
        vertices().at(i).rgba(color);
}


void Ellipse::setColor(const std::vector<QColor>& colors)
{
    if (vertices().empty())
    {
        std::cout << "Cranberry Ellipse: Can't set colors. No vertices." << std::endl;
        return;
    }
    else if (vertices().size() != colors.size())
    {
        std::cout << "Cranberry Ellipse: Can't set colors. "
                  << "Vertex count is not the same as color count." << std::endl;
        return;
    }

    // One color for one vertex.
    for (size_t i = 0; i < vertices().size(); i++)
        vertices().at(i).rgba(colors.at(i));
}


uint32_t Ellipse::renderModeWired() const
{
    if (m_isArc)
        return DrawLineStrip;
    else
        return DrawLineLoop;
}


uint32_t Ellipse::renderModeFilled() const
{
    return DrawTriangleFan;
}


CRANBERRY_END_NAMESPACE

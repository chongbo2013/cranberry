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
#include <Cranberry/Graphics/Ellipse.hpp>

// Qt headers
#include <QtMath>


CRANBERRY_USING_NAMESPACE


bool Ellipse::create(
        qreal radius1,
        qreal radius2,
        qreal beginAngle,
        qreal endAngle,
        Window *renderTarget
        )
{
    QVector<QVector2D> points;

    qreal advance = endAngle - beginAngle;
    qreal radii = radius1 + radius2;

    // Calculates the amount of vertices dynamically.
    uint segments = static_cast<uint>(radii * (advance / 100.f));
    qreal segRad =  qDegreesToRadians(advance) / segments;

    // Pre-calculates sine and cosine for the algorithm.
    qreal currentY = 0.0;
    qreal currentX = 1.0;
    qreal preTan = 0.0;
    qreal preSin = qSin(segRad);
    qreal preCos = qCos(segRad);

    // Computes the vertices.
    for (uint i = 0; i < segments; i++)
    {
        qreal x = currentX * radius1 + radius1;
        qreal y = currentY * radius2 + radius2;

        points.push_back(QVector2D(x, y));
        preTan = currentX;
        currentX = ((preCos * currentX) - (preSin * currentY));
        currentY = ((preSin * preTan) + (preCos * currentY));
    }

    m_isArc = (advance != 360);

    return createInternal(points, renderTarget);
}


uint Ellipse::renderModeFilled() const
{
    return (m_isArc) ? RenderNone : RenderTriangleFan;
}


uint Ellipse::renderModeWired() const
{
    return (m_isArc) ? RenderLineStrip : RenderLineLoop;
}

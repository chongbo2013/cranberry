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
#include <Cranberry/Graphics/Polygon.hpp>

// Qt headers
#include <QtMath>


CRANBERRY_USING_NAMESPACE


bool pointsEqual(const QPointF& p1, const QPointF& p2)
{
    return abs(p1.x() - p2.x()) < 0.1 &&
           abs(p1.y() - p2.y()) < 0.1;
}


bool Polygon::create(const QVector<QPointF>& points, Window* renderTarget)
{
    m_isClosed = pointsEqual(points.first(), points.last());

    return createInternal(points, renderTarget);
}


bool Polygon::create(float radius, uint sides, Window* renderTarget)
{
    QVector<QPointF> points;
    qreal cX = radius, cY = radius;
    qreal spi = M_PI / sides;

    // Generates the vertices.
    for (uint i = 1; i <= sides; i++)
    {
        qreal c = cX + radius * qCos(2 * i * spi);
        qreal s = cY + radius * qSin(2 * i * spi);

        points.append(QPointF(c, s));
    }

    m_isClosed = true;

    return createInternal(points, renderTarget);
}


uint Polygon::renderModeWired() const
{
    return RenderLineLoop;
}


uint Polygon::renderModeFilled() const
{
    return RenderTriangleFan;
}


bool Polygon::isShapeClosed() const
{
    return m_isClosed;
}

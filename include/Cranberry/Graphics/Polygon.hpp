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


#pragma once
#ifndef CRANBERRY_POLYGON_HPP
#define CRANBERRY_POLYGON_HPP


// Cranberry headers
#include <Cranberry/Graphics/Primitive.hpp>

// Qt headers

// Standard headers


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a polygon with multiple vertices.
///
/// \class Polygon
/// \author Nicolas Kogler
/// \date March 7, 2017
///
class CRANBERRY_EXPORT Polygon : public Primitive
{
public:

    CRANBERRY_DISABLE_MOVE(Polygon)
    CRANBERRY_DEFAULT_CTOR(Polygon)
    CRANBERRY_DEFAULT_DTOR(Polygon)
    CRANBERRY_DEFAULT_COPY(Polygon)


    ///
    /// Retrieves all points of the polygon.
    ///
    /// \returns all points.
    ///
    const std::vector<QPointF>& points() const;


    ///
    /// Specifies all vertices by a list of points.
    ///
    /// \param points Points in 2D space.
    ///
    void setVertices(const std::vector<QPointF>& points);

    ///
    /// Generates a regular polygon with \p sides.
    ///
    /// \param size Size of the entire object in pixels.
    /// \param sides Amount of sides of the polygon.
    ///
    void setVertices(float size, uint32_t sides);

    ///
    /// Paints the polygon in one single \p color.
    ///
    /// \param color The color to fill the polygon in.
    ///
    void setColor(const QColor& color);

    ///
    /// Specifies the colors of all vertices.
    ///
    /// \param colors All the colors.
    ///
    void setColor(const std::vector<QColor>& colors);


protected:

    uint32_t renderModeWired() const override;
    uint32_t renderModeFilled() const override;

private:

    // Members
    std::vector<QPointF> m_points;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_POLYGON_HPP

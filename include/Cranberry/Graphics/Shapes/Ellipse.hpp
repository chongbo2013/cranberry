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
#ifndef CRANBERRY_ELLIPSE_HPP
#define CRANBERRY_ELLIPSE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Primitive.hpp>

// Standard headers
#include <vector>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines an ellipse. The color of the vertices may only
/// be set \em after setVertices was called. If the radii
/// are the same, a circle will be displayed.
///
/// \class Ellipse
/// \author Nicolas Kogler
/// \date March 8, 2017
///
class CRANBERRY_EXPORT Ellipse : public Primitive
{
public:

    CRANBERRY_DISABLE_MOVE(Ellipse)
    CRANBERRY_DEFAULT_CTOR(Ellipse)
    CRANBERRY_DEFAULT_DTOR(Ellipse)
    CRANBERRY_DEFAULT_COPY(Ellipse)


    ///
    /// Retrieves all points of this ellipse.
    ///
    /// \returns all points.
    ///
    const std::vector<QPointF>& points() const;


    ///
    /// Specifies the ellipse. If \p radius1 equals \p radius2,
    /// a circle will be displayed. Change \p beg and \p end
    /// respectively in order to only show a specific region
    /// (i.e. an arc) of the ellipse/circle.
    ///
    /// \param radius1 First radius (in deg) of the ellipse.
    /// \param radius2 Second radius (in deg) of the ellipse.
    /// \param beg Arc begin (defaults to 0).
    /// \param end Arc end (defaults to 360).
    ///
    void setEllipse(float radius1, float radius2, float beg = 0, float end = 360);

    ///
    /// Paints the ellipse in one single \p color. Do not
    /// call this function before setEllipse() was used.
    ///
    /// \param color The color to fill the polygon in.
    ///
    void setColor(const QColor& color);

    ///
    /// Specifies the colors of all vertices. Do not
    /// call this function before setEllipse() was used.
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
    bool m_isArc;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_ELLIPSE_HPP

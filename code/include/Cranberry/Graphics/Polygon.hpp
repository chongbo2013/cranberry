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


#pragma once
#ifndef CRANBERRY_POLYGON_HPP
#define CRANBERRY_POLYGON_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/ShapeBase.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a polygon with multiple vertices.
///
/// \class Polygon
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Polygon : public ShapeBase
{
public:

    CRANBERRY_DEFAULT_CTOR(Polygon)
    CRANBERRY_DEFAULT_DTOR(Polygon)
    CRANBERRY_DISABLE_COPY(Polygon)
    CRANBERRY_DISABLE_MOVE(Polygon)


    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new polygon with the given \p points.
    ///
    /// \param points All points of the polygon.
    /// \param renderTarget Target to render polygon on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QVector<QVector2D>& points, Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Generates a new polygon with the given \p radius and the given amount
    /// of \p sides (e.g. "6" would be a hexagon).
    ///
    /// \param radius The radius of the entire object.
    /// \param sides The amount of sides to generate.
    /// \param renderTarget Target to render polygon on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(float radius, uint sides, Window* renderTarget = nullptr);


protected:

    uint renderModeWired() const override;
    uint renderModeFilled() const override;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Polygon
/// \ingroup Graphics
///
/// Capable of drawing polygons.
///
/// \code
/// cran::Polygon hexagon;
/// hexagon.create(150.f, 6);
/// hexagon.setColor(QColor(Qt::red));
///
/// ...
///
/// hexagon.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

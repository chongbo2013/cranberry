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
#ifndef CRANBERRY_TRIANGLE_HPP
#define CRANBERRY_TRIANGLE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Primitive.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a triangle by three points A, B and C.
///
/// \class Triangle
/// \author Nicolas Kogler
/// \date March 5, 2017
///
class CRANBERRY_EXPORT Triangle : public Primitive
{
public:

    CRANBERRY_DISABLE_MOVE(Triangle)
    CRANBERRY_DEFAULT_CTOR(Triangle)
    CRANBERRY_DEFAULT_DTOR(Triangle)
    CRANBERRY_DEFAULT_COPY(Triangle)


    ///
    /// Retrieves the first point of the triangle.
    ///
    /// \returns the first point.
    ///
    const QPointF& pointA() const;

    ///
    /// Retrieves the second point of the triangle.
    ///
    /// \returns the second point.
    ///
    const QPointF& pointB() const;
    ///
    /// Retrieves the third point of the triangle.
    ///
    /// \returns the third point.
    ///
    const QPointF& pointC() const;


    ///
    /// Specifies the triangle by three points A, B and C.
    ///
    /// \param a First point.
    /// \param b Second point.
    /// \param c Third point.
    ///
    void setTriangle(const QPointF& a, const QPointF& b, const QPointF& c);

    ///
    /// Specifies a single color for all vertices.
    ///
    /// \param color Color of A, B and C.
    ///
    void setColor(const QColor& color);

    ///
    /// Specifies the color of all three vertices.
    ///
    /// \param ca Color of point A.
    /// \param cb Color of point B.
    /// \param cc Color of point C.
    ///
    void setColor(const QColor& ca, const QColor& cb, const QColor& cc);


protected:

    uint32_t renderModeWired() const override;
    uint32_t renderModeFilled() const override;


private:

    // Members
    QPointF m_a;
    QPointF m_b;
    QPointF m_c;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_TRIANGLE_HPP

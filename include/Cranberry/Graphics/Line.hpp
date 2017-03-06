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
#ifndef CRANBERRY_LINE_HPP
#define CRANBERRY_LINE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Primitive.hpp>

// Qt headers
#include <QVector3D>
#include <QVector2D>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines one line from A to B.
///
/// \class Line
/// \author Nicolas Kogler
/// \date March 4, 2017
///
class CRANBERRY_EXPORT Line : public Primitive
{
public:

    CRANBERRY_DISABLE_MOVE(Line)
    CRANBERRY_DEFAULT_CTOR(Line)
    CRANBERRY_DEFAULT_DTOR(Line)
    CRANBERRY_DEFAULT_COPY(Line)


    ///
    /// Retrieves the start of the line.
    ///
    /// \returns the line start.
    ///
    const QVector3D& lineStart() const;

    ///
    /// Retrieves the end of the line.
    ///
    /// \returns the line end.
    ///
    const QVector3D& lineEnd() const;


    ///
    /// Specifies the line by setting a starting- and ending point.
    ///
    /// \param start Start of the line. Equal to setPosition(start).
    /// \param end End of the line.
    ///
    void setLine(const QPointF& start, const QPointF& end);

    ///
    /// Specifies the line by a vector starting at \p start.
    ///
    /// \param vector Mathematical vector that represents the line.
    /// \param start Starting location of the vector.
    ///
    void setLine(const QVector2D& vector, const QPointF& start = QPoint());

    ///
    /// Specifies the line by a vector starting at \p start.
    ///
    /// \param vector Mathematical vector that represents the line.
    /// \param start Starting location of the vector.
    ///
    void setLine(const QVector3D& vector, const QPointF& start = QPoint());

    ///
    /// Specifies the vertex colors of the line. If the second
    /// parameter stays default, \p c1 will be used for both
    /// vertices.
    ///
    /// \param c1 Entire line color or color of the starting point.
    /// \param c2 Ending point color.
    ///
    void setColor(const QColor& c1, const QColor& c2 = QColor());


protected:

    uint32_t renderModeWired() const override;
    uint32_t renderModeFilled() const override;

private:

    // Members
    QVector3D m_start;
    QVector3D m_end;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_LINE_HPP

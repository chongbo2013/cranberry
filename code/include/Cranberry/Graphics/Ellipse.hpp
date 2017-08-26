////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#ifndef CRANBERRY_ELLIPSE_HPP
#define CRANBERRY_ELLIPSE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/ShapeBase.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines an ellipse. Can be used as circle if the radii are the same.
///
/// \class Ellipse
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Ellipse final : public ShapeBase
{
public:

    CRANBERRY_DEFAULT_CTOR(Ellipse)
    CRANBERRY_DEFAULT_DTOR(Ellipse)
    CRANBERRY_DISABLE_COPY(Ellipse)
    CRANBERRY_DISABLE_MOVE(Ellipse)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new ellipse. If \p radius1 equals \p radius2, a circle will
    /// be displayed. Change \p beginAngle and \p endAngle respectively in order
    /// to only show a specific portion (i.e. an arc) of the ellipse/circle.
    ///
    /// \param radius1 First radius (in degrees).
    /// \param radius2 Second radius (in degrees).
    /// \param beginAngle Begin of the arc (in degrees).
    /// \param endAngle End of the arc (in degrees).
    /// \param renderTarget Target to render ellipse on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(
            qreal radius1,
            qreal radius2,
            qreal beginAngle = 0,
            qreal endAngle = 360,
            Window* renderTarget = nullptr
            );


protected overridden:

    uint renderModeWired() const override;
    uint renderModeFilled() const override;
    bool isShapeClosed() const override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    bool m_isArc;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Ellipse
/// \ingroup Graphics
///
/// Renders an ellipse, a circle or an arc.
///
/// \code
/// Ellipse arc;
/// arc.create(50, 50, 180, 360);
///
/// ...
///
/// arc.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

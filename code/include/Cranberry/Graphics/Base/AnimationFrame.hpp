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
#ifndef CRANBERRY_GRAPHICS_BASE_ANIMATIONFRAME_HPP
#define CRANBERRY_GRAPHICS_BASE_ANIMATIONFRAME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QRectF>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// The AnimationFrame class holds one frame containing rectangle, duration,
/// frame number and the index of the atlas in which it is embedded.
///
/// \class AnimationFrame
/// \author Nicolas Kogler
/// \date July 21, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT AnimationFrame final
{
public:

    CRANBERRY_DECLARE_CTOR(AnimationFrame)
    CRANBERRY_DEFAULT_DTOR(AnimationFrame)
    CRANBERRY_DEFAULT_COPY(AnimationFrame)
    CRANBERRY_DEFAULT_MOVE(AnimationFrame)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the frame rectangle.
    ///
    /// \returns the frame rectangle.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QRectF& rectangle() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the frame duration.
    ///
    /// \returns the frame duration.
    ///
    ////////////////////////////////////////////////////////////////////////////
    qreal duration() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the frame number.
    ///
    /// \returns the frame number.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int frameId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the texture atlas number.
    ///
    /// \returns the atlas number.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int atlasId() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the frame rectangle.
    ///
    /// \param rect The rectangle, in real coordinates.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRectangle(const QRectF& rect);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the frame rectangle.
    ///
    /// \param x X-position in pixels.
    /// \param y Y-position in pixels.
    /// \param width Width in pixels.
    /// \param height Height in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRectangle(qreal x, qreal y, qreal width, qreal height);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the duration of this frame, in seconds.
    ///
    /// \param duration Duration of the frame.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setDuration(qreal duration);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the frame number.
    ///
    /// \param id Number of the frame.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setFrameId(int id);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the index of the parent atlas.
    ///
    /// \param id Index of the atlas.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setAtlasId(int id);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QRectF m_rect;     ///< Frame rectangle
    qreal  m_duration; ///< Frame duration in seconds
    int    m_frameId;  ///< Frame number
    int    m_atlasId;  ///< Parent atlas
};


////////////////////////////////////////////////////////////////////////////////
/// \class AnimationFrame
/// \ingroup Graphics
///
/// You will usually only need this class when constructing raw animations.
///
/// \code
/// AnimationFrame frame;
/// frame.setRectangle(0, 0, 200, 200);
/// frame.setDuration(60.0 / 1000.0); // milliseconds <> seconds
/// frame.setFrameId(0);
/// frame.setAtlasId(0);
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

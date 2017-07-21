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


#pragma once
#ifndef CRANBERRY_RAWANIMATION_HPP
#define CRANBERRY_RAWANIMATION_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/IAnimation.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders a raw animation.
///
/// \class RawAnimation
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT RawAnimation final : public IAnimation
{
public:

    CRANBERRY_DEFAULT_CTOR(RawAnimation)
    CRANBERRY_DEFAULT_DTOR(RawAnimation)
    CRANBERRY_DISABLE_COPY(RawAnimation)
    CRANBERRY_DISABLE_MOVE(RawAnimation)


    ////////////////////////////////////////////////////////////////////////////
    /// This method is unused.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& path, Window* renderTarget = nullptr) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a raw animation from a given set of frames and durations.
    ///
    /// \param frames Frames of the animation.
    /// \param durations Duration of each frame.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool createRawAnimation(
            const QVector<QImage>& frames,
            const QVector<qreal>& durations,
            Window* renderTarget = nullptr
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a raw animation from a given set of  images and frames.
    ///
    /// \param images Images of the animation.
    /// \param frames Frames of the animation.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool createRawAnimation(
            const QVector<QImage> &images,
            const QVector<Frame> &frames,
            Window *renderTarget = nullptr
            );
};


////////////////////////////////////////////////////////////////////////////////
/// \class RawAnimation
/// \ingroup Graphics
///
/// Plays raw animations.
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

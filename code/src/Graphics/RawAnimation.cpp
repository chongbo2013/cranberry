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
#include <Cranberry/Graphics/RawAnimation.hpp>
#include <Cranberry/System/Debug.hpp>


CRANBERRY_USING_NAMESPACE


bool RawAnimation::create(const QString&, Window*)
{
    return cranError("This method is unused!");
}


bool RawAnimation::createRawAnimation(
        const QVector<QImage> &frames,
        const QVector<qreal> &durations,
        Window* renderTarget
        )
{
    return createInternal(frames, durations, renderTarget);
}


bool RawAnimation::createRawAnimation(
        const QVector<QImage>& images,
        const QVector<Frame>& frames,
        Window* renderTarget
        )
{
    return createInternal(images, frames, renderTarget);
}

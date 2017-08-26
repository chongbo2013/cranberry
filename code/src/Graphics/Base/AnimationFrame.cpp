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


// Cranberry headers
#include <Cranberry/Graphics/Base/AnimationFrame.hpp>


CRANBERRY_USING_NAMESPACE


AnimationFrame::AnimationFrame()
    : m_rect(0.0, 0.0, 0.0, 0.0)
    , m_duration(0.0)
    , m_frameId(-1)
    , m_atlasId(-1)
{
}


const QRectF& AnimationFrame::rectangle() const
{
    return m_rect;
}


qreal AnimationFrame::duration() const
{
    return m_duration;
}


int AnimationFrame::frameId() const
{
    return m_frameId;
}


int AnimationFrame::atlasId() const
{
    return m_atlasId;
}


void AnimationFrame::setRectangle(const QRectF& rect)
{
    m_rect = rect;
}


void AnimationFrame::setRectangle(qreal x, qreal y, qreal width, qreal height)
{
    m_rect = QRectF(x, y, width, height);
}


void AnimationFrame::setDuration(qreal duration)
{
    m_duration = duration;
}


void AnimationFrame::setFrameId(int id)
{
    m_frameId = id;
}


void AnimationFrame::setAtlasId(int id)
{
    m_atlasId = id;
}

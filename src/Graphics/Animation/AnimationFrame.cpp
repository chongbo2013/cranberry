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


// Cranberry headers
#include <Cranberry/Graphics/Animation/AnimationFrame.hpp>


CRANBERRY_BEGIN_NAMESPACE


AnimationFrame::AnimationFrame()
    : m_duration(0.5)
    , m_frameIndex(0)
    , m_textureIndex(0)
{

}


AnimationFrame::AnimationFrame(const AnimationFrame& other)
    : m_duration(other.m_duration)
    , m_frameIndex(other.m_frameIndex)
    , m_textureIndex(other.m_textureIndex)
    , m_textureRect(other.m_textureRect)
{
}


AnimationFrame::AnimationFrame(AnimationFrame&& other)
    : m_duration(std::move(other.m_duration))
    , m_frameIndex(std::move(other.m_frameIndex))
    , m_textureIndex(std::move(other.m_textureIndex))
    , m_textureRect(std::move(other.m_textureRect))
{
}


AnimationFrame& AnimationFrame::operator =(const AnimationFrame& other)
{
    m_duration = other.m_duration;
    m_frameIndex = other.m_frameIndex;
    m_textureIndex = other.m_textureIndex;
    m_textureRect = other.m_textureRect;

    return *this;
}


qreal AnimationFrame::duration() const
{
    return m_duration;
}


int32_t AnimationFrame::frameIndex() const
{
    return m_frameIndex;
}


int32_t AnimationFrame::textureIndex() const
{
    return m_textureIndex;
}


const QRectF& AnimationFrame::textureRect() const
{
    return m_textureRect;
}


void AnimationFrame::setDuration(qreal seconds)
{
    m_duration = seconds;
}


void AnimationFrame::setFrameIndex(int32_t index)
{
    m_frameIndex = index;
}


void AnimationFrame::setTextureIndex(int32_t index)
{
    m_textureIndex = index;
}


void AnimationFrame::setTextureRect(const QRectF& rect)
{
    m_textureRect = rect;
}


CRANBERRY_END_NAMESPACE

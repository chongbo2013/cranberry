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
#include <Cranberry/Graphics/RawAnimation.hpp>
#include <Cranberry/Graphics/Base/SpriteMovement.hpp>


CRANBERRY_USING_NAMESPACE


SpriteMovement::SpriteMovement()
    : m_name("{no_name}")
    , m_rect(0.0, 0.0, 0.0, 0.0)
    , m_advanceX(0.0)
    , m_advanceY(0.0)
    , m_mode(MovementDefault)
    , m_anim(nullptr)
{
}


SpriteMovement::~SpriteMovement()
{
    delete m_anim;
}


const QString& SpriteMovement::name() const
{
    return m_name;
}


const QRectF& SpriteMovement::idleFrame() const
{
    return m_rect;
}


qreal SpriteMovement::horizontalAdvance() const
{
    return m_advanceX;
}


qreal SpriteMovement::verticalAdvance() const
{
    return m_advanceY;
}

qreal SpriteMovement::totalDuration() const
{
    return m_duration;
}


MovementMode SpriteMovement::mode() const
{
    return m_mode;
}


RawAnimation* SpriteMovement::animation() const
{
    return m_anim;
}


void SpriteMovement::setName(const QString& name)
{
    m_name = name;
}


void SpriteMovement::setIdleFrame(const QRectF& frame)
{
    m_rect = frame;
}


void SpriteMovement::setIdleFrame(qreal x, qreal y, qreal width, qreal height)
{
    m_rect = QRectF(x, y, width, height);
}


void SpriteMovement::setHorizontalAdvance(qreal amount)
{
    m_advanceX = amount;
}


void SpriteMovement::setVerticalAdvance(qreal amount)
{
    m_advanceY = amount;
}


void SpriteMovement::setTotalDuration(qreal total)
{
    m_duration = total;
}


void SpriteMovement::setMovementMode(MovementMode mode)
{
    m_mode = mode;
}


void SpriteMovement::setRawAnimation(RawAnimation* anim)
{
    m_anim = anim;
}

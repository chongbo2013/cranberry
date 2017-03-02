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
#include <Cranberry/Graphics/System/Fadable.hpp>


CRANBERRY_BEGIN_NAMESPACE


Fadable::Fadable()
    : m_isFading(false)
    , m_dirFade(0)
    , m_speedFade(1.f)
    , m_opacity(1.f)
{
}


bool Fadable::isFading() const
{
    return m_isFading;
}


float Fadable::fadeSpeed() const
{
    return m_speedFade;
}


float Fadable::opacity() const
{
    return m_opacity;
}


void Fadable::setFadeSpeed(float speed)
{
    m_speedFade = speed;
}


void Fadable::setOpacity(float opacity)
{
    m_opacity = opacity;
}


void Fadable::setFadeDirection(FadeDirection dir)
{
    m_dirFade = static_cast<int>(dir);
}


void Fadable::startFade(float targetOpacity)
{
    m_targetOpacity = targetOpacity;
    m_isFading = true;
}


void Fadable::updateFade(const GameTime& time)
{
    if (m_isFading)
    {
        // More visible
        if (m_dirFade == FadeIn)
        {
            m_opacity += (m_speedFade * time.deltaTime());
            if (m_opacity >= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                m_isFading = false;
            }
        }

        // Less visible
        else
        {
            m_opacity -= (m_speedFade * time.deltaTime());
            if (m_opacity <= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                m_isFading = false;
            }
        }
    }
}


CRANBERRY_END_NAMESPACE

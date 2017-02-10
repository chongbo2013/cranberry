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
#include <Cranberry/System/GameTime.hpp>

// Standard headers
#include <ctime>


CRANBERRY_BEGIN_NAMESPACE


// Global variables
clock_t g_previousTicks;
clock_t g_currentTicks;


GameTime::GameTime()
    : m_delta(0.0)
{
}


const QDateTime& GameTime::totalTime() const
{
    return m_total;
}


double GameTime::deltaTime() const
{
    return m_delta;
}


void GameTime::update()
{
    g_currentTicks = clock();
    m_delta = (g_currentTicks - g_previousTicks) / CLOCKS_PER_SEC;
    m_total = m_total.addMSecs(m_delta * 1000);
    g_previousTicks = g_currentTicks;
}


CRANBERRY_END_NAMESPACE

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
#include <Cranberry/System/cbGameTime.hpp>

// Standard headers
#include <ctime>


CRANBERRY_BEGIN_NAMESPACE


// Variables
clock_t g_current;
clock_t g_elapsed;


const QTime&
cbGameTime::totalTime() const
{
    return m_totalTime;
}


double
cbGameTime::deltaTime() const
{
    return m_deltaTime;
}


void
cbGameTime::updateTime()
{
    m_deltaTime = computeDelta();
    m_totalTime = m_totalTime.addMSecs(m_deltaTime * 1000);
}


double
cbGameTime::computeDelta()
{
    g_current = clock();

    double delta = g_current - g_elapsed;
    double secs  = delta / CLOCKS_PER_SEC;

    g_elapsed = g_current;

    return secs;
}


CRANBERRY_END_NAMESPACE

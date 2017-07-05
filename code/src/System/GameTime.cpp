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
#include <Cranberry/System/GameTime.hpp>


CRANBERRY_USING_NAMESPACE


GameTime::GameTime()
    : m_total(timepoint::duration(0))
    , m_previous(timepoint::duration(0))
    , m_current(timepoint::duration(0))
    , m_null(timepoint::duration(0))
    , m_delta(0.0)
{
}


int GameTime::totalHours() const
{
    return std::chrono::duration_cast<std::chrono::hours>(m_total - m_null).count();
}


int GameTime::totalMinutes() const
{
    return std::chrono::duration_cast<std::chrono::minutes>(m_total - m_null).count();
}


int GameTime::totalSeconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(m_total - m_null).count();
}


double GameTime::deltaTime() const
{
    return m_delta;
}


void GameTime::update()
{
    m_current = clock::now();
    m_delta = std::chrono::duration_cast<ms>(m_current - m_previous).count() / 1000.0;
    m_total += (m_current - m_previous);
    m_previous = m_current;
}

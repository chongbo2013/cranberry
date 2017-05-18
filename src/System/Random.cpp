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
#include <Cranberry/System/Random.hpp>

// Standard headers


CRANBERRY_BEGIN_NAMESPACE


// 7-bit ASCII (printable characters).
constexpr char ascii[] = " !#$%&()*+'-./0123456789:;<=>?@"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                         "abcdefghijklmnopqrstuvwxyz{|}~";


Random::Random()
    : m_engine(std::time(0))
    , m_charset(ascii)
    , m_fmin(0.0)
    , m_fmax(1.0)
    , m_imin(0)
    , m_imax(std::numeric_limits<int>::max())
{
}


void Random::setSeed(unsigned int seed)
{
    m_engine.seed(seed);
}


void Random::setMinMax(int min, int max)
{
    m_imin = min;
    m_imax = max;
}


void Random::setMinMax(double min, double max)
{
    m_fmin = min;
    m_fmax = max;
}


void Random::setCharset(const QString& charset)
{
    m_charset = charset;
}


void Random::setDistribution(const std::discrete_distribution<> &dist)
{
    m_distrib = dist;
}


int Random::nextNumber() const
{
    return m_imin + m_engine() % (m_imax+1 - m_imin);
}


int Random::nextDiscrete() const
{
    return m_distrib(m_engine);
}


double Random::nextDouble() const
{
    double factor = static_cast<double>(m_engine.max()) / (m_fmax - m_fmin);
    return m_fmin + static_cast<double>(m_engine()) / factor;
}


QString Random::nextString(int size) const
{
    QString result;
    int max = m_charset.size();

    // Chooses random characters, capped by MAX.
    for (int i = 0; i < size; i++)
    {
        int value = m_engine() % max;
        result += m_charset.at(value);
    }

    return result;
}


CRANBERRY_END_NAMESPACE

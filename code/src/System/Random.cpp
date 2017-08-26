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
#include <Cranberry/System/Random.hpp>

// Standard headers
#include <ctime>


CRANBERRY_USING_NAMESPACE


namespace
{
    // Default charset (ASCII printable characters)
    constexpr char c_ascii[] = " !#$%&()*+'-./0123456789:;<=>?@"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                               "abcdefghijklmnopqrstuvwxyz{|}~";
}


Random::Random()
    : m_engine(std::time(0))
    , m_charset(c_ascii)
    , m_fmin(0.0)
    , m_fmax(1.0)
    , m_imin(0)
    , m_imax(std::numeric_limits<int>::max())
{
}


void Random::setSeed(uint seed)
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


void Random::setDistribution(const Distribution& dist)
{
    m_dist = dist;
}


bool Random::nextBoolean() const
{
    double factor = static_cast<double>(m_engine.max());
    return static_cast<bool>(round(static_cast<double>(m_engine()) / factor));
}


int Random::nextNumber() const
{
    return m_imin + m_engine() % (m_imax+1 - m_imin);
}


int Random::nextDiscrete() const
{
    return m_dist(m_engine);
}


double Random::nextDouble() const
{
    double factor = static_cast<double>(m_engine.max()) / (m_fmax - m_fmin);
    return m_fmin + static_cast<double>(m_engine()) / factor;
}


const QString Random::nextString(int size) const
{
    QString result;
    int max = m_charset.size();

    // Chooses random characters, capped by 'max'.
    for (int i = 0; i < size; i++)
    {
        int value = m_engine() % max;
        result.append(m_charset.at(value));
    }

    return result;
}


const QByteArray Random::nextBlob(int size) const
{
    QByteArray result;
    for (int i = 0; i < size; i++)
    {
        uchar value = static_cast<uchar>(m_engine() % 256);
        result.append((char) value);
    }

    return result;
}

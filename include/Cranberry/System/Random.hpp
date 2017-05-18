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


#pragma once
#ifndef CRANBERRY_RANDOM_HPP
#define CRANBERRY_RANDOM_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QString>

// Standard headers
#include <ctime>
#include <random>


CRANBERRY_BEGIN_NAMESPACE


///
/// Is able to generate random numbers from either
/// a min/max value or a discrete distribution table.
///
/// \class Random
/// \author Nicolas Kogler
/// \date May 16, 2017
///
class CRANBERRY_EXPORT Random
{
public:

    CRANBERRY_DISABLE_COPY(Random)
    CRANBERRY_DISABLE_MOVE(Random)
    CRANBERRY_DEFAULT_DTOR(Random)

    ///
    /// Initializes everything to their default values.
    ///
    Random();


    ///
    /// Specifies a new seed for the random.
    /// Initially, the current time will be
    /// taken upon constructing a Random.
    ///
    /// \param seed Something unique.
    ///
    void setSeed(unsigned int seed);

    ///
    /// Specifies the min/max value for
    /// the Random::nextNumber() function.
    ///
    /// \default By default, this is 0 to INT_MAX.
    /// \param min Minimum value.
    /// \param max Maximum value.
    ///
    void setMinMax(int min, int max);

    ///
    /// Specifies the min/max value for
    /// the Random::nextDouble() function.
    ///
    /// \default By default, this is 0 to 1.
    /// \param min Minimum value.
    /// \param max Maximum value.
    ///
    void setMinMax(double min, double max);

    ///
    /// Specifies the charset to pick random chars from
    /// for the Random::nextString() function.
    ///
    /// \defaultBy default, these are all the printable
    /// ASCII characters.
    /// \param charset A set of characters to use.
    ///
    void setCharset(const QString& charset);

    ///
    /// Specifies the distribution for numbers 0-X to
    /// appear upon generating random numbers with the
    /// Random::nextDiscrete() function. Note: Number
    /// of elements minus 1 specify the amount of
    /// possibilities.
    ///
    /// \code
    /// // 10% to hit 0, 90% to hit 1
    /// std::discrete_distribution<double> d({ 0.1, 0.9 });
    /// // 20% to hit 0, 1 and 2, 40% to hit 3
    /// std::discrete_distribution<double> d({ 0.2, 0.2, 0.2, 0.4 });
    /// \endcode
    ///
    /// \default By default, this is empty.
    /// \param dist The distribution to use.
    ///
    void setDistribution(const std::discrete_distribution<>& dist);


    ///
    /// Retrieves the next random number.
    /// Set min/max via Random::setMinMax().
    ///
    /// \returns a random number.
    ///
    int nextNumber() const;

    ///
    /// Retrieves the next number from a
    /// discrete distrubtion specified via
    /// Random::setDistribution().
    ///
    /// \returns a random number.
    ///
    int nextDiscrete() const;

    ///
    /// Retrieves the next floating-point number.
    /// Set min/max via Random::setMinMax().
    ///
    /// \returns a random floating-point number.
    ///
    double nextDouble() const;

    ///
    /// Retrieves the next random string.
    /// Set charset via Random::setCharset().
    ///
    /// \param size Size of the random string.
    /// \returns a random string.
    ///
    QString nextString(int size) const;


private:

    // Members
    mutable std::mt19937 m_engine;
    mutable std::discrete_distribution<> m_distrib;
    QString m_charset;
    double m_fmin, m_fmax;
    int m_imin, m_imax;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_RANDOM_HPP

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


#pragma once
#ifndef CRANBERRY_RANDOM_HPP
#define CRANBERRY_RANDOM_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QString>

// Standard headers
#include <random>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class is able to generate random numbers, booleans and strings. It also
/// supports discrete distribution if one needs to work with percentage.
///
/// \class Random
/// \author Nicolas Kogler
/// \date May 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT Random
{
public:

    typedef std::discrete_distribution<> Distribution;
    typedef std::mt19937 MersenneTwister;


    CRANBERRY_DEFAULT_DTOR(Random)
    CRANBERRY_DEFAULT_COPY(Random)
    CRANBERRY_DEFAULT_MOVE(Random)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes the engine with an initial seed (current time).
    ///
    ////////////////////////////////////////////////////////////////////////////
    Random();


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies a new seed for the random. By default, this is the time at
    /// which the Random object is constructed.
    ///
    /// \param seed Something unique.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSeed(uint seed);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the min/max value for the Random::nextNumber() function.
    ///
    /// \default By default, this is 0 to INT_MAX.
    /// \param min Minimum value.
    /// \param max Maximum value.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMinMax(int min, int max);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the min/max value for the Random::nextDouble() function.
    ///
    /// \default By default, this is 0 to 1.
    /// \param min Minimum value.
    /// \param max Maximum value.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMinMax(double min, double max);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the distribution for numbers 0-X to appear upon generating
    /// numbers with the Random::nextDiscrete() function. Note: Number of
    /// elements specify the amount of possibilities. To simplify calculating
    /// the percentages for yourself, it is recommended to add elements between
    /// 0 and 1 and they should add up to 1 in total.
    ///
    /// \code
    /// // 10% to hit 0, 90% to hit 1.
    /// Random::Distribution dist { 0.1, 0.9 };
    /// random.setDistribution(dist);
    /// \endcode
    /// \default By default, this is empty.
    /// \param dist Contains the distributions.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setDistribution(const Distribution& dist);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the charset to pick random characters from for the
    /// Random::nextString() function.
    ///
    /// \default By default, these are all printable ASCII characters.
    /// \param charset A set of characters to use (1 char = 1 entry).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setCharset(const QString& charset);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random boolean.
    ///
    /// \returns a random boolean.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool nextBoolean() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random number. Set min/max using Random::setMinMax().
    ///
    /// \returns a random integer number.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int nextNumber() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random number from a discrete distribution. Set
    /// the distribution by using Random::setDistribution().
    ///
    /// \returns a random discrete number.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int nextDiscrete() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random floating-point number. Set min/max by using
    /// Random::setMinMax().
    ///
    /// \returns a random floating-point number.
    ///
    ////////////////////////////////////////////////////////////////////////////
    double nextDouble() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random string. Set character set to pick from by
    /// using Random::setCharset().
    ///
    /// \param size Size of the random string.
    /// \returns a random string.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString nextString(int size) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the next random byte data.
    ///
    /// \returns a random binary blob.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QByteArray nextBlob(int size) const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    mutable MersenneTwister m_engine;       ///< Engine that generates numbers
    mutable Distribution    m_dist;         ///< Discrete distribution vector
    mutable QString         m_charset;      ///< Random string character set
    double                  m_fmin, m_fmax; ///< Min/max values for doubles
    int                     m_imin, m_imax; ///< Min/max values for integers
};


////////////////////////////////////////////////////////////////////////////////
/// \ingroup System
///
/// With this class, you can easily generate reliable random numbers,
/// booleans and strings. Usage is illustrated in the example below:
///
/// \code
/// Random random;
/// random.setMinMax(10, 40);
/// random.setDistribution({ 0.1, 0.4, 0.5 });
/// random.setCharset("!§$%&/()=?");
///
/// auto r1 = random.nextBoolean();
/// auto r2 = random.nextDouble();
/// auto r3 = random.nextNumber();
/// auto r4 = random.nextDiscrete();
/// auto r5 = random.nextString(10);
/// auto r6 = random.nextBlob(10);
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

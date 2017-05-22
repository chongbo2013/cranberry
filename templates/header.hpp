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
#ifndef CRANBERRY_${CLASS}_HPP
#define CRANBERRY_${CLASS}_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers

// Standard headers


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Description of ${CLASS}.
///
/// \class ${Class}
/// \author Nicolas Kogler
/// \date May 21, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_${MODULE}_EXPORT ${Class}
{
public:

    CRANBERRY_DEFAULT_CTOR(${Class})
    CRANBERRY_DEFAULT_DTOR(${Class})
    CRANBERRY_DEFAULT_COPY(${Class})
    CRANBERRY_DEFAULT_MOVE(${Class})


    ////////////////////////////////////////////////////////////////////////////
    /// Description of function.
    ///
    /// \throws exception
    /// \param x Description of parameter.
    /// \returns a misc value.
    ///
    ////////////////////////////////////////////////////////////////////////////


protected:



private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
};


////////////////////////////////////////////////////////////////////////////////
/// \ingroup ${Module}
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

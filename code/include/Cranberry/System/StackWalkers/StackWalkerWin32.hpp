////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#ifndef CRANBERRY_SYSTEM_STACKWALKERS_STACKWALKERWIN32_HPP
#define CRANBERRY_SYSTEM_STACKWALKERS_STACKWALKERWIN32_HPP


// Cranberry headers
#include <Cranberry/System/Debug.hpp>


CRANBERRY_BEGIN_PRIV_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Walks the stack on the windows platform.
///
/// \class StackWalkerWin32
/// \author Nicolas Kogler
/// \date July 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT StackWalkerWin32 : public StackWalker
{
public:

    CRANBERRY_DEFAULT_CTOR(StackWalkerWin32)
    CRANBERRY_DEFAULT_DTOR(StackWalkerWin32)
    CRANBERRY_DEFAULT_COPY(StackWalkerWin32)
    CRANBERRY_DEFAULT_MOVE(StackWalkerWin32)


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the stack trace at this point of execution.
    ///
    /// \returns a string representing the stack trace.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QString stackTrace() override;
};


////////////////////////////////////////////////////////////////////////////////
/// \class StackWalkerWin32
/// \ingroup System
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_PRIV_NAMESPACE


#endif

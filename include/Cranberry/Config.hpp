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


#ifndef CRANBERRY_CONFIG_HPP
#define CRANBERRY_CONFIG_HPP


// Qt headers
#include <QtCore>

// Standard headers
#include <cstdint>
#include <cstring>


//!
//! This macro exports or imports dynamic symbols. In order
//! to export symbols, CRANBERRY_BUILD must be defined. In
//! order to import symbols, it must not be defined.
//!
//! @def CRANBERRY_EXPORT
//!
#ifdef CRANBERRY_BUILD
    #define CRANBERRY_EXPORT Q_DECL_EXPORT
#else
    #define CRANBERRY_EXPORT
#endif

//!
//! These macroes simplify use of namespaces within cranberry
//! and ensure easy maintenance of namespaces in the future. If
//! there are no type name conflicts between cranberry and other
//! libraries, CRANBERRY_USING_NAMESPACE should be used to enhance
//! readability of your code.
//!
//! @def CRANBERRY_NAMESPACE CRANBERRY_USING_NAMESPACE
//! @def CRANBERRY_BEGIN_NAMESPACE CRANBERRY_END_NAMESPACE
//!
#define CRANBERRY_NAMESPACE         cran
#define CRANBERRY_BEGIN_NAMESPACE   namespace CRANBERRY_NAMESPACE {
#define CRANBERRY_END_NAMESPACE     }
#define CRANBERRY_USING_NAMESPACE   using namespace CRANBERRY_NAMESPACE;

//!
//! Uses compiler-specific macroes to retrieve the function name.
//!
//! @def CRANBERRY_FUNC
//!
#ifdef Q_CC_MSVC
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__FUNCTION__)
#elif Q_CC_GNU
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__PRETTY_FUNCTION__)
#else
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__func__) // C99
#endif

//!
//! Uses compiler-specific macroes to retrieve the class name.
//!
//! @def CRANBERRY_CLASS
//!
#define CRANBERRY_CLASS CRANBERRY_CONVERT_CLASS(CRANBERRY_FUNC)

//!
//! This macro standardizes the evaluation of the __FUNCTION__,
//! __PRETTY_FUNCTION__ and __func__ macro to 'scope::func'. Uses
//! C++11 lambdas and can therefore be used as a one-liner.
//!
//! @def CRANBERRY_CONVERT_FUNC
//!
#ifdef Q_CC_GNU
    #define CRANBERRY_CONVERT_FUNC(func)            \
    {                                               \
        [] (const char* s) -> std::string           \
        {                                           \
            std::string str(s);                     \
            std::size_t f = str.find_first_of(" "); \
            str.erase(0, f + 1);                    \
            f = str.find_first_of("(");             \
            str.erase(f, -1);                       \
            return str;                             \
        } (func)                                    \
    }
#else
    #define CRANBERRY_CONVERT_FUNC(func) (func)
#endif

//!
//! Standardized the retrieval of C++ class names, because
//! every compiler has different mangling conventions. Tries
//! to find the class name by looking at the pretty function.
//!
//! @def CRANBERRY_CONVERT_CLASS
//!
#ifdef Q_CC_GNU
    #define CRANBERRY_CONVERT_CLASS(type)           \
    {                                               \
        [] (std::string s) -> std::string           \
        {                                           \
            s.erase(s.find_last_of("::") - 1, -1);  \
            return s;                               \
        } (type)                                    \
    }
#else
    #define CRANBERRY_CONVERT_CLASS(type) (type)
#endif

//!
//! MSVC 2013 and lower do not support noexcept.
//!
//! @def noexcept
//!
#if defined(_MSC_VER) && _MSC_VER <= 1800
    #define noexcept
#endif


#endif  // CRANBERRY_CONFIG_HPP

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
#ifndef CRANBERRY_CONFIG_HPP
#define CRANBERRY_CONFIG_HPP


// Qt headers
#include <QtGlobal>


////////////////////////////////////////////////////////////////////////////////
/// These macroes export or import dynamic symbols. In order to export symbols,
/// CRANBERRY_BUILD must be defined. In order to import symbols, it must not be
/// defined.
///
/// \def CRANBERRY_EXPORT CRANBERRY_SYSTEM_EXPORT CRANBERRY_GRAPHICS_EXPORT
///      CRANBERRY_WINDOW_EXPORT CRANBERRY_GAME_EXPORT CRANBERRY_GUI_EXPORT
///
////////////////////////////////////////////////////////////////////////////////
#ifdef CRANBERRY_BUILD
    #define CRANBERRY_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_SYSTEM_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_OPENGL_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_GRAPHICS_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_INPUT_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_WINDOW_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_GAME_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_GUI_EXPORT Q_DECL_EXPORT
#else
    #define CRANBERRY_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_SYSTEM_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_OPENGL_EXPORT Q_DECL_EXPORT
    #define CRANBERRY_GRAPHICS_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_INPUT_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_WINDOW_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_GAME_EXPORT Q_DECL_IMPORT
    #define CRANBERRY_GUI_EXPORT Q_DECL_IMPORT
#endif

////////////////////////////////////////////////////////////////////////////////
/// These macroes simplify the use of namespaces within cranberry and ensure
/// easy maintenance of namespaces in the future. The sub-namespace priv
/// contains classes which should be used by cranberry itself only.
///
/// \def CRANBERRY_NAMESPACE CRANBERRY_BEGIN_NAMESPACE CRANBERRY_END_NAMESPACE
///      CRANBERRY_BEGIN_PRIV_NAMESPACE CRANBERRY_END_PRIV_NAMESPACE
///      CRANBERRY_USING_NAMESPACE
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_NAMESPACE             cran
#define CRANBERRY_BEGIN_NAMESPACE       namespace CRANBERRY_NAMESPACE {
#define CRANBERRY_BEGIN_PRIV_NAMESPACE  namespace CRANBERRY_NAMESPACE { namespace priv {
#define CRANBERRY_USING_NAMESPACE       using namespace CRANBERRY_NAMESPACE;
#define CRANBERRY_END_PRIV_NAMESPACE    }}
#define CRANBERRY_END_NAMESPACE         }

////////////////////////////////////////////////////////////////////////////////
/// These macroes simplify the use of forward declarations for Qt and cranberry.
/// Note: As for now, all Qt classes reside in the global namespace.
///
/// \def CRANBERRY_FORWARD_C CRANBERRY_FORWARD_Q
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_FORWARD_C(x)  CRANBERRY_BEGIN_NAMESPACE class x; CRANBERRY_END_NAMESPACE
#define CRANBERRY_FORWARD_Q(x)  QT_BEGIN_NAMESPACE class x; QT_END_NAMESPACE

#define TOLIST(...) { __VA_ARGS__ }

////////////////////////////////////////////////////////////////////////////////
/// A macro for defining global and const variables. Ensures that there are no
/// multiple definitions.
///
/// \def CRANBERRY_GLOBAL_VAR CRANBERRY_CONST_VAR CRANBERRY_CONST_ARR
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_GLOBAL_VAR(x, y) namespace { x y; }
#define CRANBERRY_GLOBAL_VAR_A(x, y, z) namespace { x y = z; }
#define CRANBERRY_CONST_VAR(x, y, z) namespace { const x y = z; }
#define CRANBERRY_CONST_ARR(t, n, c, d) namespace { std::array<t, n> c(TOLIST d); }

////////////////////////////////////////////////////////////////////////////////
/// These macroes are to be used inside classes in order to declare the
/// constructor, destructor, copy constructor and/or move constructor as either
/// default or deleted.
///
/// \def CRANBERRY_DEFAULT_CTOR CRANBERRY_DEFAULT_DTOR
///      CRANBERRY_DEFAULT_COPY CRANBERRY_DEFAULT_MOVE
///      CRANBERRY_DISABLE_COPY CRANBERRY_DISABLE_MOVE
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_DEFAULT_CTOR(x) x() = default;
#define CRANBERRY_DEFAULT_DTOR(x) virtual ~x() = default;
#define CRANBERRY_DEFAULT_COPY(x) x(const x& other) = default; x& operator =(const x& other) = default;
#define CRANBERRY_DEFAULT_MOVE(x) x(x&& other) = delete; x& operator =(x&& other) = default;
#define CRANBERRY_DISABLE_COPY(x) x(const x& other) = delete; x& operator =(const x& other) = delete;
#define CRANBERRY_DISABLE_MOVE(x) x(x&& other) = delete; x& operator =(x&& other) = delete;

////////////////////////////////////////////////////////////////////////////////
/// Uses compiler-specific macroes to retrieve the function name.
///
/// \def CRANBERRY_FUNC
///
////////////////////////////////////////////////////////////////////////////////
#ifdef Q_CC_MSVC
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__FUNCTION__)
#elif Q_CC_GNU
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__PRETTY_FUNCTION__)
#else
    #define CRANBERRY_FUNC CRANBERRY_CONVERT_FUNC(__func__)
#endif

////////////////////////////////////////////////////////////////////////////////
/// Uses compiler-specific macroes to retrieve the class name.
///
/// \def CRANBERRY_CLASS
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_CLASS CRANBERRY_CONVERT_CLASS(CRANBERRY_FUNC)

////////////////////////////////////////////////////////////////////////////////
/// This macro standardizes the evaluation of the __FUNCTION__,
/// __PRETTY_FUNCTION__ and __func__ macro to \e scope::func. It utilizes
/// C++11 lambdas and can therefore be used in one line.
///
/// \def CRANBERRY_CONVERT_FUNC
///
////////////////////////////////////////////////////////////////////////////////
#ifdef Q_CC_GNU
    #define CRANBERRY_CONVERT_FUNC(func)            \
    {                                               \
        [](const char* s) -> std::string            \
        {                                           \
            std::string str(s);                     \
            std::size_t f = str.find_first_of(" "); \
            str.erase(0, f + 1);                    \
            f = str.find_first_of("(");             \
            str.erase(f, -1);                       \
            f = str.find_first_of(" ");             \
            str.erase(0, f + 1);                    \
            str.append("()");                       \
            return str;                             \
        } (func).c_str()                            \
    }
#else
    #define CRANBERRY_CONVERT_FUNC(func) (func)
#endif

////////////////////////////////////////////////////////////////////////////////
/// This macro standardizes the retrieval of C++ class names, because every
/// compiler has different mangling conventions. Tries to find the class name
/// by looking at the pretty function and extracting the highest scope.
///
/// \def CRANBERRY_CONVERT_CLASS
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_CONVERT_CLASS(type)           \
{                                               \
    [](std::string s) -> std::string            \
    {                                           \
        s.erase(s.find_last_of("::") - 1, 1);   \
        return s;                               \
    } (type).c_str()                            \
}

////////////////////////////////////////////////////////////////////////////////
/// Defines all exit codes for Cranberry.
///
/// \ingroup Exitcode
/// \def CRANBERRY_NORMAL CRANBERRY_FATAL CRANBERRY_UNHANDLED
///
////////////////////////////////////////////////////////////////////////////////
#define CRANBERRY_EXIT_NORMAL       0
#define CRANBERRY_EXIT_FATAL        1
#define CRANBERRY_EXIT_UNHANDLED    2

////////////////////////////////////////////////////////////////////////////////
/// MSVC 2013 and lower do not support noexcept.
///
/// \def noexcept
///
////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && _MSC_VER <= 1800
    #define noexcept
#endif

#define ERRARG(x) x.arg(CRANBERRY_FUNC, name())
#define ERRARG_1(x, y) x.arg(CRANBERRY_FUNC, name(), y)
#define ERRARG_2(x, y, z) x.arg(CRANBERRY_FUNC, name(), y, z)
#define ERRARG_3(x, y, z, w) x.arg(CRANBERRY_FUNC, name(), y, z, w)


////////////////////////////////////////////////////////////////////////////////
/// \defgroup System System
/// All cranberry classes of module System.
/// \defgroup OpenGL OpenGL
/// All cranberry classes of module OpenGL.
/// \defgroup Graphics Graphics
/// All cranberry classes of module Graphics.
/// \defgroup Input Input
/// All cranberry classes of module Input.
/// \defgroup Window Window
/// All cranberry classes of module Window.
/// \defgroup Game Game
/// All cranberry classes of module Game.
/// \defgroup Gui Gui
/// All cranberry classes of module Gui.
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// \ingroup System
///
/// The config header provides many macroes for simplifying code and increasing
/// readability. It is referenced by all the other Cranberry headers.
///
/// Example utilizing macroes for classes:
/// \code
/// CRANBERRY_BEGIN_NAMESPACE
/// class CRANBERRY_EXPORT MyClass
/// {
/// public:
///
///     CRANBERRY_DEFAULT_CTOR(MyClass)
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


#endif

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
#ifndef CRANBERRY_INPUT_ENUMERATIONS_HPP
#define CRANBERRY_INPUT_ENUMERATIONS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds all gamepad buttons supported.
///
/// \enum GamepadButton
///
////////////////////////////////////////////////////////////////////////////////
enum GamepadButton
{
    GamepadButtonNone = -1,
    GamepadButtonA,
    GamepadButtonB,
    GamepadButtonX,
    GamepadButtonY,
    GamepadButtonL1,
    GamepadButtonR1,
    GamepadButtonL2,
    GamepadButtonR2,
    GamepadButtonL3,
    GamepadButtonR3,
    GamepadButtonStart,
    GamepadButtonSelect,
    GamepadButtonGuide,
    GamepadButtonMax
};

////////////////////////////////////////////////////////////////////////////////
/// Holds all gamepad axes supported.
///
/// \enum GamepadAxis
///
////////////////////////////////////////////////////////////////////////////////
enum GamepadAxis
{
    GamepadAxisNone = -1,
    GamepadAxisLeftX,
    GamepadAxisLeftY,
    GamepadAxisRightX,
    GamepadAxisRightY,
    GamepadAxisMax
};


CRANBERRY_END_NAMESPACE


#endif

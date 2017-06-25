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
#ifndef CRANBERRY_ENUMERATIONS_HPP
#define CRANBERRY_ENUMERATIONS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the blending modes within cranberry. Multiple blending
/// modes can also be combined by using the logical OR operator.
///
/// \enum BlendMode
///
////////////////////////////////////////////////////////////////////////////////
enum BlendMode
{
    BlendNone       = 0x0000, ///< Ignores the blend color
    BlendMultiply   = 0x0001, ///< Multiplies pixel with blend color
    BlendScreen     = 0x0002, ///< 1 - (1 - pixel) * (1 - blend)
    BlendOverlay    = 0x0004, ///< Combines multiply and screen
    BlendDivide     = 0x0008, ///< Divides pixel with blend color
    BlendAdd        = 0x0010, ///< Adds pixel and blend color
    BlendSubtract   = 0x0020, ///< Subtracts pixel and blend color
    BlendDifference = 0x0040, ///< abs(pixel - blend)
    BlendDarken     = 0x0080, ///< Picks the smallest RGB triple
    BlendLighten    = 0x0100  ///< Picks the greatest RGB triple
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the image effects within cranberry.
///
/// \enum Effect
///
////////////////////////////////////////////////////////////////////////////////
enum Effect
{
    EffectNone,
    EffectGrayscale,
    EffectSepia,
    EffectInvert,
    EffectSilhouette
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the movement directions within cranberry. Multiple move
/// directions can also be combined by using the logical OR operator.
///
/// \enum MoveDirection
///
////////////////////////////////////////////////////////////////////////////////
enum MoveDirection
{
    MoveNone    = 0x0000, ///< No direction
    MoveNorth   = 0x0001, ///< Move towards top
    MoveEast    = 0x0002, ///< Move towards right
    MoveSouth   = 0x0004, ///< Move towards bottom
    MoveWest    = 0x0008  ///< Move towards left
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the scale directions within cranberry.
///
/// \enum ScaleDirection
///
////////////////////////////////////////////////////////////////////////////////
enum ScaleDirection
{
    ScaleNone, ///< No scale
    ScaleUp,   ///< Increase scaling factor
    ScaleDown  ///< Decrease scaling factor
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the rotate directions within cranberry.
///
/// \enum RotateDirection
///
////////////////////////////////////////////////////////////////////////////////
enum RotateDirection
{
    RotateNone, ///< No rotation
    RotateCW,   ///< Clockwise rotation
    RotateCCW   ///< Counter-clockwise rotation
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the rotate axes within cranberry. Multiple rotate
/// axes can also be combined by using the logical OR operator.
///
/// \enum RotateAxis
///
////////////////////////////////////////////////////////////////////////////////
enum RotateAxis
{
    AxisNone    = 0x0000, ///< No rotation
    AxisX       = 0x0001, ///< Rotation around X
    AxisY       = 0x0002, ///< Rotation around Y
    AxisZ       = 0x0004  ///< Rotation around Z
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the fade directions within cranberry.
///
/// \enum FadeDirection
///
////////////////////////////////////////////////////////////////////////////////
enum FadeDirection
{
    FadeNone, ///< No fade
    FadeIn,   ///< Increase opacity
    FadeOut   ///< Decrease opacity
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the scroll modes within cranberry.
///
/// \enum ScrollMode
///
////////////////////////////////////////////////////////////////////////////////
enum ScrollMode
{
    ScrollNone,     ///< No scroll
    ScrollNormal,   ///< Normal limited scroll
    ScrollInfinite, ///< Infinite scroll
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the animation modes within cranberry.
///
/// \enum AnimationMode
///
////////////////////////////////////////////////////////////////////////////////
enum AnimationMode
{
    AnimateOnce,
    AnimateForever
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the render modes within cranberry.
///
/// \enum RenderMode
///
////////////////////////////////////////////////////////////////////////////////
enum RenderMode
{
    RenderNone          = -1,     ///< No rendering
    RenderPoints        = 0x0000, ///< Render simple dots
    RenderLines         = 0x0001, ///< Render one line
    RenderLineLoop      = 0x0002, ///< Render multiple connected lines
    RenderLineStrip     = 0x0003, ///< Render multiple unconnected lines
    RenderTriangles     = 0x0004, ///< Render multiple standalone triangles
    RenderTriangleStrip = 0x0005, ///< Render multiple connected triangles
    RenderTriangleFan   = 0x0006, ///< Render multiple filled triangles
    RenderPatches       = 0x000E, ///< Render dots of various sizes
};

////////////////////////////////////////////////////////////////////////////////
// Qt flags
////////////////////////////////////////////////////////////////////////////////
Q_DECLARE_FLAGS(BlendModes, BlendMode)
Q_DECLARE_FLAGS(MoveDirections, MoveDirection)
Q_DECLARE_FLAGS(ScaleDirections, ScaleDirection)
Q_DECLARE_FLAGS(RotateDirections, RotateDirection)
Q_DECLARE_FLAGS(RotateAxes, RotateAxis)
Q_DECLARE_FLAGS(FadeDirections, FadeDirection)
Q_DECLARE_FLAGS(ScrollModes, ScrollMode)


////////////////////////////////////////////////////////////////////////////////
/// \file Enumerations
/// \ingroup Graphics
///
/// The enum value names defined in each enum of this file are always prepended
/// with their purpose, e.g. 'BlendNone' for 'BlendMode'. Thid enhances
/// usability since auto-completion will suggest all blend modes just by typing
/// 'Blend'. The example below illustrates how to use them:
///
/// \code
/// auto* sprite = new Sprite("Player1", this);
/// sprite->setBlendMode(BlendMultiply | BlendScreen);
/// sprite->setMoveDirection(MoveSouth | MoveWest);
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Qt flag operators
////////////////////////////////////////////////////////////////////////////////
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::BlendModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::MoveDirections)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::ScaleDirections)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::RotateDirections)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::RotateAxes)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::FadeDirections)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::ScrollModes)


#endif

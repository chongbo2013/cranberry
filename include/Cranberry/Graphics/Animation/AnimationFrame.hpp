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
#ifndef CRANBERRY_ANIMATIONFRAME_HPP
#define CRANBERRY_ANIMATIONFRAME_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Holds one animation frame determining the
/// index of the texture and the rectangle
/// within the texture.
///
/// \class AnimationFrame
/// \author Nicolas Kogler
/// \date March 25, 2017
///
class CRANBERRY_EXPORT AnimationFrame
{
public:

    CRANBERRY_DEFAULT_DTOR(AnimationFrame)

    ///
    /// Initializes a new instance of AnimationFrame.
    ///
    AnimationFrame();

    ///
    /// Copies one AnimationFrame instance to another.
    ///
    /// \param other The other AnimationFrame instance.
    ///
    AnimationFrame(const AnimationFrame& other);

    ///
    /// Moves one AnimationFrame instance to another.
    ///
    /// \param other The other AnimationFrame instance.
    ///
    AnimationFrame(AnimationFrame&& other);

    ///
    /// Copies one AnimationFrame instance to another.
    ///
    /// \param other The other AnimationFrame instance.
    ///
    AnimationFrame& operator=(const AnimationFrame& other);


    ///
    /// Retrieves the amount of seconds before the next
    /// frame should be displayed. This can be perfectly
    /// checked against cran::GameTime::deltaTime().
    ///
    /// \returns the amount of seconds for this frame.
    ///
    qreal duration() const;

    ///
    /// Retrieves the index of this animation frame
    /// in the entire animation.
    ///
    /// \returns this frame index in the animation.
    ///
    int32_t frameIndex() const;

    ///
    /// Retrieves the index of the texture in
    /// which this frame is located in.
    ///
    /// \returns the index of the texture.
    ///
    int32_t textureIndex() const;

    ///
    /// Retrieves the location and size within
    /// the target texture.
    ///
    /// \returns the rectangle within the target texture.
    ///
    const QRectF& textureRect() const;


    ///
    /// Specifies the duration of this frame.
    ///
    /// \param seconds Seconds for this frame to last.
    ///
    void setDuration(qreal seconds);

    ///
    /// Specifies the frame index within the animation.
    ///
    /// \param index Index within animation.
    ///
    void setFrameIndex(int32_t index);

    ///
    /// Specifies the index within a container
    /// consisting of OpenGL textures. The frame
    /// will then be drawn using this texture.
    ///
    /// Note: This does not equal the index
    /// of the texture in the OpenGL server.
    ///
    /// \param index Index of the texture to use.
    ///
    void setTextureIndex(int32_t index);

    ///
    /// Specifies the source rectangle within
    /// the target texture.
    ///
    /// \param rect Source rectangle.
    ///
    void setTextureRect(const QRectF& rect);


private:

    // Members
    qreal   m_duration;
    int32_t m_frameIndex;
    int32_t m_textureIndex;
    QRectF  m_textureRect;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_ANIMATIONFRAME_HPP

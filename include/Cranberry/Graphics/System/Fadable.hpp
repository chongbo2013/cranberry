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
#ifndef CRANBERRY_FADABLE_HPP
#define CRANBERRY_FADABLE_HPP


// Cranberry headers
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/Graphics/System/GraphicsEnums.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Provides functions for fading an object.
///
/// \class Fadable
/// \author Nicolas Kogler
/// \date February 24, 2017
///
class CRANBERRY_EXPORT Fadable
{
public:

    CRANBERRY_DEFAULT_DTOR(Fadable)
    CRANBERRY_DEFAULT_COPY(Fadable)
    CRANBERRY_DISABLE_MOVE(Fadable)

    ///
    /// Initializes a new instance of Fadable.
    ///
    Fadable();


    ///
    /// Determines whether this object is fading.
    ///
    /// \returns true if fading.
    ///
    bool isFading() const;

    ///
    /// Retrieves the fade speed of the object.
    ///
    /// \returns the fade speed.
    ///
    float fadeSpeed() const;

    ///
    /// Retrieves the current opacity of the object.
    ///
    /// \returns the object's opacity.
    ///
    float opacity() const;


    ///
    /// Specifies the fade speed of the object.
    ///
    /// \param speed Fade speed.
    ///
    void setFadeSpeed(float speed);

    ///
    /// Specifies the opacity of the object.
    ///
    /// \param opacity Opacity between 0 and 1.
    ///
    void setOpacity(float opacity);

    ///
    /// Specifies the fade direction.
    ///
    /// \param dir The fade direction.
    ///
    void setFadeDirection(FadeDirection dir);


    ///
    /// Starts the fade. Stops the fade as soon as
    /// the given \p targetOpacity is reached.
    ///
    /// \param targetOpacity Value between 0 and 1.
    ///
    void startFade(float targetOpacity);


protected:

    ///
    /// Updates the opacity value of the object.
    ///
    /// \param time Holds the delta time.
    ///
    void updateFade(const GameTime& time);


private:

    // Members
    bool    m_isFading;
    int     m_dirFade;
    float   m_speedFade;
    float   m_targetOpacity;
    float   m_opacity;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_FADABLE_HPP

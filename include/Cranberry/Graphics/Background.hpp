﻿//
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
#ifndef CRANBERRY_BACKGROUND_HPP
#define CRANBERRY_BACKGROUND_HPP


// Cranberry headers
#include <Cranberry/Graphics/Image.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a scrollable and mirrorable background.
///
/// \class Background
/// \author Nicolas Kogler
/// \date March 7, 2017
///
class CRANBERRY_EXPORT Background : public Image
{
public:

    CRANBERRY_DISABLE_MOVE(Background)
    CRANBERRY_DEFAULT_COPY(Background)
    CRANBERRY_DEFAULT_DTOR(Background)


    ///
    /// Initializes a new instance of Background.
    ///
    Background();


    ///
    /// Determines whether this background is scrolling.
    ///
    /// \returns true if scrolling, false otherwise.
    ///
    bool isScrolling() const;


    ///
    /// Specifies the initial scrolling position.
    ///
    /// \param pos Scrolling position in pixels.
    ///
    void setScrollingPosition(const QPointF& pos);

    ///
    /// Specifies the scrolling speed for both directions.
    ///
    /// \param speedX Horizontal scrolling speed.
    /// \param speedY Vertical scrolling speed.
    ///
    void setScrollingSpeed(float speedX, float speedY);

    ///
    /// Specifies the scrolling behaviour.
    ///
    /// \param sb Scrolling behaviour to use.
    ///
    void setScrollingBehaviour(ScrollingBehaviour sb);

    ///
    /// Specifies the scrolling direction.
    /// Multiple MoveDirection enum values may
    /// be combined via logical OR.
    ///
    /// \param dir Directions to scroll bg in.
    ///
    void setScrollingDirection(MoveDirections dir);


    ///
    /// Starts scrolling the background. The two parameters
    /// are ignored if the scrolling behaviour is \em infinite.
    ///
    /// \param byX The amount of horizontal pixels to scroll.
    /// \param byY The amount of vertical pixels to scroll.
    ///
    void startScrolling(float byX = 0, float byY = 0);

    ///
    /// Stops scrolling the background.
    ///
    void stopScrolling();


    ///
    /// Hooks the update() method from cran::Image
    /// in order to update the custom scroll.
    ///
    /// \param time Contains the delta time.
    ///
    void update(const GameTime& time) override;


private slots:

    ///
    /// Prepares the texture to wrap around the
    /// entire window. Is connected to signal
    /// Drawable::created().
    ///
    void prepareTexture();

    ///
    /// Updates the UVs, depending on the current position.
    ///
    void updateUVs();


private:

    // Members
    bool    m_isScrolling;
    float   m_speedScrollX;
    float   m_speedScrollY;
    float   m_overflow;
    float   m_scrollX;
    float   m_scrollY;
    float   m_scrollTargetX;
    float   m_scrollTargetY;
    int     m_scrollingDir;
    int     m_scrollingMode;

    // Metadata
    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_BACKGROUND_HPP
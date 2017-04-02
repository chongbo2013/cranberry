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
#ifndef CRANBERRY_ANIMATION_HPP
#define CRANBERRY_ANIMATION_HPP


// Cranberry headers
#include <Cranberry/Graphics/Image.hpp>
#include <Cranberry/Graphics/Animation/AnimationFrame.hpp>
#include <Cranberry/Graphics/System/Fadable.hpp>
#include <Cranberry/Graphics/System/Transformable.hpp>

// Standard headers
#include <vector>


CRANBERRY_BEGIN_NAMESPACE


class Window;


///
/// Defines an animation that plays several frames.
///
/// \class Animation
/// \author Nicolas Kogler
/// \date March 25, 2017
///
class CRANBERRY_EXPORT Animation
        : public Fadable
        , public Transformable
{
public:

    CRANBERRY_DISABLE_MOVE(Animation)

    ///
    /// Initializes a new instance of Animation.
    ///
    Animation();

    ///
    /// Copies one Animation instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Animation instance.
    ///
    Animation(const Animation& other);

    ///
    /// Copies one Animation instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Animation instance.
    ///
    Animation& operator=(const Animation& other);

    ///
    /// Destroys this Animation instance.
    ///
    ~Animation();


    ///
    /// Determines whether this object is valid.
    ///
    /// \returns true if valid.
    ///
    bool isValid() const;

    ///
    /// Determines whether the animation is running.
    ///
    /// \returns true if the animation is running.
    ///
    bool isAnimating() const;

    ///
    /// Creates a new animation from a given path. The file
    /// located at \p path can either be an animated GIF file
    /// or a Cranberry Animation File (CBA).
    ///
    /// \param path GIF or CBA file path.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    bool create(const QString& path, Window* renderTarget);

    ///
    /// Creates a new animation from an existing set of frames
    /// and an existing set of images for that animation. Can
    /// be used to construct even more complex animations that
    /// can not be achieved with Cranberry Animation files.
    /// The instances of cran::Image in \p images must be
    /// created on the heap. Furthermore, this class takes
    /// ownership of these, own disposal is not necessary.
    ///
    /// \param frames Frames to execute for this animation.
    /// \param images Images to use for this animation.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    bool create(const std::vector<AnimationFrame>& frames,
                const std::vector<Image*>& images,
                Window* renderTarget);

    ///
    /// Destroys all OpenGL resources allocated by this class.
    /// The destructor can do that too as a last resort, but
    /// it is recommended to not rely on that since the OpenGL
    /// context could be already destroyed at that point.
    ///
    void destroy();


    ///
    /// Starts the animation.
    ///
    void startAnimation();


    ///
    /// Stops the animation.
    ///
    void stopAnimation();


    ///
    /// Updates the transformations of the object
    /// and the current frame, in case it changed.
    ///
    /// \param gameTime Contains the delta time.
    ///
    void update(const GameTime& gameTime);

    ///
    /// Renders the current step of the animation.
    ///
    void render();


private:

    // Helpers
    void drawInto(QImage* dst, QImage& src, QRectF& rc);
    void removeAllTextures();

    // Members
    std::vector<AnimationFrame> m_frames;
    std::vector<Image*>         m_textures;
    AnimationFrame*             m_currentFrame;
    Window*                     m_renderTarget;
    double                      m_elapsedTime;
    bool                        m_isAnimating;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_ANIMATION_HPP

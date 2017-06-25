﻿////////////////////////////////////////////////////////////////////////////////
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
#ifndef CRANBERRY_IANIMATION_HPP
#define CRANBERRY_IANIMATION_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>
#include <Cranberry/Graphics/Base/TextureAtlas.hpp>
#include <Cranberry/System/Emitters/AnimationEmitter.hpp>

// Qt headers
#include <QVector>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines an abstract class that is able to play animations of an arbitrary
/// format.
///
/// \class IAnimation
/// \author Nicolas Kogler
/// \date June 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT IAnimation : public ITransformable
{
public:

    struct Frame
    {
        QRectF rect;
        qreal duration;
        qint32 frame;
        qint32 atlas;
    };


    CRANBERRY_DISABLE_COPY(IAnimation)
    CRANBERRY_DISABLE_MOVE(IAnimation)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of the IAnimation class and resets the
    /// members to their logical default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    IAnimation();

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all texture atlases used by this class.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~IAnimation();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this animation is null.
    ///
    /// \returns true if this animation is null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the animation is running.
    ///
    /// \returns true if the animation is running.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isAnimating() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new animation by loading the file at the given path and
    /// extracting the animation data. Devired classes must implement this.
    ///
    /// \param path Path to, for example, a *.gif file.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(const QString& path, Window* renderTarget) = 0;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all texture atlases used by this class.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy();

    ////////////////////////////////////////////////////////////////////////////
    /// Runs the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startAnimation(AnimationMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Stops the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopAnimation();

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the tranformations and the animation.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time);

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the current frame with all transformations.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this object.
    ///
    /// \returns the name of this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the name of this object.
    ///
    /// \param name Name of the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the emitter object for this instance.
    ///
    /// \returns a pointer to the emitter.
    ///
    ////////////////////////////////////////////////////////////////////////////
    AnimationEmitter* animationEmitter();


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// Saves the given atlas and stores it in the list of atlases.
    ///
    /// \param atlas Valid texture atlas.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void saveAtlas(TextureAtlas* atlas);

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the texture atlases internally.
    ///
    /// \param frames Frames to insert.
    /// \param durations Duration of each frame (in milliseconds).
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool createInternal(
            const QVector<QImage>& frames,
            const QVector<qreal>& durations,
            Window* renderTarget
            );

private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    AnimationMode          m_mode;
    AnimationEmitter       m_emitter;
    QVector<Frame>         m_frames;
    QVector<TextureAtlas*> m_atlases;
    QString                m_name;
    Frame*                 m_currentFrame;
    Window*                m_renderTarget;
    qreal                  m_elapsedTime;
    bool                   m_isAnimating;
};


////////////////////////////////////////////////////////////////////////////////
/// \class IAnimation
/// \ingroup Graphics
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

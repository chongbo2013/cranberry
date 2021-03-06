﻿////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#ifndef CRANBERRY_GRAPHICS_BASE_ANIMATIONBASE_HPP
#define CRANBERRY_GRAPHICS_BASE_ANIMATIONBASE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/AnimationFrame.hpp>
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/Graphics/Base/TextureAtlas.hpp>
#include <Cranberry/System/Emitters/AnimationBaseEmitter.hpp>

// Qt headers
#include <QVector>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines an abstract class that is able to play animations of an arbitrary
/// format.
///
/// \class AnimationBase
/// \author Nicolas Kogler
/// \date June 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT AnimationBase : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(AnimationBase)
    CRANBERRY_DECLARE_DTOR(AnimationBase)
    CRANBERRY_DISABLE_COPY(AnimationBase)
    CRANBERRY_DISABLE_MOVE(AnimationBase)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this animation is null.
    ///
    /// \returns true if this animation is null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the animation is running.
    ///
    /// \returns true if the animation is running.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isAnimating() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of frames.
    ///
    /// \returns the amount of frames.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int frameCount() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Runs the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void beginAnimation(AnimationMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Switches into idle mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void beginIdle();

    ////////////////////////////////////////////////////////////////////////////
    /// Resumes the animation, while leaving the mode, current frame or elapsed
    /// time unchanged.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void resumeAnimation();

    ////////////////////////////////////////////////////////////////////////////
    /// Stops the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void endAnimation();

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the idle frame (animation doing nothing).
    ///
    /// \param atlas Atlas in which the idle frame resides.
    /// \param frame Source rectangle of the idle frame.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setIdleFrame(uint atlas, const QRectF& frame);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend color that will be applied on this object. Depends
    /// on the blend mode used.
    ///
    /// \param color Color to use for blending.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend color that will be applied on this object. Depends
    /// on the blend mode used.
    ///
    /// \param tl Top left vertex.
    /// \param tr Top right vertex.
    /// \param br Bottom right vertex.
    /// \param bl Bottom left vertex.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendColor(
            const QColor& tl,
            const QColor& tr,
            const QColor& br,
            const QColor& bl
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blend mode to render this object with.
    ///
    /// \param modes One or multiple blending modes.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlendMode(BlendModes modes);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the signals for this object.
    ///
    /// \returns the signals.
    ///
    ////////////////////////////////////////////////////////////////////////////
    AnimationBaseEmitter* signals() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all texture atlases used by this class.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the tranformations and the animation.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the current frame with all transformations.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the root model item of this instance.
    ///
    /// \returns the root model item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual TreeModelItem* rootModelItem() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void updateProperties() override;


public overridable:

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


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the texture atlases internally.
    ///
    /// \param frames Frames to insert.
    /// \param durations Duration of each frame (in milliseconds).
    /// \param renderTarget The target to show animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool createInternal(
            const QVector<QImage>& frames,
            const QVector<qreal>& durations,
            Window* renderTarget
            );

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the texture atlases but skips packing the frames, since they are
    /// already specified through the \p frames parameter.
    ///
    /// \param sheets The spritesheets to use.
    /// \param frames The frames to use.
    /// \param renderTarget The target to show animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool createInternal(
            const QVector<QImage>& sheets,
            const QVector<AnimationFrame>& frames,
            Window* renderTarget
            );

private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    TextureBase* getCurrentTexture();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    AnimationBaseEmitter    m_emitter;
    TreeModelItem*          m_rootModelItem;
    AnimationMode           m_mode;
    QVector<AnimationFrame> m_frames;
    QVector<TextureAtlas*>  m_atlases;
    AnimationFrame          m_idleFrame;
    AnimationFrame*         m_currentFrame;
    qreal                   m_elapsedTime;
    bool                    m_isAnimating;
    bool                    m_isEmbedded;
};


////////////////////////////////////////////////////////////////////////////////
/// \class AnimationBase
/// \ingroup Graphics
///
/// This class is the base for all animations.
///
/// \code
/// class GifAnimation : public AnimationBase
/// {
/// public:
///
///     bool create(const QString& path, Window* renderTarget) override;
///
///     ...
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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
#ifndef CRANBERRY_SPRITE_HPP
#define CRANBERRY_SPRITE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>
#include <Cranberry/System/Receivers/SpriteReceiver.hpp>

// Qt headers
#include <QHash>


// Forward declarations
CRANBERRY_FORWARD_C(RawAnimation)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a movable and animatable sprite.
///
/// \class Sprite
/// \author Nicolas Kogler
/// \date July 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Sprite
    : public IRenderable
    , public ITransformable
{
public:

    struct Movement
    {
        QString name;
        qreal advanceX;
        qreal advanceY;
        qreal totalTime;
        QRectF idle;
        MovementMode mode;
        RawAnimation* anim;

        ~Movement();
    };


    CRANBERRY_DISABLE_COPY(Sprite)
    CRANBERRY_DISABLE_MOVE(Sprite)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of Sprite and sets all members to their
    /// logical default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Sprite();

    ////////////////////////////////////////////////////////////////////////////
    /// Last resort to destroy any OpenGL resources.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~Sprite();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the key input is currently blocked.
    ///
    /// \returns true if blocked.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isBlocking() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the animation is running right now.
    ///
    /// \returns true if running.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isRunning() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the sprite by loading a JSON file containing the sprite's
    /// properties.
    ///
    /// \param path Path to a *.json file.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& path, Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all resources allocated by this sprite.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Runs the movement with the specified name.
    ///
    /// \param name Name of the movement to execute.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void runMovement(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Shows the idle frame of the given movement.
    ///
    /// \param name Name of the movement to show idle frame from.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void runIdle(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Resumes the current movement.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void resumeMovement();

    ////////////////////////////////////////////////////////////////////////////
    /// Stops the current movement.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopMovement();


    ////////////////////////////////////////////////////////////////////////////
    /// Updates the animation.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the animation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;


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


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    SpriteReceiver            m_receiver;
    QHash<QString, Movement*> m_movements;
    Movement*                 m_currentMove;
    bool                      m_isRunning;
    bool                      m_isBlocking;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Sprite
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
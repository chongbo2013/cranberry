﻿////////////////////////////////////////////////////////////////////////////////
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
#ifndef CRANBERRY_GRAPHICS_BASE_TRANSFORMBASE_HPP
#define CRANBERRY_GRAPHICS_BASE_TRANSFORMBASE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/Hitbox.hpp>
#include <Cranberry/System/GameTime.hpp>
#include <Cranberry/System/Emitters/TransformBaseEmitter.hpp>

// Qt headers
#include <QRectF>

// Forward declarations
CRANBERRY_FORWARD_Q(QMatrix4x4)
CRANBERRY_FORWARD_Q(QStandardItemModel)
CRANBERRY_FORWARD_C(RenderBase)
CRANBERRY_FORWARD_C(TreeModel)
CRANBERRY_FORWARD_C(TreeModelItem)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Provides functions for rotating, scaling, moving and fading objects.
///
/// \class TransformBase
/// \author Nicolas Kogler
/// \date June 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT TransformBase
{
public:

    CRANBERRY_DISABLE_COPY(TransformBase)
    CRANBERRY_DISABLE_MOVE(TransformBase)

    TransformBase();
    virtual ~TransformBase();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is currently moving.
    ///
    /// \returns true if moving.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isMoving() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is currently rotating.
    ///
    /// \returns true if rotating.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isRotating() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is currently scaling.
    ///
    /// \returns true if scaling.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isScaling() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is currently fading.
    ///
    /// \returns true if fading.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isFading() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-position of the object.
    ///
    /// \returns the X-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float x() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-position of the object.
    ///
    /// \returns the Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float y() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotation angle of the Z-axis of the object.
    /// Note: This is the actual angle in 2-D. Equal to angleZ().
    ///
    /// \returns the rotation angle of the Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float angle() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotation angle of the X-axis of the object.
    ///
    /// \returns the rotation angle of the X-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float angleX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotation angle of the Y-axis of the object.
    ///
    /// \returns the rotation angle of the Y-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float angleY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotation angle of the Z-axis of the object.
    /// Note: This is the actual angle in 2-D.
    ///
    /// \returns the rotation angle of the Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float angleZ() const;


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-scale of the object.
    ///
    /// \returns the X-scale.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float scaleX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-scale of the object.
    ///
    /// \returns the Y-scale.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float scaleY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the absolute width, in pixels. To get the visible width, issue
    /// a call to TransformBase::bounds().boundingRect().width().
    ///
    /// \returns the width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float width() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the absolute height, in pixels. To get the visible height,
    /// issue a call to TransformBase::bounds().boundingRect().height().
    ///
    /// \returns the height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float height() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the opacity of the object.
    ///
    /// \returns the opacity in a range from 0 to 1.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float opacity() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the entire transformation matrix. Since TransformBase and
    /// IRenderable are two independent classes, we need it here in order to
    /// retrieve the render target's width and height. Do not delete the
    /// returned matrix object.
    ///
    /// \param obj Target to render to. Tip: Simply use 'this' for RenderBases.
    /// \returns a pointer to the transformation matrix.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QMatrix4x4* matrix(RenderBase* obj) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the move direction of the object.
    ///
    /// \returns the move direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    MoveDirections moveDirection() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-rotate direction of the object.
    ///
    /// \returns the X-rotate direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RotateDirection rotateDirectionX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-rotate direction of the object.
    ///
    /// \returns the Y-rotate direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RotateDirection rotateDirectionY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Z-rotate direction of the object.
    ///
    /// \returns the Z-rotate direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RotateDirection rotateDirectionZ() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotate mode.
    ///
    /// \returns the rotate mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RotateMode rotateMode() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the X-scale direction of the object.
    ///
    /// \returns the X-scale direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ScaleDirection scaleDirectionX() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the Y-scale direction of the object.
    ///
    /// \returns the Y-scale direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ScaleDirection scaleDirectionY() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the fade direction of the object.
    ///
    /// \returns the fade direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    FadeDirection fadeDirection() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the position of the object.
    ///
    /// \returns the position.
    ///
    ////////////////////////////////////////////////////////////////////////////
     QPointF pos() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the origin of the object.
    ///
    /// \returns the origin.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QPointF origin() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the rotate axes of the object.
    ///
    /// \returns the rotate axes.
    ///
    ////////////////////////////////////////////////////////////////////////////
    std::tuple<float, float, float> rotateAxes() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the exact hitbox of the object. Can be used for precise
    /// collision detection.
    ///
    /// \note If you rotate your object and want to use this method to determine
    ///       the target position for a e.g. TransformBase::startMovingTo() call,
    ///       use TransformBase::rect() instead.
    ///
    /// \returns the bounds.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const Hitbox& hitbox();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the bounds of the object but does not include the rotation.
    /// It is not recommended to use this method for collision detection. Use it
    /// to calculate target positions.
    ///
    /// \returns the bounds, without rotation applied.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QRectF rect() const;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the move speed of the object.
    ///
    /// \param speedX Horizontal move speed.
    /// \param speedY Vertical move speed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setMoveSpeed(float speedX, float speedY);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate speed of the object.
    ///
    /// \param speedX Speed of X-axis.
    /// \param speedY Speed of Y-axis.
    /// \param speedZ Speed of Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRotateSpeed(float speedX, float speedY, float speedZ);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate axes of the object. Only needed when rotating the
    /// object infinitely long through TransformBase::startRotating().
    ///
    /// \param axes Rotate axes to use.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRotateAxes(RotateAxes axes);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate mode of the object.
    ///
    /// \param mode RotateOnce or RotateForever.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRotateMode(RotateMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scale speed of the object.
    ///
    /// \param speedX Horizontal scale speed.
    /// \param speedY Vertical scale speed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScaleSpeed(float speedX, float speedY);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the fade speed of the object.
    ///
    /// \param speed Fade speed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setFadeSpeed(float speed);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the X-position of the object.
    ///
    /// \param x New X-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setX(float x);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the Y-position of the object.
    ///
    /// \param y New Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setY(float y);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate angle of the object.
    /// This is exclusive to 2-D objects.
    ///
    /// \param degrees Rotate angle in degrees (0-360).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setAngle(float degrees);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate angle of the object.
    /// This is exclusive to 3-D objects.
    ///
    /// \param x Rotate angle in degrees (0-360) for X-axis.
    /// \param y Rotate angle in degrees (0-360) for Y-axis.
    /// \param z Rotate angle in degrees (0-360) for Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setAngle(float x, float y, float z);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scale factor of the object.
    ///
    /// \param scaleX Scale factor (hor) (0=invisible, 1=normal, 2=double, ...).
    /// \param scaleY Scale factor (ver) (0=invisible, 1=normal, 2=double, ...).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScale(float scaleX, float scaleY);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the opacity of the object.
    ///
    /// \param opacity Opacity, ranging from 0 to 1.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOpacity(float opacity);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the object.
    ///
    /// \param x New X-position.
    /// \param y New Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setPosition(float x, float y);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the object.
    ///
    /// \param pos New position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setPosition(const QPointF& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate/scale origin of the object.
    ///
    /// \param x New X-orgin.
    /// \param y New Y-origin.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOrigin(float x, float y);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the rotate/scale origin of the object.
    ///
    /// \param origin New rotate origin.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOrigin(const QPointF& origin);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the size of the object.
    ///
    /// \param width New width.
    /// \param height New height.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSize(float width, float height);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the size of the object.
    ///
    /// \param size New size.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSize(const QSizeF& size);

    ////////////////////////////////////////////////////////////////////////////
    /// Copies all transformations from one base to another.
    ///
    /// \param src Source transformation base.
    /// \param dst Destination transformation base.
    /// \param copySize Should size also be copied?
    ///
    ////////////////////////////////////////////////////////////////////////////
    void copyTransform(TransformBase* src, TransformBase* dst, bool copySize = false);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the object by \p advanceX in the X-direction and by \p advanceY
    /// in the Y-direction.
    ///
    /// \param advanceX Amount of units to move object horizontally.
    /// \param advanceY Amount of units to move object vertically.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startMovingBy(float advanceX, float advanceY);

    ////////////////////////////////////////////////////////////////////////////
    /// Moves the object to \p targetX in the X-direction and to \p targetY
    /// in the Y-direction. Note: If you scaled your object, you are required
    /// to use TransformBase::rect() to calculate your target position!
    ///
    /// \param targetX Target X-position.
    /// \param targetY Target Y-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startMovingTo(float targetX, float targetY);

    ////////////////////////////////////////////////////////////////////////////
    /// Starts rotating the object, but only if rotate mode is RotateForever.
    ///
    /// \param cwX Clockwise rotation around X-axis?
    /// \param cwY Clockwise rotation around Y-axis?
    /// \param cwZ Clockwise rotation around Z-axis?
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startRotating(bool cwX = true, bool cwY = true, bool cwZ = true);

    ////////////////////////////////////////////////////////////////////////////
    /// Rotates the object by \p advance degrees.
    /// This is exclusive to 2-D objects.
    ///
    /// \param advance Amount of degrees to rotate object by.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startRotatingBy(float advance);

    ////////////////////////////////////////////////////////////////////////////
    /// Rotates the object by \p advance degrees.
    /// This is exclusive to 3-D objects.
    ///
    /// \param advanceX Amount of degrees to rotate object by around X-axis.
    /// \param advanceY Amount of degrees to rotate object by around Y-axis.
    /// \param advanceZ Amount of degrees to rotate object by around Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startRotatingBy(float advanceX, float advanceY, float advanceZ);

    ////////////////////////////////////////////////////////////////////////////
    /// Rotates the object to \p target degrees.
    /// This is exclusive to 2-D objects.
    ///
    /// \param target Angle (in degrees) to rotate object to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startRotatingTo(float target);

    ////////////////////////////////////////////////////////////////////////////
    /// Rotates the object by \p targetX|targetY|targetZ degrees.
    /// This is exclusive to 3-D objects.
    ///
    /// \param targetX Angle to rotate object to around X-axis.
    /// \param targetY Angle to rotate object to around Y-axis.
    /// \param targetZ Angle to rotate object to around Z-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startRotatingTo(float targetX, float targetY, float targetZ);

    ////////////////////////////////////////////////////////////////////////////
    /// Scales the object by \p scaleX in the X-direction and by \p scaleY
    /// in the Y-direction.
    ///
    /// \param scaleX Horizontal target scale.
    /// \param scaleY Vertical target scale.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startScalingTo(float targetX, float targetY);

    ////////////////////////////////////////////////////////////////////////////
    /// Fades the object in or out until it reaches the target opacity. To
    /// determine whether the object is now fading in or out, issue a call to
    /// TransformBase::fadeDirection().
    ///
    /// \param target Target opacity (0-1).
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startFadingTo(float target);

    ////////////////////////////////////////////////////////////////////////////
    /// Stops moving the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopMoving();

    ////////////////////////////////////////////////////////////////////////////
    /// Stops rotating the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopRotating();

    ////////////////////////////////////////////////////////////////////////////
    /// Stops scaling the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopScaling();

    ////////////////////////////////////////////////////////////////////////////
    /// Stops fading the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopFading();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the signals for this object.
    ///
    /// \returns the signals.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual TransformBaseEmitter* signals();

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void createProperties(TreeModel* model);

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void updateProperties();


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the transformations of the object.
    ///
    /// \param time Holds the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void updateTransform(const GameTime& time);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    auto visiblePos(float x, float y) -> QPointF;
    void updateMove(double delta);
    void updateRotate(double delta);
    void updateScale(double delta);
    void updateFade(double delta);
    void checkMove();
    void checkScale();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    TransformBaseEmitter m_emitter;
    TreeModelItem*       m_rootModelItem;
    MoveDirections       m_moveDir;
    RotateDirection      m_rotateDirX;
    RotateDirection      m_rotateDirY;
    RotateDirection      m_rotateDirZ;
    ScaleDirection       m_scaleDirX;
    ScaleDirection       m_scaleDirY;
    FadeDirection        m_fadeDir;
    RotateAxes           m_rotateAxes;
    RotateMode           m_rotateMode;
    QMatrix4x4*          m_matrix;
    Hitbox               m_hitbox;
    bool                 m_isMovingX;
    bool                 m_isMovingY;
    bool                 m_isRotatingX;
    bool                 m_isRotatingY;
    bool                 m_isRotatingZ;
    bool                 m_isScalingX;
    bool                 m_isScalingY;
    bool                 m_isFading;
    float                m_speedMoveX;
    float                m_speedMoveY;
    float                m_speedRotateX;
    float                m_speedRotateY;
    float                m_speedRotateZ;
    float                m_speedScaleX;
    float                m_speedScaleY;
    float                m_speedFade;
    float                m_targetMoveX;
    float                m_targetMoveY;
    float                m_targetRotateX;
    float                m_targetRotateY;
    float                m_targetRotateZ;
    float                m_targetScaleX;
    float                m_targetScaleY;
    float                m_targetOpacity;
    float                m_x;
    float                m_y;
    float                m_width;
    float                m_height;
    float                m_angleX;
    float                m_angleY;
    float                m_angleZ;
    float                m_scaleX;
    float                m_scaleY;
    float                m_opacity;
    float                m_originX;
    float                m_originY;
};


////////////////////////////////////////////////////////////////////////////////
/// \class TransformBase
/// \ingroup Graphics
///
/// This class is capable of applying all kinds of transformations on an object.
/// Along with that, it provides signals that are emitted as soon as one of the
/// transformations stopped in order to write complex animations with ease!
///
/// \code
/// void start()
/// {
///     connect(sprite->transformableEmitter(), SIGNAL(stoppedMoving()), this, SLOT(stop()));
///     sprite->startMovingBy(100, 80);
/// }
///
/// void stop()
/// {
///     qDebug() << "I stopped moving!";
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

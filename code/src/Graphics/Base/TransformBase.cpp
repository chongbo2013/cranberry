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


// Cranberry headers
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/Graphics/Base/TransformBase.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/System/Models/TreeModelItem.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QMatrix4x4>
#include <QTransform>

// Standard headers
#include <tuple>


CRANBERRY_USING_NAMESPACE


TransformBase::TransformBase()
    : m_moveDir(MoveNone)
    , m_rotateDirX(RotateCW)
    , m_rotateDirY(RotateCW)
    , m_rotateDirZ(RotateCW)
    , m_scaleDirX(ScaleNone)
    , m_scaleDirY(ScaleNone)
    , m_fadeDir(FadeNone)
    , m_rotateAxes(AxisZ)
    , m_rotateMode(RotateOnce)
    , m_matrix(new QMatrix4x4)
    , m_isMovingX(false)
    , m_isMovingY(false)
    , m_isRotatingX(false)
    , m_isRotatingY(false)
    , m_isRotatingZ(false)
    , m_isScalingX(false)
    , m_isScalingY(false)
    , m_isFading(false)
    , m_speedMoveX(50.f)
    , m_speedMoveY(50.f)
    , m_speedRotateX(50.f)
    , m_speedRotateY(50.f)
    , m_speedRotateZ(50.f)
    , m_speedScaleX(1.f)
    , m_speedScaleY(1.f)
    , m_speedFade(1.f)
    , m_targetMoveX(0.f)
    , m_targetMoveY(0.f)
    , m_targetRotateX(0.f)
    , m_targetRotateY(0.f)
    , m_targetRotateZ(0.f)
    , m_targetScaleX(0.f)
    , m_targetScaleY(0.f)
    , m_targetOpacity(0.f)
    , m_x(0.f)
    , m_y(0.f)
    , m_width(0.f)
    , m_height(0.f)
    , m_angleX(0.f)
    , m_angleY(0.f)
    , m_angleZ(0.f)
    , m_scaleX(1.f)
    , m_scaleY(1.f)
    , m_opacity(1.f)
    , m_originX(0.f)
    , m_originY(0.f)
{
}


TransformBase::~TransformBase()
{
    delete m_matrix;
}


bool TransformBase::isMoving() const
{
    return m_isMovingX || m_isMovingY;
}


bool TransformBase::isRotating() const
{
    return m_isRotatingX || m_isRotatingY || m_isRotatingZ;
}


bool TransformBase::isScaling() const
{
    return m_isScalingX || m_isScalingY;
}


bool TransformBase::isFading() const
{
    return m_isFading;
}


float TransformBase::x() const
{
    return m_x;
}


float TransformBase::y() const
{
    return m_y;
}


float TransformBase::angle() const
{
    return m_angleZ;
}


float TransformBase::angleX() const
{
    return m_angleX;
}


float TransformBase::angleY() const
{
    return m_angleY;
}


float TransformBase::angleZ() const
{
    return m_angleZ;
}



float TransformBase::scaleX() const
{
    return m_scaleX;
}


float TransformBase::scaleY() const
{
    return m_scaleY;
}


float TransformBase::width() const
{
    return m_width;
}


float TransformBase::height() const
{
    return m_height;
}


float TransformBase::opacity() const
{
    return m_opacity;
}


float TransformBase::moveSpeedX() const
{
    return m_speedMoveX;
}


float TransformBase::moveSpeedY() const
{
    return m_speedMoveY;
}


float TransformBase::scaleSpeedX() const
{
    return m_speedScaleX;
}


float TransformBase::scaleSpeedY() const
{
    return m_speedScaleY;
}


float TransformBase::rotateSpeedX() const
{
    return m_speedRotateX;
}


float TransformBase::rotateSpeedY() const
{
    return m_speedRotateY;
}


float TransformBase::rotateSpeedZ() const
{
    return m_speedRotateZ;
}


float TransformBase::fadeSpeed() const
{
    return m_speedFade;
}


QMatrix4x4* TransformBase::matrix(RenderBase* obj) const
{
    QMatrix4x4 proj, tran, rot, scale, orig, norig;
    qreal fw = static_cast<qreal>(obj->renderTarget()->width());
    qreal fh = static_cast<qreal>(obj->renderTarget()->height());

    proj.ortho(0.f, fw, fh, 0.f, -1, 1);
    tran.translate(m_x, m_y);
    rot.rotate(m_angleX, 1.f, 0.f, 0.f);
    rot.rotate(m_angleY, 0.f, 1.f, 0.f);
    rot.rotate(m_angleZ, 0.f, 0.f, 1.f);
    scale.scale(m_scaleX, m_scaleY);
    orig.translate(m_originX, m_originY);
    norig.translate(m_originX * -1, m_originY * -1);

    *m_matrix = proj * tran * orig * rot * norig * orig * scale * norig;

    return m_matrix;
}


MoveDirections TransformBase::moveDirection() const
{
    return m_moveDir;
}


RotateDirection TransformBase::rotateDirectionX() const
{
    return m_rotateDirX;
}


RotateDirection TransformBase::rotateDirectionY() const
{
    return m_rotateDirY;
}


RotateDirection TransformBase::rotateDirectionZ() const
{
    return m_rotateDirZ;
}


RotateMode TransformBase::rotateMode() const
{
    return m_rotateMode;
}


ScaleDirection TransformBase::scaleDirectionX() const
{
    return m_scaleDirX;
}


ScaleDirection TransformBase::scaleDirectionY() const
{
    return m_scaleDirY;
}


FadeDirection TransformBase::fadeDirection() const
{
    return m_fadeDir;
}


QPointF TransformBase::pos() const
{
    return QPointF(m_x, m_y);
}


QPointF TransformBase::origin() const
{
    return QPointF(m_originX, m_originY);
}


const Hitbox& TransformBase::hitbox()
{
    QPainterPath path;
    QTransform transform;

    path.addRect(m_x / m_scaleX, m_y / m_scaleY, m_width, m_height);
    transform.translate(m_originX, m_originY);

    if (m_angleX != 0.f) transform.rotate(m_angleX, Qt::XAxis);
    if (m_angleY != 0.f) transform.rotate(m_angleY, Qt::YAxis);
    if (m_angleZ != 0.f) transform.rotate(m_angleZ, Qt::ZAxis);

    transform.scale(m_scaleX, m_scaleY);
    transform.translate(-m_originX, -m_originY);

    m_hitbox.setHitbox(new QPainterPath(transform.map(path)));

    return m_hitbox;
}


QRectF TransformBase::visibleBounds() const
{
    QPainterPath path;
    QTransform transform;

    path.addRect(m_x / m_scaleX, m_y / m_scaleY, m_width, m_height);

    transform.translate(m_originX, m_originY);
    transform.scale(m_scaleX, m_scaleY);
    transform.translate(-m_originX, -m_originY);

    return transform.map(path).boundingRect();
}


void TransformBase::setMoveSpeed(float speedX, float speedY)
{
    m_speedMoveX = speedX;
    m_speedMoveY = speedY;
}


void TransformBase::setRotateSpeed(float speedX, float speedY, float speedZ)
{
    m_speedRotateX = speedX;
    m_speedRotateY = speedY;
    m_speedRotateZ = speedZ;
}


void TransformBase::setRotateAxes(RotateAxes axes)
{
    m_rotateAxes = axes;
}


void TransformBase::setRotateMode(RotateMode mode)
{
    m_rotateMode = mode;
}


void TransformBase::setScaleSpeed(float speedX, float speedY)
{
    m_speedScaleX = speedX;
    m_speedScaleY = speedY;
}


void TransformBase::setFadeSpeed(float speed)
{
    m_speedFade = speed;
}


void TransformBase::setX(float x)
{
    m_x = x;
    m_emitter.emitPositionChanged();
}


void TransformBase::setY(float y)
{
    m_y = y;
    m_emitter.emitPositionChanged();
}


void TransformBase::setAngle(float degrees)
{
    setAngle(0, 0, degrees);
}


void TransformBase::setAngle(float x, float y, float z)
{
    m_angleX = x;
    m_angleY = y;
    m_angleZ = z;
}


void TransformBase::setScale(float scaleX, float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;
    m_emitter.emitSizeChanged();
}


void TransformBase::setOpacity(float opacity)
{
    m_opacity = opacity;
}


void TransformBase::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    m_emitter.emitPositionChanged();
}


void TransformBase::setPosition(const QPointF& pos)
{
    setPosition(pos.x(), pos.y());
}


void TransformBase::setOrigin(float x, float y)
{
    m_originX = x;
    m_originY = y;
}


void TransformBase::setOrigin(const QPointF& origin)
{
    setOrigin(origin.x(), origin.y());
}


void TransformBase::moveBy(float advanceX, float advanceY)
{
    m_isMovingX = false;
    m_isMovingY = false;
    m_moveDir = MoveNone;

    // Calculates the target position.
    if (advanceX != 0.f)
    {
        m_isMovingX = true;
        m_targetMoveX = m_x + advanceX;

        if (advanceX < 0)
        {
            m_moveDir |= MoveWest;
        }
        else
        {
            m_moveDir |= MoveEast;
        }
    }

    if (advanceY != 0.f)
    {
        m_isMovingY = true;
        m_targetMoveY = m_y + advanceY;

        if (advanceY < 0)
        {
            m_moveDir |= MoveNorth;
        }
        else
        {
            m_moveDir |= MoveSouth;
        }
    }
}


void TransformBase::moveTo(float targetX, float targetY)
{
    m_isMovingX = false;
    m_isMovingY = false;
    m_moveDir = MoveNone;

    // Gathers amount of pixels to move in real coordinates.
    auto b = visibleBounds();
    float toMoveX = targetX - b.x();
    float toMoveY = targetY - b.y();

    // Determines the target location.
    if (targetX < b.x())
    {
        m_moveDir |= MoveWest;
        m_targetMoveX = m_x + toMoveX;
    }
    else
    {
        m_moveDir |= MoveEast;
        m_targetMoveX = m_x + toMoveX;
    }

    if (targetY < m_y)
    {
        m_moveDir |= MoveNorth;
        m_targetMoveY = m_y + toMoveY;
    }
    else
    {
        m_moveDir |= MoveSouth;
        m_targetMoveY = m_y + toMoveY;
    }

    if (int(toMoveX) != 0) m_isMovingX = true;
    if (int(toMoveY) != 0) m_isMovingY = true;
}


void TransformBase::beginRotate(bool cwX, bool cwY, bool cwZ)
{
    if (m_rotateMode != RotateForever) return;

    m_isRotatingX = (m_rotateAxes & AxisX);
    m_isRotatingY = (m_rotateAxes & AxisY);
    m_isRotatingZ = (m_rotateAxes & AxisZ);

    m_rotateDirX  = cwX ? RotateCW : RotateCCW;
    m_rotateDirY  = cwY ? RotateCW : RotateCCW;
    m_rotateDirZ  = cwZ ? RotateCW : RotateCCW;
}


void TransformBase::rotateBy(float advance)
{
    rotateBy(0, 0, advance);
}


void TransformBase::rotateBy(float advanceX, float advanceY, float advanceZ)
{
    m_isRotatingX = false;
    m_isRotatingY = false;
    m_isRotatingZ = false;
    m_rotateDirX = RotateNone;
    m_rotateDirY = RotateNone;
    m_rotateDirZ = RotateNone;

    // Calculates the target angle.
    if (advanceX != 0)
    {
        m_isRotatingX = true;

        if (advanceX < 0)
        {
            m_rotateDirX = RotateCCW;
            m_targetRotateX = m_angleX - advanceX;
        }
        else
        {
            m_rotateDirX = RotateCW;
            m_targetRotateX = m_angleX + advanceX;
        }
    }

    if (advanceY != 0)
    {
        m_isRotatingY = true;

        if (advanceY < 0)
        {
            m_rotateDirY = RotateCCW;
            m_targetRotateY = m_angleY - advanceY;
        }
        else
        {
            m_rotateDirY = RotateCW;
            m_targetRotateY = m_angleY + advanceY;
        }
    }

    if (advanceZ != 0)
    {
        m_isRotatingZ = true;

        if (advanceZ < 0)
        {
            m_rotateDirZ = RotateCCW;
            m_targetRotateZ = m_angleZ - advanceZ;
        }
        else
        {
            m_rotateDirZ = RotateCW;
            m_targetRotateZ = m_angleZ + advanceZ;
        }
    }
}


void TransformBase::rotateTo(float target)
{
    rotateTo(0, 0, target);
}


void TransformBase::rotateTo(float targetX, float targetY, float targetZ)
{
    m_targetRotateX = targetX;
    m_targetRotateY = targetY;
    m_targetRotateZ = targetZ;

    // Determines the rotate directions.
    if (targetX < m_angleX) m_rotateDirX = RotateCCW;
    else m_rotateDirX = RotateCW;
    if (targetY < m_angleY) m_rotateDirY = RotateCCW;
    else m_rotateDirY = RotateCW;
    if (targetZ < m_angleZ) m_rotateDirZ = RotateCCW;
    else m_rotateDirY = RotateCW;

    m_isRotatingX = true;
    m_isRotatingY = true;
    m_isRotatingZ = true;
}


void TransformBase::scaleTo(float scaleX, float scaleY)
{
    // Do not accept negative values.
    m_targetScaleX = qAbs(scaleX);
    m_targetScaleY = qAbs(scaleY);

    // Calculates the target scale.
    if (m_targetScaleX <= m_scaleX) m_scaleDirX = ScaleDown;
    else m_scaleDirX = ScaleUp;
    if (m_targetScaleY <= m_scaleY) m_scaleDirY = ScaleDown;
    else m_scaleDirY = ScaleUp;

    m_isScalingX = true;
    m_isScalingY = true;
}


void TransformBase::fadeTo(float target)
{
    // Do not accept negative values.
    target = (float)(uchar) qAbs(target);

    // Calculates the target opacity.
    if (target <= m_opacity) m_fadeDir = FadeOut;
    else m_fadeDir = FadeIn;

    m_targetOpacity = target;
    m_isFading = true;
}


void TransformBase::endMove()
{
    m_isMovingX = false;
    m_isMovingY = false;

    signals()->emitStoppedMoving();
}


void TransformBase::endRotate()
{
    m_isRotatingX = false;
    m_isRotatingY = false;
    m_isRotatingZ = false;

    signals()->emitStoppedRotating();
}


void TransformBase::endScale()
{
    m_isScalingX = false;
    m_isScalingY = false;

    signals()->emitStoppedScaling();
}


void TransformBase::endFade()
{
    m_isFading = false;

    signals()->emitStoppedFading();
}


void TransformBase::updateTransform(const GameTime& time)
{
    updateMove(time.deltaTime());
    updateRotate(time.deltaTime());
    updateScale(time.deltaTime());
    updateFade(time.deltaTime());
}


void TransformBase::setSize(float width, float height)
{
    m_width = width;
    m_height = height;

    signals()->emitSizeChanged();
}


void TransformBase::setSize(const QSizeF& size)
{
    setSize(size.width(), size.height());
}


void TransformBase::copyTransform(TransformBase* src, TransformBase* dst, bool s)
{
    dst->m_x       = src->m_x;
    dst->m_y       = src->m_y;
    dst->m_angleX  = src->m_angleX;
    dst->m_angleY  = src->m_angleY;
    dst->m_angleZ  = src->m_angleZ;
    dst->m_scaleX  = src->m_scaleX;
    dst->m_scaleY  = src->m_scaleY;
    dst->m_opacity = src->m_opacity;
    dst->m_originX = src->m_originX;
    dst->m_originY = src->m_originY;

    if (s)
    {
        dst->m_width   = src->m_width;
        dst->m_height  = src->m_height;
    }
}


QPointF TransformBase::visiblePos(float x, float y)
{
    QPainterPath path;
    QTransform transform;

    path.addRect(x * m_scaleX, y * m_scaleY, m_width, m_height);

    transform.translate(m_originX, m_originY);
    transform.scale(-m_scaleX, -m_scaleY);
    transform.translate(-m_originX, -m_originY);

    return transform.map(path).boundingRect().topLeft();
}


void TransformBase::updateMove(double delta)
{
    if (m_isMovingX)
    {
        if ((m_moveDir & MoveEast) != 0)
        {
            m_x += (m_speedMoveX * delta);
            if (m_x >= m_targetMoveX)
            {
                m_x = m_targetMoveX;
                m_isMovingX = false;
                checkMove();
            }
        }
        else if ((m_moveDir & MoveWest) != 0)
        {
            m_x -= (m_speedMoveX * delta);
            if (m_x <= m_targetMoveX)
            {
                m_x = m_targetMoveX;
                m_isMovingX = false;
                checkMove();
            }
        }
    }

    if (m_isMovingY)
    {
        if ((m_moveDir & MoveSouth) != 0)
        {
            m_y += (m_speedMoveY * delta);
            if (m_y >= m_targetMoveY)
            {
                m_y = m_targetMoveY;
                m_isMovingY = false;
                checkMove();
            }
        }
        else if ((m_moveDir & MoveNorth) != 0)
        {
            m_y -= (m_speedMoveY * delta);
            if (m_y <= m_targetMoveY)
            {
                m_y = m_targetMoveY;
                m_isMovingY = false;
                checkMove();
            }
        }
    }
}


void TransformBase::updateRotate(double delta)
{
    if (m_isRotatingX)
    {
        if (m_rotateDirX == RotateCW)
        {
            m_angleX += (m_speedRotateX * delta);
            if (m_rotateMode == RotateOnce && m_angleX >= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                endRotate();
            }
        }
        else
        {
            m_angleX -= (m_speedRotateX * delta);
            if (m_rotateMode == RotateOnce && m_angleX <= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                endRotate();
            }
        }

        if (m_angleX >= 360.0)
        {
            m_angleX -= 360.0;
        }
    }
    if (m_isRotatingY)
    {
        if (m_rotateDirY == RotateCW)
        {
            m_angleY += (m_speedRotateY * delta);
            if (m_rotateMode == RotateOnce && m_angleY >= m_targetRotateY)
            {
                m_angleY = m_targetRotateY;
                endRotate();
            }
        }
        else
        {
            m_angleY -= (m_speedRotateY * delta);
            if (m_rotateMode == RotateOnce && m_angleY <= m_targetRotateY)
            {
                m_angleY = m_targetRotateY;
                endRotate();
            }
        }

        if (m_angleY >= 360.0)
        {
            m_angleY -= 360.0;
        }
    }

    if (m_isRotatingZ)
    {
        if (m_rotateDirZ == RotateCW)
        {
            m_angleZ += (m_speedRotateZ * delta);
            if (m_rotateMode == RotateOnce && m_angleZ >= m_targetRotateZ)
            {
                m_angleZ = m_targetRotateZ;
                endRotate();
            }
        }
        else
        {
            m_angleZ -= (m_speedRotateZ * delta);
            if (m_rotateMode == RotateOnce && m_angleZ <= m_targetRotateZ)
            {
                m_angleZ = m_targetRotateZ;
                endRotate();
            }
        }

        if (m_angleZ >= 360.0)
        {
            m_angleZ -= 360.0;
        }
    }
}


void TransformBase::updateScale(double delta)
{
    if (m_isScalingX)
    {
        if (m_scaleDirX == ScaleUp)
        {
            m_scaleX += (m_speedScaleX * delta);
            if (m_scaleX >= m_targetScaleX)
            {
                m_scaleX = m_targetScaleX;
                m_isScalingX = false;
                checkScale();
            }
        }
        else
        {
            m_scaleX -= (m_speedScaleX * delta);
            if (m_scaleX <= m_targetScaleX)
            {
                m_scaleX = m_targetScaleX;
                m_isScalingX = false;
                checkScale();
            }
        }
    }

    if (m_isScalingY)
    {
        if (m_scaleDirY == ScaleUp)
        {
            m_scaleY += (m_speedScaleY * delta);
            if (m_scaleY >= m_targetScaleY)
            {
                m_scaleY = m_targetScaleY;
                m_isScalingY = false;
                checkScale();
            }
        }
        else
        {
            m_scaleY -= (m_speedScaleY * delta);
            if (m_scaleY <= m_targetScaleY)
            {
                m_scaleY = m_targetScaleY;
                m_isScalingY = false;
                checkScale();
            }
        }
    }
}


void TransformBase::updateFade(double delta)
{
    if (m_isFading)
    {
        if (m_fadeDir == FadeIn)
        {
            m_opacity += (m_speedFade * delta);
            if (m_opacity >= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                endFade();
            }
        }
        else
        {
            m_opacity -= (m_speedFade * delta);
            if (m_opacity <= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                endFade();
            }
        }
    }
}


void TransformBase::checkMove()
{
    if (!m_isMovingX && !m_isMovingY) endMove();
}


void TransformBase::checkScale()
{
    if (!m_isScalingX && !m_isScalingY) endScale();
}


TransformBaseEmitter* TransformBase::signals()
{
    return &m_emitter;
}


void TransformBase::createProperties(TreeModel* model)
{
    QRectF bounds = visibleBounds();

    TreeModelItem* tmiRect = new TreeModelItem("Bounds");
    TreeModelItem* tmiRota = new TreeModelItem("Rotation");
    TreeModelItem* tmiScal = new TreeModelItem("Scale");
    TreeModelItem* tmiOrig = new TreeModelItem("Origin");
    TreeModelItem* tmiStat = new TreeModelItem("States");

    m_rootModelItem = new TreeModelItem("TransformBase");
    m_rootModelItem->appendChild(tmiRect);
    m_rootModelItem->appendChild(tmiRota);
    m_rootModelItem->appendChild(tmiScal);
    m_rootModelItem->appendChild(tmiOrig);
    m_rootModelItem->appendChild(tmiStat);

    tmiRect->appendChild(new TreeModelItem("x", bounds.x()));
    tmiRect->appendChild(new TreeModelItem("y", bounds.y()));
    tmiRect->appendChild(new TreeModelItem("w", bounds.width()));
    tmiRect->appendChild(new TreeModelItem("h", bounds.height()));

    tmiRota->appendChild(new TreeModelItem("x", m_angleX));
    tmiRota->appendChild(new TreeModelItem("y", m_angleY));
    tmiRota->appendChild(new TreeModelItem("z", m_angleZ));

    tmiScal->appendChild(new TreeModelItem("x", m_scaleX));
    tmiScal->appendChild(new TreeModelItem("y", m_scaleY));

    tmiOrig->appendChild(new TreeModelItem("x", m_originX));
    tmiOrig->appendChild(new TreeModelItem("y", m_originY));

    tmiStat->appendChild(new TreeModelItem("opacity", m_opacity));
    tmiStat->appendChild(new TreeModelItem("isMoving", isMoving()));
    tmiStat->appendChild(new TreeModelItem("isRotating", isRotating()));
    tmiStat->appendChild(new TreeModelItem("isScaling", isScaling()));
    tmiStat->appendChild(new TreeModelItem("isFading", isFading()));

    model->addItem(m_rootModelItem);
}


void TransformBase::updateProperties()
{
    QRectF bounds = visibleBounds();

    TreeModelItem* tmiRect = m_rootModelItem->childAt(0);
    TreeModelItem* tmiRota = m_rootModelItem->childAt(1);
    TreeModelItem* tmiScal = m_rootModelItem->childAt(2);
    TreeModelItem* tmiOrig = m_rootModelItem->childAt(3);
    TreeModelItem* tmiStat = m_rootModelItem->childAt(4);

    tmiRect->childAt(0)->setValue(bounds.x());
    tmiRect->childAt(1)->setValue(bounds.y());
    tmiRect->childAt(2)->setValue(bounds.width());
    tmiRect->childAt(3)->setValue(bounds.height());

    tmiRota->childAt(0)->setValue(m_angleX);
    tmiRota->childAt(1)->setValue(m_angleY);
    tmiRota->childAt(2)->setValue(m_angleZ);

    tmiScal->childAt(0)->setValue(m_scaleX);
    tmiScal->childAt(1)->setValue(m_scaleY);

    tmiOrig->childAt(0)->setValue(m_originX);
    tmiOrig->childAt(1)->setValue(m_originY);

    tmiStat->childAt(0)->setValue(m_opacity);
    tmiStat->childAt(1)->setValue(isMoving());
    tmiStat->childAt(2)->setValue(isRotating());
    tmiStat->childAt(3)->setValue(isScaling());
    tmiStat->childAt(4)->setValue(isFading());
}

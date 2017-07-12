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


// Cranberry headers
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QTransform>
#include <QVector3D>


CRANBERRY_USING_NAMESPACE


ITransformable::ITransformable()
    : m_moveDir(MoveNone)
    , m_rotateDirX(RotateCW)
    , m_rotateDirY(RotateCW)
    , m_rotateDirZ(RotateCW)
    , m_scaleDirX(ScaleNone)
    , m_scaleDirY(ScaleNone)
    , m_fadeDir(FadeNone)
    , m_rotateAxes(AxisZ)
    , m_rotateMode(RotateOnce)
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


bool ITransformable::isMoving() const
{
    return m_isMovingX || m_isMovingY;
}


bool ITransformable::isRotating() const
{
    return m_isRotatingX || m_isRotatingY || m_isRotatingZ;
}


bool ITransformable::isScaling() const
{
    return m_isScalingX || m_isScalingY;
}


bool ITransformable::isFading() const
{
    return m_isFading;
}


float ITransformable::x() const
{
    return m_x;
}


float ITransformable::y() const
{
    return m_y;
}


float ITransformable::angle() const
{
    return m_angleZ;
}


float ITransformable::angleX() const
{
    return m_angleX;
}


float ITransformable::angleY() const
{
    return m_angleY;
}


float ITransformable::angleZ() const
{
    return m_angleZ;
}



float ITransformable::scaleX() const
{
    return m_scaleX;
}


float ITransformable::scaleY() const
{
    return m_scaleY;
}


float ITransformable::width() const
{
    return m_width;
}


float ITransformable::height() const
{
    return m_height;
}


float ITransformable::opacity() const
{
    return m_opacity;
}


QMatrix4x4 ITransformable::matrix(IRenderable* obj, bool flipped) const
{
    QMatrix4x4 proj, tran, rot, scale, orig, norig;
    qreal fw = static_cast<qreal>(obj->renderTarget()->width());
    qreal fh = static_cast<qreal>(obj->renderTarget()->height());

    if (flipped)
    {
        proj.ortho(0.f, fw, fh, 0.f, -1, 1);
    }
    else
    {
        proj.ortho(0.f, fw, 0.f, fh, -1, 1);
    }

    tran.translate(x(), y());
    rot.rotate(angleX(), 1.f, 0.f, 0.f);
    rot.rotate(angleY(), 0.f, 1.f, 0.f);
    rot.rotate(angleZ(), 0.f, 0.f, 1.f);
    scale.scale(scaleX(), scaleY());
    orig.translate(origin());
    norig.translate(origin() * -1);

    return proj * tran * orig * rot * norig * orig * scale * norig;
}


MoveDirections ITransformable::moveDirection() const
{
    return m_moveDir;
}


RotateDirection ITransformable::rotateDirectionX() const
{
    return m_rotateDirX;
}


RotateDirection ITransformable::rotateDirectionY() const
{
    return m_rotateDirY;
}


RotateDirection ITransformable::rotateDirectionZ() const
{
    return m_rotateDirZ;
}


RotateMode ITransformable::rotateMode() const
{
    return m_rotateMode;
}


ScaleDirection ITransformable::scaleDirectionX() const
{
    return m_scaleDirX;
}


ScaleDirection ITransformable::scaleDirectionY() const
{
    return m_scaleDirY;
}


FadeDirection ITransformable::fadeDirection() const
{
    return m_fadeDir;
}


QVector2D ITransformable::pos() const
{
    return QVector2D(m_x, m_y);
}


QVector3D ITransformable::origin() const
{
    return QVector3D(m_originX, m_originY, 0);
}


QVector3D ITransformable::rotateAxes() const
{
    QVector3D axes;

    if (m_angleX != 0.f) axes.setX(1);
    if (m_angleY != 0.f) axes.setY(1);
    if (m_angleZ != 0.f) axes.setZ(1);

    return axes;
}


QPainterPath ITransformable::bounds() const
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

    return transform.map(path);
}


QRectF ITransformable::rect() const
{
    QPainterPath path;
    QTransform transform;

    path.addRect(m_x / m_scaleX, m_y / m_scaleY, m_width, m_height);

    transform.translate(m_originX, m_originY);
    transform.scale(m_scaleX, m_scaleY);
    transform.translate(-m_originX, -m_originY);

    return transform.map(path).boundingRect();
}


TransformableEmitter* ITransformable::transformableEmitter()
{
    return &m_emitter;
}


void ITransformable::setMoveSpeed(float speedX, float speedY)
{
    m_speedMoveX = speedX;
    m_speedMoveY = speedY;
}


void ITransformable::setRotateSpeed(float speedX, float speedY, float speedZ)
{
    m_speedRotateX = speedX;
    m_speedRotateY = speedY;
    m_speedRotateZ = speedZ;
}


void ITransformable::setRotateAxes(RotateAxes axes)
{
    m_rotateAxes = axes;
}


void ITransformable::setRotateMode(RotateMode mode)
{
    m_rotateMode = mode;
}


void ITransformable::setScaleSpeed(float speedX, float speedY)
{
    m_speedScaleX = speedX;
    m_speedScaleY = speedY;
}


void ITransformable::setX(float x)
{
    m_x = x;
    m_emitter.emitPositionChanged();
}


void ITransformable::setY(float y)
{
    m_y = y;
    m_emitter.emitPositionChanged();
}


void ITransformable::setAngle(float degrees)
{
    setAngle(0, 0, degrees);
}


void ITransformable::setAngle(float x, float y, float z)
{
    m_angleX = x;
    m_angleY = y;
    m_angleZ = z;
}


void ITransformable::setScale(float scaleX, float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;
    m_emitter.emitSizeChanged();
}


void ITransformable::setOpacity(float opacity)
{
    m_opacity = opacity;
}


void ITransformable::setPosition(const QVector2D& pos)
{
    m_x = pos.x();
    m_y = pos.y();
    m_emitter.emitPositionChanged();
}


void ITransformable::setOrigin(const QVector2D& origin)
{
    m_originX = origin.x();
    m_originY = origin.y();
}


void ITransformable::startMovingBy(float advanceX, float advanceY)
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


void ITransformable::startMovingTo(float targetX, float targetY)
{
    m_isMovingX = false;
    m_isMovingY = false;
    m_moveDir = MoveNone;

    // Gathers amount of pixels to move in real coordinates.
    auto b = rect();
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


void ITransformable::startRotating(bool cwX, bool cwY, bool cwZ)
{
    if (m_rotateMode != RotateForever) return;

    m_isRotatingX = (m_rotateAxes & AxisX);
    m_isRotatingY = (m_rotateAxes & AxisY);
    m_isRotatingZ = (m_rotateAxes & AxisZ);

    m_rotateDirX  = cwX ? RotateCW : RotateCCW;
    m_rotateDirY  = cwY ? RotateCW : RotateCCW;
    m_rotateDirZ  = cwZ ? RotateCW : RotateCCW;
}


void ITransformable::startRotatingBy(float advance)
{
    startRotatingBy(0, 0, advance);
}


void ITransformable::startRotatingBy(float advanceX, float advanceY, float advanceZ)
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


void ITransformable::startRotatingTo(float target)
{
    startRotatingTo(0, 0, target);
}


void ITransformable::startRotatingTo(float targetX, float targetY, float targetZ)
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


void ITransformable::startScalingTo(float scaleX, float scaleY)
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


void ITransformable::startFadingTo(float target)
{
    // Do not accept negative values.
    target = (float)(uchar) qAbs(target);

    // Calculates the target opacity.
    if (target <= m_opacity) m_fadeDir = FadeOut;
    else m_fadeDir = FadeIn;

    m_targetOpacity = target;
    m_isFading = true;
}


void ITransformable::stopMoving()
{
    m_isMovingX = false;
    m_isMovingY = false;
    m_emitter.emitStoppedMoving();
}


void ITransformable::stopRotating()
{
    m_isRotatingX = false;
    m_isRotatingY = false;
    m_isRotatingZ = false;
    m_emitter.emitStoppedRotating();
}


void ITransformable::stopScaling()
{
    m_isScalingX = false;
    m_isScalingY = false;
    m_emitter.emitStoppedScaling();
}


void ITransformable::stopFading()
{
    m_isFading = false;
    m_emitter.emitStoppedFading();
}


void ITransformable::updateTransform(const GameTime& time)
{
    updateMove(time.deltaTime());
    updateRotate(time.deltaTime());
    updateScale(time.deltaTime());
    updateFade(time.deltaTime());
}


void ITransformable::setSize(float width, float height)
{
    m_width = width;
    m_height = height;

    m_emitter.emitSizeChanged();
}


QPointF ITransformable::visiblePos(float x, float y)
{
    QPainterPath path;
    QTransform transform;

    path.addRect(x * m_scaleX, y * m_scaleY, m_width, m_height);

    transform.translate(m_originX, m_originY);
    transform.scale(-m_scaleX, -m_scaleY);
    transform.translate(-m_originX, -m_originY);

    return transform.map(path).boundingRect().topLeft();
}


void ITransformable::updateMove(double delta)
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


void ITransformable::updateRotate(double delta)
{
    if (m_isRotatingX)
    {
        if (m_rotateDirX == RotateCW)
        {
            m_angleX += (m_speedRotateX * delta);
            if (m_rotateMode == RotateOnce && m_angleX >= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                stopRotating();
            }
        }
        else
        {
            m_angleX -= (m_speedRotateX * delta);
            if (m_rotateMode == RotateOnce && m_angleX <= m_targetRotateX)
            {
                m_angleX = m_targetRotateX;
                stopRotating();
            }
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
                stopRotating();
            }
        }
        else
        {
            m_angleY -= (m_speedRotateY * delta);
            if (m_rotateMode == RotateOnce && m_angleY <= m_targetRotateY)
            {
                m_angleY = m_targetRotateY;
                stopRotating();
            }
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
                stopRotating();
            }
        }
        else
        {
            m_angleZ -= (m_speedRotateZ * delta);
            if (m_rotateMode == RotateOnce && m_angleZ <= m_targetRotateZ)
            {
                m_angleZ = m_targetRotateZ;
                stopRotating();
            }
        }
    }
}


void ITransformable::updateScale(double delta)
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


void ITransformable::updateFade(double delta)
{
    if (m_isFading)
    {
        if (m_fadeDir == FadeIn)
        {
            m_opacity += (m_speedFade * delta);
            if (m_opacity >= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                stopFading();
            }
        }
        else
        {
            m_opacity -= (m_speedFade * delta);
            if (m_opacity <= m_targetOpacity)
            {
                m_opacity = m_targetOpacity;
                stopFading();
            }
        }
    }
}


void ITransformable::checkMove()
{
    if (!m_isMovingX && !m_isMovingY) stopMoving();
}


void ITransformable::checkScale()
{
    if (!m_isScalingX && !m_isScalingY) stopScaling();
}


ITransformable::operator QString() const
{
    QString s;
    QRectF br = rect();
    QString sx = " x=" + QString::number(m_x);
    QString sy = " y=" + QString::number(m_y);
    QString sw = " w=" + QString::number(m_width);
    QString sh = " h=" + QString::number(m_height);
    QString rx = " x=" + QString::number(br.x());
    QString ry = " y=" + QString::number(br.y());
    QString rw = " w=" + QString::number(br.width());
    QString rh = " h=" + QString::number(br.height());
    QString ax = " x=" + QString::number(m_angleX);
    QString ay = " y=" + QString::number(m_angleY);
    QString az = " z=" + QString::number(m_angleZ);
    QString cx = " x=" + QString::number(m_scaleX);
    QString cy = " y=" + QString::number(m_scaleY);
    QString ox = " x=" + QString::number(m_originX);
    QString oy = " y=" + QString::number(m_originY);

    s.append("-- Transformable\n");
    s.append(QString("Position:") + sx + sy + "\n");
    s.append(QString("Size:") + sw + sh + "\n");
    s.append(QString("Real position:") + rx + ry + "\n");
    s.append(QString("Real size:") + rw + rh + "\n");
    s.append(QString("Origin:") + ox + oy + "\n");
    s.append(QString("Angle:") + ax + ay + az + "\n");
    s.append(QString("Scale:") + cx + cy + "\n");
    s.append(QString("Opacity: ") + QString::number(m_opacity) + "\n");
    s.append(QString("Is moving: ") + ((isMoving())? "true\n" : "false\n"));
    s.append(QString("Is rotating: ") + ((isRotating())? "true\n" : "false\n"));
    s.append(QString("Is scaling: ") + ((isScaling())? "true\n" : "false\n"));
    s.append(QString("Is fading: ") + ((isFading())? "true\n" : "false\n\n"));

    return s;
}

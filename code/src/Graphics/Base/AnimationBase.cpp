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
#include <Cranberry/Graphics/Base/AnimationBase.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - The given render target is invalid.")
CRANBERRY_CONST_VAR(qint32, c_maxSize, 4096)


CRANBERRY_USING_NAMESPACE


AnimationBase::AnimationBase()
    : RenderBase()
    , m_mode(AnimateOnce)
    , m_currentFrame(nullptr)
    , m_elapsedTime(0.0)
    , m_isAnimating(false)

{
}


AnimationBase::~AnimationBase()
{
    destroy();
}


bool AnimationBase::isNull() const
{
    return RenderBase::isNull()     ||
           m_currentFrame == nullptr ||
           m_frames.isEmpty()        ||
           m_atlases.isEmpty();
}


bool AnimationBase::isAnimating() const
{
    return m_isAnimating;
}


int AnimationBase::frameCount() const
{
    return m_frames.size();
}


void AnimationBase::destroy()
{
    for (TextureAtlas* atlas : m_atlases)
    {
        delete atlas;
    }

    m_frames.clear();
    m_atlases.clear();
    m_currentFrame = nullptr;
    m_isAnimating = false;

    RenderBase::destroy();
}


void AnimationBase::startAnimation(AnimationMode mode)
{
    m_mode = mode;
    m_elapsedTime = 0.0;
    m_currentFrame = &m_frames.first();
    m_isAnimating = true;
}


void AnimationBase::startIdle()
{
    m_currentFrame = &m_idleFrame;
    m_isAnimating = false;
}


void AnimationBase::resumeAnimation()
{
    m_isAnimating = true;
}


void AnimationBase::stopAnimation()
{
    m_emitter.emitStoppedAnimating();
    m_isAnimating = false;
}


void AnimationBase::update(const GameTime& time)
{
    updateTransform(time);

    // Updates the animation.
    if (m_isAnimating && Q_LIKELY(!isNull()))
    {
        m_elapsedTime += time.deltaTime();
        if (m_elapsedTime >= m_currentFrame->duration())
        {
            // Jump to the next frame and handle overflow.
            int frameIndex = m_currentFrame->frameId() + 1;
            if (frameIndex >= m_frames.size())
            {
                frameIndex = 0;

                if (m_mode == AnimateOnce)
                {
                    m_isAnimating = false;
                    m_emitter.emitStoppedAnimating();
                }
            }

            // Specify the next frame and reset timer.
            m_elapsedTime = 0.0;
            m_currentFrame = &m_frames[frameIndex];
        }
    }

    // Copies all transformations.
    TextureBase* texture = getCurrentTexture();
    {
        texture->setShaderProgram(shaderProgram());
        texture->setSourceRectangle(m_currentFrame->rectangle());
        copyTransform(this, texture);
    }
}


void AnimationBase::render()
{
    if (!prepareRendering()) return;

    // Renders the current texture.
    getCurrentTexture()->render();
}


void AnimationBase::setIdleFrame(uint atlas, const QRectF& frame)
{
    m_idleFrame.setAtlasId(atlas);
    m_idleFrame.setRectangle(frame);
    m_idleFrame.setDuration(0.0);
    m_idleFrame.setFrameId(-1);
}


void AnimationBase::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void AnimationBase::setBlendColor(
    const QColor& tl,
    const QColor& tr,
    const QColor& br,
    const QColor& bl
    )
{

    for (TextureAtlas* atlas : m_atlases)
    {
        atlas->texture()->setBlendColor(tl, tr, br, bl);
    }
}


void AnimationBase::setBlendMode(BlendModes modes)
{
    for (TextureAtlas* atlas : m_atlases)
    {
        atlas->texture()->setBlendMode(modes);
    }
}


void AnimationBase::setEffect(Effect effect)
{
    for (auto* atlas : m_atlases)
    {
        atlas->texture()->setEffect(effect);
    }
}


AnimationEmitter* AnimationBase::animationEmitter()
{
    return &m_emitter;
}


bool AnimationBase::createInternal(
    const QVector<QImage>& frames,
    const QVector<qreal>& durations,
          Window* rt
    )
{
    if (!RenderBase::create(rt)) return false;

    qint32 maxSize = qMin(c_maxSize, TextureBase::maxSize());
    TextureAtlas* currentAtlas = new TextureAtlas(maxSize, renderTarget());
    QSize largestSize;

    AnimationFrame currentFrame;
    qint32 frameCount = frames.size();

    // Finds a nice place in the atlas, for every frame.
    for (int i = 0; i < frameCount; i++)
    {
        const QImage& img = frames.at(i);
        const qreal& dura = durations.at(i);
        if (!currentAtlas->canInsert())
        {
            // Atlas is full; create new one.
            m_atlases.append(currentAtlas);
            currentAtlas = new TextureAtlas(maxSize, renderTarget());
        }

        currentAtlas->insert(img);
        currentFrame.setAtlasId(m_atlases.size());
        currentFrame.setDuration(dura / 1000.0);
        currentFrame.setRectangle(currentAtlas->lastRectangle());
        currentFrame.setFrameId(i);

        m_frames.push_back(currentFrame);

        // Finds the largest image.
        largestSize.rwidth() = qMax(img.width(), largestSize.width());
        largestSize.rheight() = qMax(img.height(), largestSize.height());
    }

    // Adds the last atlas to the list.
    m_atlases.append(currentAtlas);
    m_currentFrame = &m_frames.first();

    setSize(largestSize.width(), largestSize.height());
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return true;
}


bool AnimationBase::createInternal(
    const QVector<QImage>& sheets,
    const QVector<AnimationFrame>& frames,
          Window* rt
    )
{
    if (!RenderBase::create(rt)) return false;

    QSize largestSize;

    // Creates texture atlases out of the sheets.
    for (const QImage& img : sheets)
    {
        m_atlases.append(new TextureAtlas(img, renderTarget()));

        // Finds the largest image.
        largestSize.rwidth() = qMax(img.width(), largestSize.width());
        largestSize.rheight() = qMax(img.height(), largestSize.height());
    }

    // Simply copies the frames since everything is prepared.
    m_frames = frames;
    m_currentFrame = &m_frames.first();

    setSize(largestSize.width(), largestSize.height());
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return true;
}


TextureBase* AnimationBase::getCurrentTexture()
{
    return m_atlases
           [
               m_currentFrame
              ->atlasId()
           ]  ->texture();
}


AnimationBase::operator QString() const
{
    QRectF r = m_currentFrame->rectangle();
    QString sx = " x=" + QString::number(r.x());
    QString sy = " y=" + QString::number(r.y());
    QString sw = " w=" + QString::number(r.width());
    QString sh = " h=" + QString::number(r.height());
    QString s;

    s.append(RenderBase::operator QString());
    s.append(TransformBase::operator QString());
    s.append("-- Animation\n");
    s.append(QString("Is animating: ") + ((m_isAnimating) ? "true\n" : "false\n"));
    s.append(QString("Frame amount: ") + QString::number(m_frames.size()) + "\n");
    s.append(QString("Current frame: ") + QString::number(m_currentFrame->frameId()) + "\n");
    s.append(QString("Current rect:") + sx + sy + sw + sh + "\n\n");

    return s;
}

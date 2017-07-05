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
#include <Cranberry/Graphics/Base/IAnimation.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - The given render target is invalid.")
CRANBERRY_CONST_VAR(qint32, c_maxSize, 4096)


CRANBERRY_USING_NAMESPACE


IAnimation::IAnimation()
    : IRenderable()
    , ITransformable()
    , m_mode(AnimateOnce)
    , m_currentFrame(nullptr)
    , m_elapsedTime(0.0)
    , m_isAnimating(false)

{
}


IAnimation::~IAnimation()
{
    destroy();
}


bool IAnimation::isNull() const
{
    return IRenderable::isNull()     ||
           m_currentFrame == nullptr ||
           m_frames.isEmpty()        ||
           m_atlases.isEmpty();
}


bool IAnimation::isAnimating() const
{
    return m_isAnimating;
}


bool IAnimation::createRawAnimation(
        const QVector<QImage> &frames,
        const QVector<qreal> &durations,
        Window* renderTarget
        )
{
    return createInternal(frames, durations, renderTarget);
}


void IAnimation::destroy()
{
    for (auto* atlas : m_atlases) delete atlas;

    m_frames.clear();
    m_atlases.clear();
    m_currentFrame = nullptr;

    IRenderable::destroy();
}


void IAnimation::startAnimation(AnimationMode mode)
{
    m_mode = mode;
    m_isAnimating = true;
}


void IAnimation::stopAnimation()
{
    m_emitter.emitStoppedAnimating();
    m_isAnimating = false;
}


void IAnimation::update(const GameTime& time)
{
    updateTransform(time);

    // Updates the animation.
    if (m_isAnimating && Q_LIKELY(!isNull()))
    {
        m_elapsedTime += time.deltaTime();
        if (m_elapsedTime >= m_currentFrame->duration)
        {
            // Jump to the next frame and handle overflow.
            int frameIndex = m_currentFrame->frame + 1;
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
}


void IAnimation::render()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_01));
        return;
    }

    // Copies all transformations and renders the current texture.
    ITexture* texture = m_atlases[m_currentFrame->atlas]->texture();
    texture->setShaderProgram(shaderProgram());
    texture->setPosition(pos());
    texture->setAngle(angle());
    texture->setOpacity(opacity());
    texture->setScale(scaleX(), scaleY());
    texture->setSourceRectangle(m_currentFrame->rect);
    texture->render();
}


void IAnimation::setBlendColor(const QColor& color)
{
    setBlendColor(color, color, color, color);
}


void IAnimation::setBlendColor(
        const QColor& tl,
        const QColor& tr,
        const QColor& br,
        const QColor& bl
        )
{

    for (auto* atlas : m_atlases)
    {
        atlas->texture()->setBlendColor(tl, tr, br, bl);
    }
}


void IAnimation::setBlendMode(BlendModes modes)
{
    for (auto* atlas : m_atlases)
    {
        atlas->texture()->setBlendMode(modes);
    }
}


void IAnimation::setEffect(Effect effect)
{
    for (auto* atlas : m_atlases)
    {
        atlas->texture()->setEffect(effect);
    }
}


AnimationEmitter* IAnimation::animationEmitter()
{
    return &m_emitter;
}


void IAnimation::saveAtlas(TextureAtlas* atlas)
{
    m_atlases.append(atlas);
}


bool IAnimation::createInternal(
        const QVector<QImage>& frames,
        const QVector<qreal>& durations,
        Window* rt
        )
{
    if (!IRenderable::create(rt)) return false;

    renderTarget()->makeCurrent();

    qint32 maxSize = qMin(c_maxSize, ITexture::maxSize());
    TextureAtlas* currentAtlas = new TextureAtlas(maxSize, renderTarget());
    Frame currentFrame;

    // Finds a nice place in the atlas for every frame.
    for (int i = 0; i < frames.size(); i++)
    {
        const QImage& img = frames.at(i);
        const qreal& dura = durations.at(i);
        if (!currentAtlas->canInsert())
        {
            // Atlas is full; create new one.
            saveAtlas(currentAtlas);
            currentAtlas = new TextureAtlas(maxSize, renderTarget());
        }

        currentAtlas->insert(img);
        currentFrame.atlas = m_atlases.size();
        currentFrame.duration = dura / 1000.0; // to seconds
        currentFrame.rect = currentAtlas->lastRectangle();
        currentFrame.frame = i;

        m_frames.push_back(currentFrame);
    }

    // Adds the last atlas to the list.
    saveAtlas(currentAtlas);
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    m_currentFrame = &m_frames[0];

    return true;
}

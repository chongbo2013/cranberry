////////////////////////////////////////////////////////////////////////////////
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


// Cranberry headers
#include <Cranberry/Graphics/Base/AnimationBase.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
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
    , m_isEmbedded(false)

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


void AnimationBase::beginAnimation(AnimationMode mode)
{
    m_mode = mode;
    m_elapsedTime = 0.0;
    m_currentFrame = &m_frames.first();
    m_isAnimating = true;
}


void AnimationBase::beginIdle()
{
    m_currentFrame = &m_idleFrame;
    m_isAnimating = false;
}


void AnimationBase::resumeAnimation()
{
    m_isAnimating = true;
}


void AnimationBase::endAnimation()
{
    m_emitter.emitFinishedAnimation();
    m_isAnimating = false;
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


AnimationBaseEmitter* AnimationBase::signals()
{
    return &m_emitter;
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
                    m_emitter.emitFinishedAnimation();
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
    setOrigin(width() / 2, height() / 2);
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
    setOrigin(width() / 2, height() / 2);
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return true;
}


TreeModelItem* AnimationBase::rootModelItem()
{
    return m_rootModelItem;
}


QString getAnimModeString(AnimationMode mode)
{
    switch (mode)
    {
    case AnimateOnce:    return "Once";
    case AnimateForever: return "Forever";
    default:             return "Unknown";
    }
}


void AnimationBase::createProperties(TreeModel* model)
{
    TreeModelItem* tmiMode = new TreeModelItem("Mode", getAnimModeString(m_mode));
    TreeModelItem* tmiFram = new TreeModelItem("Frame count", m_frames.size());
    TreeModelItem* tmiAtla = new TreeModelItem("Atlas count", m_atlases.size());
    TreeModelItem* tmiAnim = new TreeModelItem("Is animating?", m_isAnimating);
    TreeModelItem* tmiIdle = new TreeModelItem("Idle frame");
    TreeModelItem* tmiIdDu = new TreeModelItem("Duration", m_idleFrame.duration());
    TreeModelItem* tmiIdRe = new TreeModelItem("Rectangle");
    TreeModelItem* tmiIdRx = new TreeModelItem("x", m_idleFrame.rectangle().x());
    TreeModelItem* tmiIdRy = new TreeModelItem("y", m_idleFrame.rectangle().y());
    TreeModelItem* tmiIdRw = new TreeModelItem("w", m_idleFrame.rectangle().width());
    TreeModelItem* tmiIdRh = new TreeModelItem("h", m_idleFrame.rectangle().height());
    TreeModelItem* tmiCurr = new TreeModelItem("Current frame");
    TreeModelItem* tmiCuId = new TreeModelItem("Frame number", m_currentFrame->frameId());
    TreeModelItem* tmiAtId = new TreeModelItem("Atlas number", m_currentFrame->atlasId());
    TreeModelItem* tmiCuDu = new TreeModelItem("Duration", m_currentFrame->duration());
    TreeModelItem* tmiCuRe = new TreeModelItem("Rectangle");
    TreeModelItem* tmiCuRx = new TreeModelItem("x", m_currentFrame->rectangle().x());
    TreeModelItem* tmiCuRy = new TreeModelItem("y", m_currentFrame->rectangle().y());
    TreeModelItem* tmiCuRw = new TreeModelItem("w", m_currentFrame->rectangle().width());
    TreeModelItem* tmiCuRh = new TreeModelItem("h", m_currentFrame->rectangle().height());

    m_rootModelItem = new TreeModelItem("AnimationBase");
    m_rootModelItem->appendChild(tmiMode);
    m_rootModelItem->appendChild(tmiFram);
    m_rootModelItem->appendChild(tmiAtla);
    m_rootModelItem->appendChild(tmiAnim);
    m_rootModelItem->appendChild(tmiIdle);
    m_rootModelItem->appendChild(tmiCurr);

    tmiIdle->appendChild(tmiIdDu);
    tmiIdle->appendChild(tmiIdRe);
    tmiCurr->appendChild(tmiCuId);
    tmiCurr->appendChild(tmiAtId);
    tmiCurr->appendChild(tmiCuDu);
    tmiCurr->appendChild(tmiCuRe);
    tmiIdRe->appendChild(tmiIdRx);
    tmiIdRe->appendChild(tmiIdRy);
    tmiIdRe->appendChild(tmiIdRw);
    tmiIdRe->appendChild(tmiIdRh);
    tmiCuRe->appendChild(tmiCuRx);
    tmiCuRe->appendChild(tmiCuRy);
    tmiCuRe->appendChild(tmiCuRw);
    tmiCuRe->appendChild(tmiCuRh);

    // The animation is also used by other classes; give the possibility to
    // simply append the root item to another item.
    if (model != nullptr)
    {
        model->addItem(m_rootModelItem);
        RenderBase::createProperties(model);
    }
    else
    {
        m_isEmbedded = true;
    }
}


void AnimationBase::updateProperties()
{
    TreeModelItem* tmiIdle = m_rootModelItem->childAt(4);
    TreeModelItem* tmiCurr = m_rootModelItem->childAt(5);

    m_rootModelItem->childAt(0)->setValue(getAnimModeString(m_mode));
    m_rootModelItem->childAt(1)->setValue(m_frames.size());
    m_rootModelItem->childAt(2)->setValue(m_atlases.size());
    m_rootModelItem->childAt(3)->setValue(m_isAnimating);

    tmiIdle->childAt(0)->setValue(m_idleFrame.duration());
    tmiIdle->childAt(1)->childAt(0)->setValue(m_idleFrame.rectangle().x());
    tmiIdle->childAt(1)->childAt(1)->setValue(m_idleFrame.rectangle().y());
    tmiIdle->childAt(1)->childAt(2)->setValue(m_idleFrame.rectangle().width());
    tmiIdle->childAt(1)->childAt(3)->setValue(m_idleFrame.rectangle().height());
    tmiCurr->childAt(0)->setValue(m_currentFrame->frameId());
    tmiCurr->childAt(1)->setValue(m_currentFrame->atlasId());
    tmiCurr->childAt(2)->setValue(m_currentFrame->duration());
    tmiCurr->childAt(3)->childAt(0)->setValue(m_currentFrame->rectangle().x());
    tmiCurr->childAt(3)->childAt(1)->setValue(m_currentFrame->rectangle().y());
    tmiCurr->childAt(3)->childAt(2)->setValue(m_currentFrame->rectangle().width());
    tmiCurr->childAt(3)->childAt(3)->setValue(m_currentFrame->rectangle().height());

    if (!m_isEmbedded)
    {
        RenderBase::updateProperties();
    }
}


TextureBase* AnimationBase::getCurrentTexture()
{
    return m_atlases
           [
               m_currentFrame
              ->atlasId()
           ]  ->texture();
}

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


// Cranberry headers
#include <Cranberry/Graphics/Animation/Animation.hpp>
#include <Cranberry/Graphics/System/TextureAtlas.hpp>
#include <Cranberry/System/DebugLog.hpp>

// Qt headers
#include <QImageReader>
#include <QPainter>
#include <QOpenGLTexture>


CRANBERRY_BEGIN_NAMESPACE


Animation::Animation()
    : Fadable()
    , Transformable()
    , m_currentFrame(nullptr)
    , m_renderTarget(nullptr)
    , m_elapsedTime(0.0)
    , m_isAnimating(false)
{
}


Animation::Animation(const Animation& other)
    : Fadable(other)
    , Transformable(other)
    , m_frames(other.m_frames)
    , m_textures(other.m_textures)
    , m_currentFrame(other.m_currentFrame)
    , m_renderTarget(other.m_renderTarget)
    , m_elapsedTime(other.m_elapsedTime)
    , m_isAnimating(other.m_isAnimating)
{
}


Animation& Animation::operator =(const Animation& other)
{
    m_frames = other.m_frames;
    m_textures = other.m_textures;
    m_currentFrame = other.m_currentFrame;
    m_renderTarget = other.m_renderTarget;
    m_elapsedTime = other.m_elapsedTime;
    m_isAnimating = other.m_isAnimating;

    return *this;
}


Animation::~Animation()
{
    // Destroys every allocated texture as last resort.
    destroy();
}


bool Animation::isValid() const
{
    return m_renderTarget != nullptr;
}


bool Animation::isAnimating() const
{
    return m_isAnimating;
}


bool Animation::create(const QString& path, Window* renderTarget)
{
    // Determines whether the given file exists.
    QFileInfo info(path);
    QString extension = info.suffix();
    if (!info.exists() || (extension != "gif" && extension != "cba"))
    {
        cranError(QString("Asset '%0' does not exist!").arg(path).toStdString());
        return false;
    }

    // Depending on the extension, loads a GIF or CBA file.
    if (extension == "gif")
    {
        QImage image, bigImage(1024, 1024, QImage::Format_ARGB32);
        QImageReader reader(path);
        TextureAtlas atlas(1024, 1024);
        bigImage.fill(Qt::transparent);

        // Attempts to read all frames.
        auto count = reader.imageCount();
        for (int i = 0; i < count; i++)
        {
            if (!reader.read(&image))
            {
                cranError(QString("Asset '%0' is not a valid GIF file!").arg(path).toStdString());
                m_textures.clear();
                m_frames.clear();
                return false;
            }

            QRectF fit = atlas.find(image.size());
            if (fit.isNull())
            {
                // Before creating new texture, save old one.
                Image tex;
                tex.create(bigImage, renderTarget);
                m_textures.push_back(tex);

                // No space - requires new texture.
                atlas.reset(1024, 1024);
                bigImage.fill(Qt::transparent);
            }

            // Places the image inside the texture.
            drawInto(&bigImage, image, fit);

            // Creates an animation frame instance.
            AnimationFrame frame;
            frame.setDuration(reader.nextImageDelay() / 1000.0);
            frame.setFrameIndex(m_frames.size());
            frame.setTextureIndex(m_textures.size());
            frame.setTextureRect(fit);
            m_frames.push_back(frame);
        }

        // Adds the last texture.
        Image tex;
        tex.create(bigImage, renderTarget);
        m_textures.push_back(tex);
        m_currentFrame = &m_frames[0];
    }
    else
    {
    }


    // Retrieves the current window if 'renderTarget' is unspecified.
    if ((m_renderTarget = renderTarget) == nullptr)
    {
        if ((m_renderTarget = Window::activeWindow()) == nullptr)
        {
            cranError("Animation::create: No render target specified!");
            m_textures.clear();
            m_frames.clear();
            return false;
        }
    }

    return true;
}


bool Animation::create(const std::vector<AnimationFrame>& frames,
                       const std::vector<Image>& images,
                       Window* renderTarget)
{
    m_frames = frames;
    m_textures = images;

    if (m_textures.size() == 0 || m_frames.size() == 0)
    {
        cranError("Animation::create: No frames or textures specified.");
        return false;
    }
    else if ((m_renderTarget = renderTarget) == nullptr)
    {
        // Retrieves the current window if 'renderTarget' is unspecified.
        if ((m_renderTarget = Window::activeWindow()) == nullptr)
        {
            cranError("Animation::create: No render target specified.");
            return false;
        }
    }

    m_currentFrame = &m_frames[0];
    return true;
}


void Animation::destroy()
{
    if (isValid())
    {
        m_textures.clear();
        m_frames.clear();
        m_currentFrame = nullptr;
        m_isAnimating = false;
    }
}


void Animation::startAnimation()
{
    m_isAnimating = true;
}


void Animation::stopAnimation()
{
    m_isAnimating = false;
}


void Animation::update(const GameTime& gameTime)
{
    updateFade(gameTime);
    updateTransform(gameTime);

    // Updates the animation.
    if (isValid() && m_isAnimating)
    {
        if (m_elapsedTime >= m_currentFrame->duration())
        {
            // Jump to next frame; handles overflow.
            int frameIndex = m_currentFrame->frameIndex() + 1;
            if (frameIndex >= (int) m_frames.size())
                frameIndex = 0;

            // Specify next frame and reset timer.
            m_elapsedTime = 0.0;
            m_currentFrame = &m_frames[frameIndex];
        }
    }
}


void Animation::render()
{
    if (Q_UNLIKELY(isValid()))
    {
        // Copies all transformations and renders the current texture.
        Image* image = &m_textures[m_currentFrame->textureIndex()];
        image->setPosition(pos());
        image->setAngle(angle());
        image->setOpacity(opacity());
        image->setScale(scale());
        image->setSourceRectangle(m_currentFrame->textureRect());
        image->render();
    }
}


void Animation::drawInto(QImage* dst, QImage& src, QRectF& rc)
{
    QPainter painter(dst);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(rc, src);
}


CRANBERRY_END_NAMESPACE

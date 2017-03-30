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
    , m_isCreated(false)
{
}


Animation::Animation(const Animation& other)
    : Fadable(other)
    , Transformable(other)
    , m_frames(other.m_frames)
    , m_textures(other.m_textures)
{
}


Animation& Animation::operator =(const Animation& other)
{
    m_frames = other.m_frames;
    m_textures = other.m_textures;

    return *this;
}


Animation::~Animation()
{
    // Destroys every allocated texture.
    for (auto* tex : m_textures)
    {
        tex->destroy();
        delete tex;
    }
}


bool Animation::isNull() const
{
    return m_frames.size() == 0;
}


bool Animation::create(const QString& path, Window* renderTarget)
{
    m_renderTarget = renderTarget;

    // Determines whether the given file exists.
    QFileInfo info(path);
    QString extension = info.suffix();
    if (!info.exists() || (extension != "gif" && extension != "cba"))
    {
        cranError(QString("Asset '%0' does not exist!").arg(path).toStdString());
        return false;
    }

    // Retrieves the current window if 'renderTarget' is unspecified.
    if (m_renderTarget == nullptr)
    {
        if ((m_renderTarget = Window::activeWindow()) == nullptr)
        {
            cranError("Animation::create: No render target specified!");
            return false;
        }
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
                return false;
            }

            QRectF fit = atlas.find(image.size());
            if (fit.isNull())
            {
                // Before creating new texture, save old one.
                Image* tex = new Image;
                tex->create(bigImage, m_renderTarget);
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
        Image* tex = new Image;
        tex->create(bigImage, m_renderTarget);
        m_textures.push_back(tex);
    }
    else
    {
    }

    m_isCreated = true;
    return true;
}


bool Animation::create(const std::vector<AnimationFrame>& frames,
                       const std::vector<Image*>& images,
                       Window* renderTarget)
{
    m_frames = frames;
    m_textures = images;

    if ((m_renderTarget = renderTarget) == nullptr)
    {
        // Retrieves the current window if 'renderTarget' is unspecified.
        if ((m_renderTarget = Window::activeWindow()) == nullptr)
        {
            cranError("Animation::create: No render target specified!");
            return false;
        }
    }

    m_isCreated = true;
    return true;
}


void Animation::drawInto(QImage* dst, QImage& src, QRectF& rc)
{
    QPainter painter(dst);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(rc, src);
}


CRANBERRY_END_NAMESPACE

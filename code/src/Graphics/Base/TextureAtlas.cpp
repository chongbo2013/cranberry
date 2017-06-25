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
#include <Cranberry/Graphics/Base/TextureAtlas.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLTexture>
#include <QOpenGLFunctions>


CRANBERRY_USING_NAMESPACE


TextureAtlas::TextureAtlas(int size, Window* renderTarget)
    : m_size(size)
    , m_usedSpace(0)
    , m_occupancy(0.0)
    , m_texture(new ITexture)
{
    if (renderTarget == nullptr)
    {
        if ((renderTarget = Window::activeWindow()) == nullptr)
        {
            cranError("TextureAtlas: Rendertarget is invalid.");
            return;
        }
    }

    if (QOpenGLContext::currentContext() != renderTarget->context())
    {
        renderTarget->makeCurrent();
    }

    QOpenGLTexture* tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
    if (!tex->create())
    {
        cranError("TextureAtlas: Texture could not be created.");
        return;
    }

    tex->setSize(size, size);
    tex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
    if (!tex->isStorageAllocated())
    {
        cranError("TextureAtlas: Texture storage could not be allocated.");
        return;
    }

    m_texture->create(tex, renderTarget);
    m_texId = tex->textureId();
    gl = renderTarget->functions();
}


bool TextureAtlas::insert(const QImage& img)
{
    // Finds a fit.
    int index;
    auto fit = find(img.width(), img.height(), index);
    if (fit.isNull()) return false;

    // Performs a clean-up.
    split(m_free.at(index), fit);
    m_free.erase(m_free.begin() + index);
    merge();

    // Places the rectangle inside the texture.
    m_used.push_back(fit);
    m_usedSpace += (fit.width() * fit.height());
    m_occupancy = (m_usedSpace / (m_size * m_size));
    drawIntoTexture(img, fit);

    return true;
}


bool TextureAtlas::canInsert() const
{
    return m_occupancy < 0.9;
}


int TextureAtlas::score(int width, int height, const QRect& free)
{
    return free.width() * free.height() - width * height;
}


QRect TextureAtlas::find(int width, int height, int& index)
{
    QRect bestNode(0, 0, width, height);
    int bestScore = std::numeric_limits<int>::max(), n = 0;
    bool success = false;

    // Finds the best fit.
    for (auto it = m_free.begin(); it < m_free.end(); ++it, ++n)
    {
        // Does fit perfectly in a free rectangle?
        if (width == it->width() && height == it->height())
        {
            bestNode.moveLeft(it->x());
            bestNode.moveTop(it->y());
            bestScore = std::numeric_limits<int>::min();

            index = n;
            success = true;
            break;
        }

        // Does it fit anyhow in the free rectangle?
        else if (width <= it->width() && height <= it->height())
        {
            auto s = score(width, height, *it);
            if (s < bestScore)
            {
                bestNode.moveLeft(it->x());
                bestNode.moveTop(it->y());
                bestScore = s;

                index = n;
                success = true;
            }
        }
    }

    return (success) ? bestNode : QRect();
}


void TextureAtlas::split(const QRect& free, const QRect& used)
{
    // Compute leftover area.
    int w = free.width() - used.width();
    int h = free.height() - used.height();
    bool hsplit = used.width() * h <= used.height() * w;

    // Splits a free rectangle into two new rects.
    QRect bottom(free.x(), free.y() + used.height(), 0, free.height() - used.height());
    QRect right(free.x() + used.width(), free.y(), free.width() - used.width(), 0);

    if (hsplit)
    {
        bottom.setWidth(free.width());
        right.setHeight(used.height());
    }
    else
    {
        bottom.setWidth(used.width());
        right.setHeight(free.height());
    }

    // Adds both rectangles, if valid, to the free ones.
    if (bottom.width() > 0 && bottom.height() > 0) m_free.push_back(bottom);
    if (right.width() > 0 && right.height() > 0) m_free.push_back(right);
}


void TextureAtlas::drawIntoTexture(QImage img, const QRect& src)
{
    // Ensures the image is ARGB32.
    if (img.format() != QImage::Format_ARGB32)
    {
        img = img.convertToFormat(QImage::Format_ARGB32);
    }

    // Writes the new data into the texture.
    glDebug(gl->glBindTexture(GL_TEXTURE_2D, m_texId));
    glDebug(gl->glTexSubImage2D(
                GL_TEXTURE_2D, GL_ZERO,
                src.x(),
                src.y(),
                src.width(),
                src.height(),
                GL_RGBA, GL_UNSIGNED_BYTE,
                img.constBits()
                ));
    glDebug(gl->glBindTexture(GL_TEXTURE_2D, 0));
}


void TextureAtlas::merge()
{
    for (int i = 0; i < m_free.size(); i++)
    {
        QRect& ifree = m_free[i];
        for (int j = i + 1; j < m_free.size(); j++)
        {
            QRect& jfree = m_free[j];
            if (ifree.width() == jfree.width() && ifree.x() == jfree.x())
            {
                if (ifree.y() == jfree.y() + jfree.height())
                {
                    ifree.setTop(ifree.top() - jfree.height());
                    ifree.setHeight(ifree.height() + jfree.height());
                    m_free.erase(m_free.begin() + j--);
                }
                else if (ifree.y() + ifree.height() == jfree.y())
                {
                    ifree.setHeight(ifree.height() + jfree.height());
                    m_free.erase(m_free.begin() + j--);
                }
            }
            else if (ifree.height() == jfree.height() && ifree.y() == jfree.y())
            {
                if (ifree.x() == jfree.x() + jfree.width())
                {
                    ifree.setLeft(ifree.left() - jfree.width());
                    ifree.setWidth(ifree.width() + jfree.width());
                    m_free.erase(m_free.begin() + j--);
                }
                else if (ifree.x() + ifree.width() == jfree.x())
                {
                    ifree.setWidth(ifree.width() + jfree.width());
                    m_free.erase(m_free.begin() + j--);
                }
            }
        }
    }
}

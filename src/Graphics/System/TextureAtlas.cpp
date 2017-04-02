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
#include <Cranberry/Graphics/System/TextureAtlas.hpp>


CRANBERRY_BEGIN_NAMESPACE


TextureAtlas::TextureAtlas(int width, int height)
{
    reset(width, height);
}


void TextureAtlas::reset(int width, int height)
{
    m_width = width;
    m_height = height;
    m_free.clear();
    m_used.clear();
    m_free.push_back(QRect(0, 0, width, height));
}


QRectF TextureAtlas::find(const QSize& requestedSize)
{
    int w = requestedSize.width(),
        h = requestedSize.height(),
        nodeIndex = 0;

    // Finds a fitting rectangle.
    QRect fit = findPrivate(w, h, nodeIndex);
    if (fit.isNull())
        return fit;

    // Performs a clean-up.
    split(m_free.at(nodeIndex), fit);
    m_free.erase(m_free.begin() + nodeIndex);
    merge();

    // Places the rectangle in the texture.
    m_used.push_back(fit);
    return fit;
}


double TextureAtlas::occupancy() const
{
    uint64_t usedArea = 0;
    for (size_t i = 0; i < m_used.size(); i++)
        usedArea += (m_used.at(i).width() * m_used.at(i).height());

    return usedArea / (m_width * m_height);
}


int TextureAtlas::score(int width, int height, const QRect& free)
{
    return free.width() * free.height() - width * height;
    //return max(abs(free.width() - width), abs(free.height() - height));
}


QRect TextureAtlas::findPrivate(int width, int height, int &index)
{
    QRect bestNode(0, 0, width, height);
    int bestScore = std::numeric_limits<int>::max(), n = 0;
    bool success = false;

    // Finds the most fitting free rectangle.
    for (auto it = m_free.begin(); it < m_free.end(); ++it, ++n)
    {
        // Determines whether the rect fits perfectly.
        if (width == it->width() && height == it->height())
        {
            bestNode.moveLeft(it->x());
            bestNode.moveTop(it->y());
            bestScore = std::numeric_limits<int>::min();
            index = n;
            success = true;
            break;
        }

        // Determines whether it fits. Wasted space does not matter.
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

    // Determines whether a horizontal split would be better.
    // Tries to make rectangles more even-sized.
    bool horSplit = (used.width() * h <= used.height() * w);

    // Splits the free rectangle in two new rects.
    QRect bottom(free.x(), free.y() + used.height(), 0, free.height() - used.height());
    QRect right(free.x() + used.width(), free.y(), free.width() - used.width(), 0);
    if (horSplit)
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
    if (bottom.width() > 0 && bottom.height() > 0)
        m_free.push_back(bottom);
    if (right.width() > 0 && right.height() > 0)
        m_free.push_back(right);
}


void TextureAtlas::merge()
{
    for (size_t i = 0; i < m_free.size(); i++)
    {
        QRect& ifree = m_free.at(i);
        for (size_t j = i + 1; j < m_free.size(); j++)
        {
            QRect& jfree = m_free.at(j);
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


CRANBERRY_END_NAMESPACE

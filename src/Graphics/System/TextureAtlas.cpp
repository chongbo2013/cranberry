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
    m_free.clear();
    m_used.clear();
    m_free.push_back(QRect(0, 0, width, height));
}


QRectF TextureAtlas::find(const QSize& requestedSize)
{
    int w = requestedSize.width(),
        h = requestedSize.height();

    // Finds a fitting rectangle.
    QRect fit = score(w, h);
    if (fit.isNull())
        return fit;

    // Places the rectangle in the texture.
    place(fit);
    return fit;
}


bool TextureAtlas::contains(const QRect& r1, const QRect& r2)
{
    int x1 = r1.x(), x2 = r2.x();
    int y1 = r1.y(), y2 = r2.y();
    int w1 = r1.width(), w2 = r2.width();
    int h1 = r1.height(), h2 = r2.height();

    // Determines whether r1 can even contain r2.
    if (x2 < x1 || y2 < y1)
        return false;

    int xx = w1 + x1;
    int yy = h1 + y1;
    int ww = w2 + x2;
    int hh = h2 + y2;

    // Determines whether r1 exceeds r2's bounds.
    if (ww <= x2)
    {
        if (xx >= x1 || ww > xx)
            return false;
    }
    else
    {
        if (xx >= x1 && ww > xx)
            return false;
    }
    if (hh <= y2)
    {
        if (yy >= y1 || hh > yy)
            return false;
    }
    else
    {
        if (yy >= y1 && hh > yy)
            return false;
    }

    return true;
}


void TextureAtlas::place(const QRect& node)
{
    // Inserts the given rectangle in a free region.
    size_t toProcess = m_free.size();
    for (size_t i = 0; i < toProcess;)
    {
        if (split(m_free.at(i), node))
        {
            m_free.erase(m_free.begin() + i);
            toProcess--;
        }
        else
        {
            i++;
        }
    }

    // Reorders the list.
    tidyUp();
    m_used.push_back(node);
}


QRect TextureAtlas::score(int width, int height)
{
    QRect best(0, 0, width, height);
    int bssf = std::numeric_limits<int>::max();
    int blsf = std::numeric_limits<int>::max();
    bool success = false;

    // Gathers free space via "best short side fit".
    for (auto it = m_free.begin(); it != m_free.end(); ++it)
    {
        if (it->width() >= width && it->height() >= height)
        {
            int hleft = std::abs(it->width() - width);
            int vleft = std::abs(it->height() - height);
            int szMin = std::min(hleft, vleft);
            int szMax = std::max(hleft, vleft);

            // Does rectangle fit the space?
            if (szMin < bssf || (szMin == bssf && szMax < blsf))
            {
                best.moveLeft(it->x());
                best.moveTop(it->y());
                bssf = szMin;
                blsf = szMax;
                success = true;
            }
        }
    }

    // Determines whether a space was found.
    if (success)
        return best;
    else
        return QRect();
}


bool TextureAtlas::split(const QRect& free, const QRect& used)
{
    // Do rectangles even intersect?
    if (used.x() >= free.x() + free.width()  || used.x() + used.width()  <= free.x() ||
        used.y() >= free.y() + free.height() || used.y() + used.height() <= free.y())
        return false;

    // Is 'used' inside 'free' horizontally?
    if (used.x() < free.x() + free.width() && used.x() + used.width() > free.x())
    {
        if (used.y() > free.y() && used.y() < free.y() + free.height())
        {
            QRect node = free;
            node.setHeight(used.y() - free.y());
            m_free.push_back(node);
        }
        if (used.y() + used.height() < free.y() + free.height())
        {
            QRect node = free;
            node.moveTop(used.y() + used.height());
            node.setHeight((free.y() + free.height()) - node.y());
            m_free.push_back(node);
        }
    }

    // Is 'used' inside 'free' vertically?
    if (used.y() < free.y() + free.height() && used.y() + used.height() > free.y())
    {
        if (used.x() > free.x() && used.x() < free.x() + free.width())
        {
            QRect node = free;
            node.setWidth(used.x() - free.x());
            m_free.push_back(node);
        }
        if (used.x() + used.width() < free.x() + free.width())
        {
            QRect node = free;
            node.moveLeft(used.x() + used.width());
            node.setWidth((free.x() + free.width()) - node.x());
            m_free.push_back(node);
        }
    }

    return true;
}


void TextureAtlas::tidyUp()
{
    // Removes redundant rectangles.
    for (size_t i = 0; i < m_free.size(); i++)
    {
        for (size_t j = i + 1; j < m_free.size(); j++)
        {
            if (contains(m_free.at(i), m_free.at(j)))
            {
                // i inside j - we can simply delete it.
                m_free.erase(m_free.begin() + (i--));
                break;
            }
            if (contains(m_free.at(j), m_free.at(i)))
            {
                // j inside i - we can simply delete it.
                m_free.erase(m_free.begin() + (j--));
            }
        }
    }
}


CRANBERRY_END_NAMESPACE

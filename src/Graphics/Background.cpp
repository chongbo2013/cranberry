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
#include <Cranberry/Graphics/Background.hpp>

// Qt headers
#include <QOpenGLTexture>


CRANBERRY_BEGIN_NAMESPACE


Background::Background()
    : Image()
    , m_isScrolling(false)
    , m_speedScrollX(100)
    , m_speedScrollY(100)
    , m_scrollX(0)
    , m_scrollY(0)
    , m_scrollTargetX(0)
    , m_scrollTargetY(0)
    , m_scrollingDir(0)
    , m_scrollingMode(0)
{
    // In order to repeat the background to fill the
    // entire window, we need to prepare the texture
    // once the object has been made ready.
    QObject::connect(this, SIGNAL(created()), this, SLOT(prepareTexture()));
}


bool Background::isScrolling() const
{
    return m_isScrolling;
}


void Background::setScrollingPosition(const QPointF& pos)
{
    m_scrollX = pos.x();
    m_scrollY = pos.y();
}


void Background::setScrollingDirection(MoveDirections dir)
{
    m_scrollingDir = static_cast<int>(dir);
}


void Background::setScrollingBehaviour(ScrollingBehaviour sb)
{
    m_scrollingMode = static_cast<int>(sb);
}


void Background::setScrollingSpeed(float speedX, float speedY)
{
    m_speedScrollX = speedX;
    m_speedScrollY = speedY;
}


void Background::startScrolling(float byX, float byY)
{
    if (m_scrollingMode == InfiniteScroll)
    {
        m_isScrolling = true;
        return;
    }

    // Does not accept negative values. Directions are
    // specified in function setScrollingDirection().
    byX = qAbs(byX);
    byY = qAbs(byY);

    // Calculates the target position.
    if ((m_scrollingDir & MoveEast) && (m_scrollingDir & MoveWest) == 0)
        m_scrollTargetX = m_scrollX + byX;
    else if ((m_scrollingDir & MoveWest) && (m_scrollingDir & MoveEast) == 0)
        m_scrollTargetX = m_scrollX - byX;
    if ((m_scrollingDir & MoveSouth) && (m_scrollingDir & MoveNorth) == 0)
        m_scrollTargetY = m_scrollY + byY;
    else if ((m_scrollingDir & MoveNorth) && (m_scrollingDir & MoveSouth) == 0)
        m_scrollTargetY = m_scrollY - byY;

    if (m_scrollingDir != MoveNone)
        m_isScrolling = true;
}


void Background::stopScrolling()
{
    m_isScrolling = false;
}


void Background::update(const GameTime& time)
{
    Image::update(time);

    // Updates the scroll value.
    if (m_isScrolling)
    {
        if ((m_scrollingDir & MoveEast) && (m_scrollingDir & MoveWest) == 0)
        {
            m_scrollX += (m_speedScrollX * time.deltaTime());
            if (m_scrollX >= m_scrollTargetX && m_scrollingMode == LimitedScroll)
            {
                m_scrollX = m_scrollTargetX;
                m_isScrolling = false;
            }

            // Makes sure that the primary image
            // does not leave the window's bounds.
            if (m_scrollX >= renderTarget()->width())
                m_scrollX = renderTarget()->width() - texture()->width();
        }
        else if ((m_scrollingDir & MoveWest) && (m_scrollingDir & MoveEast) == 0)
        {
            m_scrollX -= (m_speedScrollX * time.deltaTime());
            if (m_scrollX <= m_scrollTargetX && m_scrollingMode == LimitedScroll)
            {
                m_scrollX = m_scrollTargetX;
                m_isScrolling = false;
            }

            // Makes sure that the primary image
            // does not leave the window's bounds.
            if (m_scrollX <= -texture()->width())
                m_scrollX = x();
        }
        if ((m_scrollingDir & MoveSouth) && (m_scrollingDir & MoveNorth) == 0)
        {
            m_scrollY += (m_speedScrollY * time.deltaTime());
            if (m_scrollY >= m_scrollTargetY && m_scrollingMode == LimitedScroll)
            {
                m_scrollY = m_scrollTargetY;
                m_isScrolling = false;
            }

            // Makes sure that the primary image
            // does not leave the window's bounds.
            if (m_scrollY >= renderTarget()->height())
                m_scrollY = renderTarget()->height() - texture()->height();
        }
        else if ((m_scrollingDir & MoveNorth) && (m_scrollingDir & MoveSouth) == 0)
        {
            m_scrollY -= (m_speedScrollY * time.deltaTime());
            if (m_scrollY <= m_scrollTargetY && m_scrollingMode == LimitedScroll)
            {
                m_scrollY = m_scrollTargetX;
                m_isScrolling = false;
            }

            // Makes sure that the primary image
            // does not leave the window's bounds.
            if (m_scrollY <= -texture()->height())
                m_scrollY = y();
        }

        updateUVs();
        requestUpdate();
    }
}


void Background::prepareTexture()
{
    // Repeats the texture on each axis.
    texture()->bind();
    texture()->setWrapMode(QOpenGLTexture::Repeat);

    // Calculates the amount of repeated images.
    float winX = renderTarget()->width();
    float winY = renderTarget()->height();
    float repeatX = winX / texture()->width();
    float repeatY = winY / texture()->height();

    // Modifies the vertices to fill the entire window.
    vertices().at(0).xyz(0.f,  0.f,  0.f);
    vertices().at(1).xyz(winX, 0.f,  0.f);
    vertices().at(2).xyz(winX, winY, 0.f);
    vertices().at(3).xyz(0.f,  winY, 0.f);

    // Modifies the texture coordinates to repeat the image.
    vertices().at(0).uv(0.f,     0.f);
    vertices().at(1).uv(repeatX, 0.f);
    vertices().at(2).uv(repeatX, repeatY);
    vertices().at(3).uv(0.f,     repeatY);
}


void Background::updateUVs()
{
    // Calculates the amount of repeated images.
    float winX = renderTarget()->width();
    float winY = renderTarget()->height();
    float repeatX = winX / texture()->width();
    float repeatY = winY / texture()->height();

    // Calculates the texture coordinates.
    float uvX = m_scrollX / texture()->width();
    float uvY = m_scrollY / texture()->height();
    float uvW = repeatX - uvX;
    float uvH = repeatY - uvY;

    // Applies the coordinates.
    vertices().at(0).uv(-uvX, -uvY);
    vertices().at(1).uv(+uvW, -uvY);
    vertices().at(2).uv(+uvW, +uvH);
    vertices().at(3).uv(-uvX, +uvH);
}


CRANBERRY_END_NAMESPACE

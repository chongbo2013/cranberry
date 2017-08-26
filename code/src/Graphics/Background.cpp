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
#include <Cranberry/Graphics/Background.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLTexture>


CRANBERRY_USING_NAMESPACE


Background::Background()
    : TextureBase()
    , m_speedScrollX(100.f)
    , m_speedScrollY(100.f)
    , m_scrollX(0.f)
    , m_scrollY(0.f)
    , m_targetScrollX(0.f)
    , m_targetScrollY(0.f)
    , m_isScrolling(false)
    , m_scrollMode(ScrollNormal)
    , m_scrollDir(MoveNone)
{
}


bool Background::isScrolling() const
{
    return m_isScrolling;
}


void Background::setScrollView(const QRectF& view)
{
    m_view = view;
}


void Background::setScrollPosition(const QPointF& pos)
{
    m_scrollX = pos.x();
    m_scrollY = pos.y();
}


void Background::setScrollMode(ScrollMode mode)
{
    m_scrollMode = mode;
}


void Background::setScrollSpeed(float speedX, float speedY)
{
    m_speedScrollX = speedX;
    m_speedScrollY = speedY;
}


void Background::setScrollDirection(MoveDirections dir)
{
    m_scrollDir = dir;
}


void Background::beginScroll()
{
    if (m_scrollMode == ScrollInfinite)
    {
        m_isScrolling = true;
    }
}


void Background::scrollBy(const QPointF& advance)
{
    // Does not accept negative values. Directions are specified in function
    // Background::setScrollingDirection().
    float byX = qAbs(advance.x());
    float byY = qAbs(advance.y());

    // Calculates the target position (x).
    if ((m_scrollDir & MoveEast) != 0 && (m_scrollDir & MoveWest) == 0)
    {
        m_targetScrollX = m_scrollX + byX;
    }
    else if ((m_scrollDir & MoveEast) == 0 && (m_scrollDir & MoveWest) != 0)
    {
        m_targetScrollX = m_scrollX - byX;
    }

    // Calculates the target position (y).
    if ((m_scrollDir & MoveSouth) != 0 && (m_scrollDir & MoveNorth) == 0)
    {
        m_targetScrollY = m_scrollY + byY;
    }
    else if ((m_scrollDir & MoveSouth) == 0 && (m_scrollDir & MoveNorth) != 0)
    {
        m_targetScrollY = m_scrollY - byY;
    }

    if (m_scrollDir != MoveNone && m_scrollMode != ScrollNone)
    {
        m_isScrolling = true;
    }
}


void Background::scrollTo(const QPointF& pos)
{
    m_scrollDir = MoveNone;

    if (pos.x() > m_scrollX)
    {
        m_scrollDir |= MoveEast;
    }
    else
    {
        m_scrollDir |= MoveWest;
    }

    if (pos.y() > m_scrollY)
    {
        m_scrollDir |= MoveSouth;
    }
    else
    {
        m_scrollDir |= MoveNorth;
    }

    scrollBy(QPointF(qAbs(pos.x() - m_scrollX), qAbs(pos.y() - m_scrollY)));
}


void Background::endScroll()
{
    m_isScrolling = false;
}


void Background::update(const GameTime& time)
{
    TextureBase::update(time);

    // Updates the scroll value.
    if (m_isScrolling)
    {
        if ((m_scrollDir & MoveEast) && (m_scrollDir & MoveWest) == 0)
        {
            m_scrollX += (m_speedScrollX * time.deltaTime());
            if (m_scrollX >= m_targetScrollX && m_scrollMode == ScrollNormal)
            {
                m_scrollX = m_targetScrollX;
                m_isScrolling = false;

                signals()->emitFinishedScroll();
            }

            // Makes sure that the primary image does not leave the window.
            if (m_scrollX >= renderTarget()->width())
            {
                m_scrollX = renderTarget()->width() - texture()->width();
            }
        }
        else if ((m_scrollDir & MoveWest) && (m_scrollDir & MoveEast) == 0)
        {
            m_scrollX -= (m_speedScrollX * time.deltaTime());
            if (m_scrollX <= m_targetScrollX && m_scrollMode == ScrollNormal)
            {
                m_scrollX = m_targetScrollX;
                m_isScrolling = false;

                signals()->emitFinishedScroll();
            }

            // Makes sure that the primary image does not leave the window.
            if (m_scrollX <= -texture()->width())
            {
                m_scrollX = x();
            }
        }
        if ((m_scrollDir & MoveSouth) && (m_scrollDir & MoveNorth) == 0)
        {
            m_scrollY += (m_speedScrollY * time.deltaTime());
            if (m_scrollY >= m_targetScrollY && m_scrollMode == ScrollNormal)
            {
                m_scrollY = m_targetScrollY;
                m_isScrolling = false;

                signals()->emitFinishedScroll();
            }

            // Makes sure that the primary image does not leave the window.
            if (m_scrollY >= renderTarget()->height())
            {
                m_scrollY = renderTarget()->height() - texture()->height();
            }
        }
        else if ((m_scrollDir & MoveNorth) && (m_scrollDir & MoveSouth) == 0)
        {
            m_scrollY -= (m_speedScrollY * time.deltaTime());
            if (m_scrollY <= m_targetScrollY && m_scrollMode == ScrollNormal)
            {
                m_scrollY = m_targetScrollX;
                m_isScrolling = false;

                signals()->emitFinishedScroll();
            }

            // Makes sure that the primary image does not leave the window.
            if (m_scrollY <= -texture()->height())
            {
                m_scrollY = y();
            }
        }

        updateUVs();
        requestUpdate();
    }
}


BackgroundEmitter* Background::signals()
{
    return &m_emitter;
}


bool Background::initializeData()
{
    TextureBase::initializeData();
    prepareTexture();

    return true;
}


TreeModelItem* Background::rootModelItem()
{
    return m_rootModelItem;
}


void Background::createProperties(TreeModel* model)
{
    TreeModelItem* tmiDire = new TreeModelItem("Direction", getMoveDirString(m_scrollDir));
    TreeModelItem* tmiMode = new TreeModelItem("Mode", getScrollModeString(m_scrollMode));
    TreeModelItem* tmiScro = new TreeModelItem("Is scrolling?", m_isScrolling);
    TreeModelItem* tmiView = new TreeModelItem("Viewport");
    TreeModelItem* tmiVieX = new TreeModelItem("x", m_view.x());
    TreeModelItem* tmiVieY = new TreeModelItem("y", m_view.y());
    TreeModelItem* tmiVieW = new TreeModelItem("w", m_view.width());
    TreeModelItem* tmiVieH = new TreeModelItem("h", m_view.height());
    TreeModelItem* tmiPosi = new TreeModelItem("Scroll position");
    TreeModelItem* tmiPosX = new TreeModelItem("x", m_scrollX);
    TreeModelItem* tmiPosY = new TreeModelItem("y", m_scrollY);

    m_rootModelItem = new TreeModelItem("Background");
    m_rootModelItem->appendChild(tmiDire);
    m_rootModelItem->appendChild(tmiMode);
    m_rootModelItem->appendChild(tmiScro);
    m_rootModelItem->appendChild(tmiView);
    m_rootModelItem->appendChild(tmiPosi);

    tmiView->appendChild(tmiVieX);
    tmiView->appendChild(tmiVieY);
    tmiView->appendChild(tmiVieW);
    tmiView->appendChild(tmiVieH);
    tmiPosi->appendChild(tmiPosX);
    tmiPosi->appendChild(tmiPosY);

    model->addItem(m_rootModelItem);

    TextureBase::createProperties(model);
}


void Background::updateProperties()
{
    m_rootModelItem->childAt(0)->setValue(getMoveDirString(m_scrollDir));
    m_rootModelItem->childAt(1)->setValue(getScrollModeString(m_scrollMode));
    m_rootModelItem->childAt(2)->setValue(m_isScrolling);
    m_rootModelItem->childAt(3)->childAt(0)->setValue(m_view.x());
    m_rootModelItem->childAt(3)->childAt(1)->setValue(m_view.y());
    m_rootModelItem->childAt(3)->childAt(2)->setValue(m_view.width());
    m_rootModelItem->childAt(3)->childAt(3)->setValue(m_view.height());
    m_rootModelItem->childAt(4)->childAt(0)->setValue(m_scrollX);
    m_rootModelItem->childAt(4)->childAt(1)->setValue(m_scrollY);

    TextureBase::updateProperties();
}


void Background::prepareTexture()
{
    texture()->bind();
    texture()->setWrapMode(QOpenGLTexture::Repeat);
    updateUVs();
}


void Background::updateUVs()
{
    // Gathers the view.
    float winX = (m_view.isNull()) ? 0.f : m_view.x();
    float winY = (m_view.isNull()) ? 0.f : m_view.y();
    float winW = (m_view.isNull()) ? renderTarget()->width()  : m_view.width();
    float winH = (m_view.isNull()) ? renderTarget()->height() : m_view.height();

    // Calculates the amount of repeated images.
    float repeatX = winW / texture()->width();
    float repeatY = winH / texture()->height();

    winW += winX;
    winH += winY;

    // Calculates the texture coordinates.
    float uvX = m_scrollX / texture()->width();
    float uvY = m_scrollY / texture()->height();
    float uvW = repeatX - uvX;
    float uvH = repeatY - uvY;

    // Modifies the vertices to fill the entire window.
    vertices().at(0).xyz(winX, winY, 0.f);
    vertices().at(1).xyz(winW, winY, 0.f);
    vertices().at(2).xyz(winW, winH, 0.f);
    vertices().at(3).xyz(winX, winH, 0.f);

    // Applies the coordinates.
    vertices().at(0).uv(-uvX, -uvY);
    vertices().at(1).uv(+uvW, -uvY);
    vertices().at(2).uv(+uvW, +uvH);
    vertices().at(3).uv(-uvX, +uvH);
}

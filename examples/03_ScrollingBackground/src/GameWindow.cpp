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


// Example headers
#include <GameWindow.hpp>

// Macroes
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)
#define WINDOW_H_SIZE QSizeF(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
#define WINDOW_H_POS  QPointF(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
#define CENTER_X(w)   (WINDOW_WIDTH / 2 - w / 2)
#define CENTER_Y(h)   (WINDOW_HEIGHT / 2 - h / 2)


GameWindow::GameWindow()
    : Window()
    , m_background({ 0, 0, 0, 0 })
    , m_linkText(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("03_ScrollingBackground");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


QRectF getView(int i)
{
    switch (i)
    {
        case 0: return QRectF(QPointF(), WINDOW_H_SIZE);
        case 1: return QRectF(QPointF(WINDOW_WIDTH / 2, 0), WINDOW_H_SIZE);
        case 2: return QRectF(QPointF(0, WINDOW_HEIGHT / 2), WINDOW_H_SIZE);
        case 3: return QRectF(WINDOW_H_POS, WINDOW_H_SIZE);
        default: return QRectF();
    }
}


QColor getColor(int i)
{
    switch (i)
    {
        case 0: return QColor(255, 200, 200);
        case 1: return QColor(255, 255, 200);
        case 2: return QColor(200, 255, 200);
        case 3: return QColor(200, 200, 255);
        default: return QColor();
    }
}


void GameWindow::onInit()
{
    // m_background
    for (int i = 0; i < 4; i++)
    {
        m_background[i] = new Background;
        m_background[i]->create(":/sakura.png", this);
        m_background[i]->setScrollDirection(MoveEast | MoveSouth);
        m_background[i]->setScrollMode(ScrollInfinite);
        m_background[i]->setScrollView(getView(i));
        m_background[i]->setBlendColor(getColor(i));
        m_background[i]->setBlendMode(BlendMultiply);
        m_background[i]->startScrolling();
    }

    // m_linkText
    m_linkText = new Text;
    m_linkText->create(this);
    m_linkText->setText("https://www.toptal.com/designers/subtlepatterns/sakura");
    m_linkText->setFont(QFont("", 20));
    m_linkText->setX(CENTER_X(m_linkText->width()));
    m_linkText->setY(WINDOW_HEIGHT - 24 - m_linkText->height());
}


void GameWindow::onExit()
{
    // m_background
    for (int i = 0; i < 4; i++)
    {
        if (m_background[i] != nullptr)
        {
            delete m_background[i];
            m_background[i] = nullptr;
        }
    }

    // m_linkText
    if (m_linkText != nullptr)
    {
        delete m_linkText;
        m_linkText = nullptr;
    }
}


void GameWindow::onCrash()
{
    onExit();
}


void GameWindow::onUpdate(const GameTime& time)
{
    for (int i = 0; i < 4; i++)
    {
        m_background[i]->update(time);
    }

    m_linkText->update(time);
}


void GameWindow::onRender()
{
    for (int i = 0; i < 4; i++)
    {
        m_background[i]->render();
    }

    m_linkText->render();
}

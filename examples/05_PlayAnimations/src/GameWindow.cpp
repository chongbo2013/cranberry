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
#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 540
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)
#define RATIO_X(x)    WINDOW_WIDTH / x
#define RATIO_Y(y)    WINDOW_HEIGHT / y


GameWindow::GameWindow()
    : Window()
    , m_gifAnim(nullptr)
    , m_cranAnim(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("05_PlayAnimations");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


void GameWindow::onInit()
{
    // m_gifAnim
    // Sets origin to (0, 0) in order to precisely position the scaled object!
    m_gifAnim = new GifAnimation;
    m_gifAnim->create(":/earth.gif", this);
    m_gifAnim->setPosition(QPointF(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 200));
    m_gifAnim->setScale(200 / m_gifAnim->width(), 200 / m_gifAnim->height());
    m_gifAnim->setOrigin(QPointF(0, 0));
    m_gifAnim->startAnimation(AnimateForever);

    // m_cranAnim
    m_cranAnim = new CranAnimation;
    m_cranAnim->create(":/pika.json", this);
    m_cranAnim->setScale(RATIO_X(m_cranAnim->width()), RATIO_Y(m_cranAnim->height()));
    m_cranAnim->startAnimation(AnimateForever);
}


void GameWindow::onExit()
{
    if (m_gifAnim != nullptr)
    {
        delete m_gifAnim;
        m_gifAnim = nullptr;
    }

    if (m_cranAnim != nullptr)
    {
        delete m_cranAnim;
        m_cranAnim = nullptr;
    }
}


void GameWindow::onCrash()
{
    onExit();
}


void GameWindow::onUpdate(const GameTime& time)
{
    m_cranAnim->update(time);
    m_gifAnim->update(time);
}


void GameWindow::onRender()
{
    m_cranAnim->render();
    m_gifAnim->render();
}

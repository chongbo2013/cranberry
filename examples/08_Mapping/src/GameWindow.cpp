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
#define WINDOW_WIDTH  608
#define WINDOW_HEIGHT 416
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)
#define CENTER_X(w)   (WINDOW_WIDTH / 2 - w / 2)
#define CENTER_Y(h)   (WINDOW_HEIGHT / 2 - h / 2)
#define CENTER(w, h)  (QVector2D(CENTER_X(w), CENTER_Y(h)))


GameWindow::GameWindow()
    : Window()
    , m_map(nullptr)
    , m_rect(nullptr)
    , m_sprite(nullptr)
    , m_text(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("08_Mapping");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


void GameWindow::onInit()
{
    // Builds the font for the text.
    QFont font;
    font.setPointSizeF(16.5);
    font.setStretch(QFont::SemiExpanded);

    // m_text
    m_text = new Text;
    m_text->create(this);
    m_text->setText("Controls:\n\nW,A,S,D - steer\nQ - quit");
    m_text->setFont(font);
    m_text->setTextColor(QColor(Qt::black));

    // m_map
    m_map = new GameMap;
    m_map->create(":/map.tmx", this);
    m_map->player()->setTileX(6);

    // Builds rectangle vertices.
    float rectw = m_text->width() + 16.0f;
    float recth = m_text->height() + 16.0f;

    m_rectPoints << QPointF(0.0f,  0.0f);
    m_rectPoints << QPointF(rectw, 0.0f);
    m_rectPoints << QPointF(rectw, recth);
    m_rectPoints << QPointF(0.0f,  recth);

    // m_rect
    m_rect = new Polygon;
    m_rect->create(m_rectPoints, this);
    m_rect->setPosition(WINDOW_WIDTH - rectw - 8.0f, 8.0f);
    m_rect->setColor(QColor(Qt::white));
    m_rect->setShapeFilled(true);
    m_rect->setOpacity(0.7f);
    m_text->setPosition(m_rect->x() + 8.0f, 16.0f);

    // m_sprite
    m_sprite = new Sprite;
    m_sprite->create(":/sprite.json", this);
    m_sprite->beginIdle("down");

    // The map's player should be the sprite. Also synchronises the movement
    // since the Sprite class moves the object, but the MapPlayer uses their
    // own transform properties for rendering.
    m_map->player()->setRenderObject(m_sprite);
    m_map->player()->synchroniseWith(m_sprite);
}


void GameWindow::onExit()
{
    delete m_map;
    delete m_rect;
    delete m_text;
    delete m_sprite;

    m_map = nullptr;
    m_rect = nullptr;
    m_text = nullptr;
    m_sprite = nullptr;
}


void GameWindow::onCrash()
{
    onExit();
}


void GameWindow::onKeyDown(const KeyboardState& keyboard)
{
    // Sprite movement (W,A,S,D)
    if (keyboard.isKeyDown(Qt::Key_S))
    {
        if (m_map->player()->movePlayerBy(0, 1))
        {
            m_sprite->beginMove("down");
        }
        else
        {
            m_sprite->beginIdle("down");
        }
    }
    else if (keyboard.isKeyDown(Qt::Key_W))
    {
        if (m_map->player()->movePlayerBy(0, -1))
        {
            m_sprite->beginMove("up");
        }
        else
        {
            m_sprite->beginIdle("up");
        }
    }
    else if (keyboard.isKeyDown(Qt::Key_A))
    {
        if (m_map->player()->movePlayerBy(-1, 0))
        {
            m_sprite->beginMove("left");
        }
        else
        {
            m_sprite->beginIdle("left");
        }
    }
    else if (keyboard.isKeyDown(Qt::Key_D))
    {
        if (m_map->player()->movePlayerBy(1, 0))
        {
            m_sprite->beginMove("right");
        }
        else
        {
            m_sprite->beginIdle("right");
        }
    }
    else if (keyboard.isKeyDown(Qt::Key_Q))
    {
        exitGame();
    }
}


void GameWindow::onUpdate(const GameTime& time)
{
    m_map->update(time);
    m_rect->update(time);
    m_text->update(time);
}


void GameWindow::onRender()
{
    m_map->render();
    m_rect->render();
    m_text->render();
}

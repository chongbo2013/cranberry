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
#define CENTER_X(w)   (WINDOW_WIDTH / 2 - w / 2)
#define CENTER_Y(h)   (WINDOW_HEIGHT / 2 - h / 2)
#define CENTER(w, h)  (QVector2D(CENTER_X(w), CENTER_Y(h)))


GameWindow::GameWindow()
    : Window()
    , m_batch(nullptr)
    , m_texture(nullptr)
    , m_linkText(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("06_PostProcessing");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


void GameWindow::onInit()
{
    // m_texture
    m_texture = new ITexture;
    m_texture->create(QImage(":/femalecodertocat.png"), this);
    m_texture->setName("m_texture");
    m_texture->setMoveSpeed(100.f, 100.f);
    m_texture->setPosition(CENTER(m_texture->width(), m_texture->height()));
    m_texture->setScale(0.5, 0.5);

    // m_linkText
    m_linkText = new Text;
    m_linkText->create(this);
    m_linkText->setText("https://octodex.github.com/femalecodertocat");
    m_linkText->setFont(QFont("", 24));
    m_linkText->setOutlineWidth(8);
    m_linkText->setX(CENTER_X(m_linkText->width()));
    m_linkText->setY(WINDOW_HEIGHT - 32 - m_linkText->height());

    // m_batch
    m_batch = new SpriteBatch;
    m_batch->create(this);
    m_batch->addObject(m_texture);
    m_batch->addObject(m_linkText);
    m_batch->setShaderProgram(OpenGLDefaultShaders::get("cb.glsl.film"));
}


void GameWindow::onExit()
{
    // m_texture
    if (m_texture != nullptr)
    {
        delete m_texture;
        m_texture = nullptr;
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
    m_batch->update(time);
}


void GameWindow::onRender()
{
    m_batch->render();
}

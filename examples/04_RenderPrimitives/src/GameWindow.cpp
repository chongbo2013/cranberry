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

// Cranberry headers
#include <Cranberry/System/Random.hpp>

// Macroes
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_SIZE   QSize(WINDOW_WIDTH, WINDOW_HEIGHT)
#define WINDOW_POS    QPointF(0.75 * WINDOW_WIDTH, 0.25 * WINDOW_HEIGHT)
#define CENTER_X(w)   (WINDOW_WIDTH / 2 - w / 2)
#define CENTER_Y(h)   (WINDOW_HEIGHT / 2 - h / 2)


GameWindow::GameWindow()
    : Window()
    , m_ellipse(nullptr)
    , m_hexagon(nullptr)
    , m_decagon(nullptr)
    , m_polygon(nullptr)
{
    WindowSettings settings;
    settings.setResizable(false);
    settings.setVerticalSync(true);
    settings.setDoubleBuffered(true);
    settings.setSize(WINDOW_SIZE);
    settings.setPosition(Qt::AlignCenter);
    settings.setTitle("04_RenderPrimitives");
    setSettings(settings);
}


GameWindow::~GameWindow()
{
}


QVector<QColor> colorGenerator(ShapeBase* e, uint f)
{

    QVector<QColor> v;
    Random random;
    random.setMinMax(0, 255);

    for (uint i = 0; i < e->vertexCount(); i += f)
    {
        uchar r = random.nextNumber();
        uchar g = random.nextNumber();
        uchar b = random.nextNumber();

        for (uint j = 0; j < f; j++)
        {
            v.append(QColor(r, g, b, 255));
        }
    }

    while ((uint) v.size() != e->vertexCount())
    {
        v.remove(v.length() - 1);
    }

    return v;
}


QVector<QPointF> pointsGenerator()
{
    QVector<QPointF> p;

    Random random, xRand, yRand;
    random.setMinMax(10, 20);
    xRand.setMinMax(WINDOW_WIDTH / 2, WINDOW_WIDTH);
    yRand.setMinMax(0, WINDOW_HEIGHT);

    int amount = random.nextNumber();
    for (int i = 0; i < amount; i++)
    {
        int x = xRand.nextNumber();
        int y = yRand.nextNumber();

        p.append(QPointF(x, y));
    }

    return p;
}


void GameWindow::onInit()
{
    // m_ellipse
    m_ellipse = new Ellipse;
    m_ellipse->create(150, 125, 0, 360, this);
    m_ellipse->setColor(colorGenerator(m_ellipse, 32));
    m_ellipse->setRotateMode(RotateForever);
    m_ellipse->setShapeFilled(true);

    // m_hexagon
    m_hexagon = new Polygon;
    m_hexagon->create(100, 6, this);
    m_hexagon->setColor(QColor(Qt::white));
    m_hexagon->setShapeFilled(true);
    m_hexagon->setPosition(QPointF(
                CENTER_X(m_hexagon->width()),
                CENTER_Y(m_hexagon->height())
                ));

    // m_decagon
    m_decagon = new Polygon;
    m_decagon->setLineWidth(4);
    m_decagon->create(150, 10, this);
    m_decagon->setColor(colorGenerator(m_decagon, 1));
    m_decagon->setPosition(QPointF(100, 300));

    // m_polygon
    m_polygon = new Polygon;
    m_polygon->setLineWidth(2);
    m_polygon->create(pointsGenerator(), this);
    m_polygon->setColor(QColor(Qt::red));
    m_polygon->setRotateMode(RotateForever);
    m_polygon->setOrigin(WINDOW_POS);

    // transformations
    m_ellipse->startRotating();
    m_hexagon->startScalingTo(0.25, 0.25);
    m_decagon->startMovingBy(200, 0);
    m_polygon->startRotating(false, false, false);

    // signals & slots
    QObject::connect(
            m_hexagon->signals(),
            &TransformBaseEmitter::stoppedScaling,
            this,
            &GameWindow::stoppedScaling
            );

    QObject::connect(
            m_hexagon->signals(),
            &TransformBaseEmitter::stoppedMoving,
            this,
            &GameWindow::stoppedMoving
            );
}


void GameWindow::onExit()
{
    // m_ellipse
    if (m_ellipse != nullptr)
    {
        delete m_ellipse;
        m_ellipse = nullptr;
    }

    // m_hexagon
    if (m_hexagon != nullptr)
    {
        delete m_hexagon;
        m_hexagon = nullptr;
    }

    // m_decagon
    if (m_decagon != nullptr)
    {
        delete m_decagon;
        m_decagon = nullptr;
    }

    // m_polygon
    if (m_polygon != nullptr)
    {
        delete m_polygon;
        m_polygon = nullptr;
    }
}


void GameWindow::onCrash()
{
    onExit();
}


void GameWindow::onUpdate(const GameTime& time)
{
    m_ellipse->update(time);
    m_hexagon->update(time);
    m_decagon->update(time);
    m_polygon->update(time);
}


void GameWindow::onRender()
{
    m_ellipse->render();
    m_decagon->render();
    m_hexagon->render();
    m_polygon->render();
}


void GameWindow::stoppedScaling()
{
    if (m_hexagon->scaleDirectionX() == ScaleDown)
    {
        m_hexagon->startScalingTo(1.0, 1.0);
    }
    else
    {
        m_hexagon->startScalingTo(0.25, 0.25);
    }
}


void GameWindow::stoppedMoving()
{
    if (m_decagon->moveDirection() & MoveEast)
    {
        m_decagon->startMovingBy(-200, 0);
    }
    else
    {
        m_decagon->startMovingBy(200, 0);
    }
}

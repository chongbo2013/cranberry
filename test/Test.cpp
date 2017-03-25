#include "Test.hpp"


Test::Test() : m_direction(0)
{
    qDebug() << "Numbers (0-9): Change blending mode.";

    m_currentColor = QColor(Qt::red);
    setVerticalSync(true);
    resize(800, 600);
}


void Test::init()
{
    m_bg.create(QImage(":/pattern.png"), this);
    m_bg.setScrollingBehaviour(cran::InfiniteScroll);
    m_bg.setScrollingDirection(cran::MoveSouth | cran::MoveEast);
    m_bg.setBlendColor(m_currentColor);
    m_bg.startScrolling();

    m_text.create(this);
    m_text.setTextColor(QColor(Qt::white));
    m_text.setOutlineColor(QColor(Qt::black));
    m_text.setOutlineWidth(10);
    m_text.setText(tr("Hello world!"));
    m_text.setFont(QFont("Arial", 20));
    m_text.setPosition(QVector2D(0, 0));
}


void Test::exit()
{
    m_bg.destroy();
}


void Test::keyUp(const cran::KeyReleaseEvent& ev)
{
    if (ev.key() == Qt::Key_0)
        m_bg.setBlendMode(cran::MultiplyBlend);
    else if (ev.key() == Qt::Key_1)
        m_bg.setBlendMode(cran::ScreenBlend);
    else if (ev.key() == Qt::Key_2)
        m_bg.setBlendMode(cran::OverlayBlend);
    else if (ev.key() == Qt::Key_3)
        m_bg.setBlendMode(cran::DivideBlend);
    else if (ev.key() == Qt::Key_4)
        m_bg.setBlendMode(cran::AddBlend);
    else if (ev.key() == Qt::Key_5)
        m_bg.setBlendMode(cran::SubtractBlend);
    else if (ev.key() == Qt::Key_6)
        m_bg.setBlendMode(cran::DiffBlend);
    else if (ev.key() == Qt::Key_7)
        m_bg.setBlendMode(cran::DarkenBlend);
    else if (ev.key() == Qt::Key_8)
        m_bg.setBlendMode(cran::LightenBlend);
}


void Test::update(const cran::GameTime& time)
{
    if (m_direction == TEST_FADE_RED)
    {
        m_currentColor.setRed(m_currentColor.red() - 1);
        m_currentColor.setGreen(m_currentColor.green() + 1);

        if (m_currentColor.red() == 0)
            m_direction = TEST_FADE_GREEN;
    }
    else if (m_direction == TEST_FADE_GREEN)
    {
        m_currentColor.setGreen(m_currentColor.green() - 1);
        m_currentColor.setBlue(m_currentColor.blue() + 1);

        if (m_currentColor.green() == 0)
            m_direction = TEST_FADE_BLUE;
    }
    else if (m_direction == TEST_FADE_BLUE)
    {
        m_currentColor.setBlue(m_currentColor.blue() - 1);
        m_currentColor.setRed(m_currentColor.red() + 1);

        if (m_currentColor.blue() == 0)
            m_direction = TEST_FADE_RED;
    }

    m_bg.setBlendColor(m_currentColor);
    m_bg.update(time);
    m_text.update(time);
}


void Test::render()
{
    m_bg.render();
    m_text.render();
}

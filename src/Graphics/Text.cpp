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
#include <Cranberry/Graphics/Text.hpp>

// Qt headers
#include <QPainter>


CRANBERRY_BEGIN_NAMESPACE


Text::Text()
    : Drawable()
    , Fadable()
    , Transformable()
    , m_text(QUuid::createUuid().toString())
    , m_textBrush(QBrush(Qt::white))
    , m_outlinePen(QPen(Qt::black))
    , m_outlineSize(0)
{
    m_outlinePen.setWidth(m_outlineSize);
}


const QString& Text::text() const
{
    return m_text;
}


const QFont& Text::font() const
{
    return m_font;
}


const QColor& Text::textColor() const
{
    return m_textBrush.color();
}


QColor Text::outlineColor() const
{
    return m_outlinePen.color();
}


int32_t Text::outlineWidth() const
{
    return m_outlineSize;
}


void Text::setText(const QString& str)
{
    m_text = str;
}


void Text::setFont(const QFont& font)
{
    m_font = font;
}

void Text::setTextColor(const QColor& color)
{
    m_textBrush.setColor(color);
}


void Text::setOutlineColor(const QColor& color)
{
    m_outlinePen.setColor(color);
}


void Text::setOutlineWidth(int32_t width)
{
    m_outlinePen.setWidth(width);
}


void Text::create(Window* target)
{
    createInternal(target, 0);
}


void Text::destroy()
{
    destroyInternal();
}


void Text::update(const GameTime& time)
{
    updateFade(time);
    updateTransform(time);
}


void Text::render()
{
    // Creates a painter path that contains the text.
    QPainterPath path;
    path.addText(QPointF(), m_font, m_text);

    // Creates a painter that draws onto the current render target.
    QPainter painter(renderTarget());
    painter.setRenderHints(QPainter::HighQualityAntialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::Antialiasing);

    // Transformations
    painter.translate(x(), y());
    painter.scale(scale(), scale());
    painter.rotate(angle());
    painter.setOpacity(opacity());

    // Appearance
    painter.setFont(m_font);
    painter.setPen(QColor(Qt::red));
    painter.setBrush(m_textBrush);
    painter.drawPath(path);
}


CRANBERRY_END_NAMESPACE

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
#include <QUuid>


CRANBERRY_BEGIN_NAMESPACE


Text::Text()
    : Drawable()
    , Fadable()
    , Transformable()
    , m_text(QUuid::createUuid().toString())
    , m_textPen(QPen(Qt::white))
    , m_outlineBrush(QBrush(Qt::black))
    , m_outlineSize(0)
{
    QTextOption op;
    op.setAlignment(Qt::AlignLeft | Qt::AlignTop | Qt::AlignBaseline);
    op.setWrapMode(QTextOption::NoWrap);

    m_staticText.setText(m_text);
    m_staticText.setTextOption(op);
    m_staticText.setTextFormat(Qt::RichText);
    m_staticText.setPerformanceHint(QStaticText::AggressiveCaching);
}


const QString& Text::text() const
{
    return m_text;
}


const QFont& Text::font() const
{
    return m_font;
}


QColor Text::textColor() const
{
    return m_textPen.color();
}


QColor Text::outlineColor() const
{
    return m_outlineBrush.color();
}


int32_t Text::outlineWidth() const
{
    return m_outlineSize;
}


void Text::setText(const QString& str)
{
    m_text = str;
    m_staticText.setText(str);
    prepareText();
}


void Text::setFont(const QFont& font)
{
    m_font = font;
    prepareText();
}

void Text::setTextColor(const QColor& color)
{
    m_textPen.setColor(color);
}


void Text::setTextOptions(const QTextOption& option)
{
    m_staticText.setTextOption(option);
}


void Text::setOutlineColor(const QColor& color)
{
    m_outlineBrush.setColor(color);
}


void Text::setOutlineWidth(int32_t width)
{
    m_outlineSize = width;
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
    // Creates a painter that draws onto the current render target.
    QPainter painter(renderTarget());
    painter.setRenderHints(QPainter::HighQualityAntialiasing |
                           QPainter::SmoothPixmapTransform |
                           QPainter::TextAntialiasing |
                           QPainter::Antialiasing);

    // Transformations
    QTransform transform;
    transform.translate(x(), y());
    transform.scale(scale(), scale());
    transform.rotate(angle());
    painter.setTransform(transform);

    // Appearance
    painter.setOpacity(opacity());
    painter.setFont(m_font);
    painter.setPen(m_textPen);

    // Text
    if (m_outlineSize > 0)
        painter.fillPath(m_outlinePath, m_outlineBrush);

    painter.drawStaticText(0, 0, m_staticText);
}


void Text::prepareText()
{
    // Prepares the static text transform.
    QTransform transform;
    transform.translate(x(), y());
    transform.scale(scale(), scale());
    transform.rotate(angle());
    m_staticText.prepare(transform, m_font);

    // Prepares the outline, if any.
    if (m_outlineSize > 0)
    {
        QFontMetrics fm(m_font, renderTarget());
        QPainterPath textPath;
        QPainterPathStroker stroker;
        QRect tight = fm.tightBoundingRect(m_text);

        // Stroker
        stroker.setJoinStyle(Qt::RoundJoin);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setWidth(m_outlineSize);

        // Path
        textPath.addText(x(), y() + tight.height() + 1, m_font, m_text);
        m_outlinePath = stroker.createStroke(textPath);
    }
}


CRANBERRY_END_NAMESPACE

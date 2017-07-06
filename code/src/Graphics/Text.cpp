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


// Cranberry headers
#include <Cranberry/Graphics/Text.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QOpenGLTexture>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Cannot render invalid object.")


Text::Text()
    : IRenderable()
    , ITransformable()
    , m_text("{empty}")
    , m_textPen(new QPen(Qt::white))
    , m_outlineBrush(new QBrush(Qt::black))
    , m_texture(new ITexture)
    , m_outlineWidth(0)
    , m_textUpdate(true)
{
    m_options.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_options.setWrapMode(QTextOption::NoWrap);
}


Text::~Text()
{
    delete m_textPen;
    delete m_outlineBrush;
    delete m_texture;
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
    return m_textPen->color();
}


QColor Text::outlineColor() const
{
    return m_outlineBrush->color();
}


int Text::outlineWidth() const
{
    return m_outlineWidth;
}


void Text::setText(const QString& str)
{
    m_text = str;
    m_textUpdate = true;

    recalculateSize();
}


void Text::setFont(const QFont& font)
{
    m_font = font;
    m_textUpdate = true;

    recalculateSize();
}


void Text::setTextColor(const QColor& color)
{
    m_textPen->setColor(color);
    m_textUpdate = true;
}


void Text::setTextOptions(const QTextOption& option)
{
    m_options = option;
    m_textUpdate = true;
}


void Text::setOutlineColor(const QColor& color)
{
    m_outlineBrush->setColor(color);
    m_textUpdate = true;
}


void Text::setOutlineWidth(int width)
{
    m_outlineWidth = width;
    m_textUpdate = true;

    recalculateSize();
}


bool Text::create(Window* rt)
{
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return IRenderable::create(rt);
}


void Text::destroy()
{
    if (!m_texture->isNull()) m_texture->destroy();

    IRenderable::destroy();
}


void Text::update(const GameTime& time)
{
    updateTransform(time);
}


void Text::render()
{
    if (Q_UNLIKELY(isNull()))
    {
        cranError(ERRARG(e_01));
        return;
    }

    if (m_textUpdate)
    {
        createTexture();
        m_textUpdate = false;
    }

    // Copies all transformations.
    m_texture->setShaderProgram(shaderProgram());
    m_texture->setPosition(pos());
    m_texture->setAngle(angle());
    m_texture->setOpacity(opacity());
    m_texture->setScale(scaleX(), scaleY());
    m_texture->render();
}


void Text::createTexture()
{
    if (!m_texture->isNull()) m_texture->destroy();

    // Find perfect size for the image.
    QFontMetrics fm(m_font);
    QPoint pt(m_outlineWidth / 2, m_outlineWidth / 2);
    QSize sz = fm.boundingRect(m_text).size();
    sz.setWidth(sz.width() + m_outlineWidth);
    sz.setHeight(sz.height() + m_outlineWidth);

    if ((sz.width() % 2) != 0) sz.rwidth() += 1;
    if ((sz.height() % 2) != 0) sz.rheight() += 1;

    // Base image
    QImage img = QImage(sz, QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    // Rendering hints
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setRenderHints(
            QPainter::HighQualityAntialiasing |
            QPainter::SmoothPixmapTransform   |
            QPainter::TextAntialiasing        |
            QPainter::Antialiasing
            );

    // Outline test
    if (m_outlineWidth > 0)
    {
        QPainterPathStroker stroker;
        stroker.setJoinStyle(Qt::RoundJoin);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setWidth(m_outlineWidth);

        QPainterPath ppath;
        ppath.addText(pt.x(), pt.y() + fm.ascent(), m_font, m_text);
        painter.setBrush(*m_outlineBrush);
        painter.setPen(Qt::NoPen);
        painter.drawPath(stroker.createStroke(ppath));
    }

    // Appearance
    painter.setFont(m_font);
    painter.setPen(*m_textPen);

    // Text
    painter.drawText(QRect(pt, sz), m_text, m_options);
    painter.end();

    m_texture->create(img, renderTarget());
    m_texture->texture()->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_texture->texture()->setWrapMode(QOpenGLTexture::ClampToEdge);

    setSize(m_texture->width(), m_texture->height());
}


void Text::recalculateSize()
{
    QFontMetrics fm(m_font);
    QSize sz = fm.boundingRect(m_text).size();
    sz.setWidth(sz.width() + m_outlineWidth);
    setSize(sz.width(), sz.height());
}

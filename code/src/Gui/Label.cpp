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
#include <Cranberry/Graphics/Base/ITexture.hpp>
#include <Cranberry/Gui/Label.hpp>

// Qt headers

// Standard headers


CRANBERRY_USING_NAMESPACE


Label::Label(Widget* parent)
    : Widget(parent)
    , m_align(Qt::AlignTop | Qt::AlignLeft)
    , m_text(new Text)
    , m_texture(new ITexture)
{
    addObjectToBatch(m_text);
}


Label::~Label()
{
    destroy();
}


bool Label::isNull() const
{
    return Widget::isNull() ||
           m_text->isNull() ||
           m_texture->isNull();
}


bool Label::create(Window* renderTarget)
{
    if (!Widget::create(renderTarget)) return false;
    return m_text->create(renderTarget);
}


void Label::destroy()
{
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }

    if (m_texture != nullptr)
    {
        delete m_texture;
        m_texture = nullptr;
    }

    Widget::destroy();
}


const QString& Label::text() const
{
    return m_text->text();
}


const QImage& Label::image() const
{
    return m_image;
}


Qt::Alignment Label::textAlignment() const
{
    return m_align;
}


void Label::setText(const QString& text)
{
    m_text->setText(text);
}


void Label::setImage(const QImage& img)
{
    if (img.isNull()) return;

    if (!m_texture->isNull())
    {
        m_texture->destroy();
    }

    m_texture->create(img, renderTarget());
    m_image = img;

    addObjectToBatch(m_texture);
}


void Label::setTextAlignment(Qt::Alignment align)
{
    m_align = align;

    recalculatePositions();
}


void Label::onSizeChanged(const QSize&)
{
    recalculatePositions();
}


void Label::recalculatePositions()
{
    qreal x = 0, y = 0;
    QRectF r = ITransformable::rect();
    QRectF t = m_text->rect();

    if ((m_align & Qt::AlignRight) != 0)
    {
        x = r.width() - t.width();
    }
    if ((m_align & Qt::AlignBottom) != 0)
    {
        y = r.height() - t.height();
    }
    if ((m_align & Qt::AlignHCenter) != 0)
    {
        x = r.width() / 2 - t.width() / 2;
    }
    if ((m_align & Qt::AlignVCenter) != 0)
    {
        y = r.height() / 2 - t.height() / 2;
    }

    m_text->setPosition(QVector2D(x, y));
}

////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTextDocumentFragment>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Cannot render invalid object.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Texture could not be created.")
CRANBERRY_CONST_VAR(qint32, c_maxSize, 4096)


Text::Text()
    : RenderBase()
    , m_textPen(new QPen(Qt::white))
    , m_outlineBrush(new QBrush(Qt::black))
    , m_texture(new TextureBase)
    , m_outlineWidth(0)
    , m_columnLimit(-1)
    , m_rowLimit(-1)
    , m_lastWidth(0)
    , m_lastHeight(0)
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

    recalcSize();
}


void Text::setFont(const QFont& font)
{
    m_font = font;
    m_textUpdate = true;

    recalcSize();
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

    recalcSize();
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

    recalcSize();
}


bool Text::create(Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    return true;
}


void Text::destroy()
{
    if (!m_texture->isNull()) m_texture->destroy();

    RenderBase::destroy();
}


void Text::update(const GameTime& time)
{
    updateTransform(time);

    // Copies all transformations.
    m_texture->setShaderProgram(shaderProgram());
    m_texture->copyTransform(this, m_texture);
}


void Text::render()
{
    if (!RenderBase::prepareRendering())
    {
        return;
    }
    else if (m_textUpdate)
    {
        updateTexture();
        m_textUpdate = false;
    }

    m_texture->render();
}


void Text::updateTexture()
{
    if (m_texture->isNull())
    {
        createTexture();
    }

    auto size = measureText();
    if (size.width() > m_lastWidth || size.height() > m_lastHeight)
    {
        resizeTexture(size);
    }

    renderToTexture();
}


void Text::createTexture()
{
    QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    if (!texture->create())
    {
        cranError(ERRARG(e_02));
        delete texture;
        return;
    }

    // Smoothen the texture and avoid pixel fragments.
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    // Creates a new ITexture on base of the QOpenGLTexture.
    m_texture->create(texture, renderTarget());
}


void Text::resizeTexture(QSizeF base)
{
    // Finds a suitable size for the texture.
    float w, h;
    if (m_columnLimit <= 0 || m_rowLimit <= 0)
    {
        w = base.width();
        h = base.height();
    }
    else
    {
        auto s = findPerfectSize();
        w = s.width();
        h = s.height();
    }

    m_texture->texture()->bind();

    // Allocates storage for the texture, without initial pixel data.
    glDebug(gl->glTexImage2D(
                GL_TEXTURE_2D,  GL_ZERO, GL_RGBA8,
                int(w), int(h), GL_ZERO, GL_RGBA,
                GL_UNSIGNED_BYTE, nullptr
                ));

    m_texture->texture()->release();
    m_texture->setSize(w, h);
    m_texture->setOrigin(w / 2.0, h / 2.0);
    m_texture->setSourceRectangle(0.0, 0.0, w, h);
    m_lastWidth = w;
    m_lastHeight = h;
}


void Text::renderToTexture()
{
    QFontMetrics fm(m_font);
    QPoint pt(m_outlineWidth / 2, m_outlineWidth / 2);

    // Base image
    QImage img = QImage(width(), height(), QImage::Format_RGBA8888);
    img.fill(Qt::transparent);

    // Rendering hints
    QPainter painter(&img);
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
        ppath.addText(pt.x(), pt.y() + fm.ascent() - 0.5, m_font, m_text);
        painter.setBrush(*m_outlineBrush);
        painter.setPen(Qt::NoPen);
        painter.drawPath(stroker.createStroke(ppath));
    }

    // Appearance
    painter.setFont(m_font);
    painter.setPen(*m_textPen);

    // Text
    painter.drawText(QRect(pt, img.size()), m_text, m_options);
    painter.end();

    m_texture->texture()->bind();

    // Renders all pixels into the texture.
    glDebug(gl->glTexSubImage2D(
                GL_TEXTURE_2D, GL_ZERO,
                0, 0, img.width(), img.height(),
                GL_RGBA, GL_UNSIGNED_BYTE, img.constBits()
                ));

    m_texture->texture()->release();
}


void Text::recalcSize()
{
    QSizeF s = measureText();
    setSize(s.width(), s.height());
}


QSizeF Text::findPerfectSize()
{
    QFontMetrics fm(m_font);
    QSize sz = fm.boundingRect('x').size();

    sz.setWidth(sz.width() * m_columnLimit + m_outlineWidth);
    sz.setHeight(sz.height() * m_rowLimit + m_outlineWidth);

    // Align to power of 2.
    if ((sz.width() % 2) != 0) sz.rwidth() += 1;
    if ((sz.height() % 2) != 0) sz.rheight() += 1;

    return sz;
}


QString htmlToPlain(const QString& str)
{
    return QTextDocumentFragment::fromHtml(str).toPlainText();
}


QSizeF Text::measureText()
{
    QFontMetrics fm(m_font);
    QSize sz = fm.boundingRect(m_text).size();

    sz.setWidth(sz.width() + m_outlineWidth);
    sz.setHeight(sz.height() + m_outlineWidth);

    // Align to power of 2.
    if ((sz.width() % 2) != 0) sz.rwidth() += 1;
    if ((sz.height() % 2) != 0) sz.rheight() += 1;

    return sz;
}


TreeModelItem* Text::rootModelItem()
{
    return m_rootModelItem;
}


void Text::createProperties(TreeModel* model)
{
    TreeModelItem* tmiText = new TreeModelItem("Text", htmlToPlain(m_text));
    TreeModelItem* tmiFont = new TreeModelItem("Font", m_font.toString());
    TreeModelItem* tmiTCol = new TreeModelItem("Text color", m_textPen->color());
    TreeModelItem* tmiOCol = new TreeModelItem("Outline color", m_outlineBrush->color());
    TreeModelItem* tmiOWid = new TreeModelItem("Outline width", m_outlineWidth);
    TreeModelItem* tmiUpda = new TreeModelItem("Requires update?", m_textUpdate);

    m_rootModelItem = new TreeModelItem("Text");
    m_rootModelItem->appendChild(tmiText);
    m_rootModelItem->appendChild(tmiFont);
    m_rootModelItem->appendChild(tmiTCol);
    m_rootModelItem->appendChild(tmiOCol);
    m_rootModelItem->appendChild(tmiOWid);
    m_rootModelItem->appendChild(tmiUpda);

    model->addItem(m_rootModelItem);

    RenderBase::createProperties(model);
}


void Text::updateProperties()
{
    m_rootModelItem->childAt(0)->setValue(htmlToPlain(m_text));
    m_rootModelItem->childAt(1)->setValue(m_font.toString());
    m_rootModelItem->childAt(2)->setValue(m_textPen->color());
    m_rootModelItem->childAt(3)->setValue(m_outlineBrush->color());
    m_rootModelItem->childAt(4)->setValue(m_outlineWidth);
    m_rootModelItem->childAt(5)->setValue(m_textUpdate);

    RenderBase::updateProperties();
}

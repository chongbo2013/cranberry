﻿////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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


#pragma once
#ifndef CRANBERRY_TEXT_HPP
#define CRANBERRY_TEXT_HPP


// Cranberry headers
#include <Cranberry/Graphics/SpriteBatch.hpp>

// Qt headers
#include <QFont>
#include <QOpenGLPaintDevice>
#include <QTextOption>

// Forward declarations
CRANBERRY_FORWARD_Q(QPen)
CRANBERRY_FORWARD_Q(QBrush)
CRANBERRY_FORWARD_Q(QStaticText)
CRANBERRY_FORWARD_Q(QPainterPath)
CRANBERRY_FORWARD_Q(QPainter)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class is capable of rendering text. If you have a static text, calling
/// create() and not doing anything else is fine. But in case you need dynamic
/// text that changes very often, invoke setColumnLimit() and setRowLimit().
/// Cranberry will calculate the optimal texture size from these two parameters,
/// but is still able to resize the texture in case text happens to be bigger.
///
/// \class Text
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Text final : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(Text)
    CRANBERRY_DECLARE_DTOR(Text)
    CRANBERRY_DISABLE_COPY(Text)
    CRANBERRY_DISABLE_MOVE(Text)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the currently displayed text.
    ///
    /// \returns the current text.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& text() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the currently used font.
    ///
    /// \returns the current font.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QFont& font() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current text color.
    ///
    /// \returns the text color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QColor textColor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current outline color.
    ///
    /// \returns the outline color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QColor outlineColor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current outline width.
    ///
    /// \returns the outline width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int outlineWidth() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the blur factor of the text.
    ///
    /// \returns the blur factor.
    ///
    ////////////////////////////////////////////////////////////////////////////
    float blurFactor() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the maximum amount of characters per line.
    ///
    /// \returns the column limit.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int columnLimit() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the maximum amount of lines.
    ///
    /// \returns the row limit.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int rowLimit() const;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the string to be displayed.
    ///
    /// \param str String to display.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setText(const QString& str);

    ////////////////////////////////////////////////////////////////////////////
    /// If the constraint is a null QRect, no constraint will be applied.
    /// If otherwise it is not null, the text will be wrapped accordingly.
    ///
    /// \param constraint Constraint to apply.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setConstraint(const QRect& constraint);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the font to render text with.
    ///
    /// \param font Font used for rendering.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setFont(const QFont& font);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color of the text.
    ///
    /// \param color New text color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTextColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the text options such as wrap or text alignment.
    ///
    /// \param option Text options to apply.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTextOptions(const QTextOption& option);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the outline color.
    ///
    /// \param color New outline color.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOutlineColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the width of the outline.
    ///
    /// \param width New outline width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setOutlineWidth(int width);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the blur factor of the text. As Qt somehow does not smoothen
    /// the text properly, the default text shader applies some blur afterwards.
    /// The value must be in range 0 to 1, while a higher number leads to more
    /// blur. It is recommended to use a small value for small texts and a big
    /// value for big texts.
    ///
    /// \param factor Blur factor in range 0 to 1.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setBlurFactor(float factor);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the maximum amount of characters per line. A negative value
    /// indicates that the size of the texture is adjusted to the size of the
    /// text.
    ///
    /// \param limit Character limit per line.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setColumnLimit(int limit);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the maximum amount of lines for that object. A negative value
    /// indicates that the size of the texture is adjusted to the size of the
    /// text.
    ///
    /// \param limit Line limit.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRowLimit(int limit);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the OpenGL resources for this object.
    ///
    /// \param renderTarget Target to render text on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(Window* renderTarget = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the OpenGL resources for this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the transform of this object.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the root model item of this instance.
    ///
    /// \returns the root model item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem* rootModelItem() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void updateProperties() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    void updateTexture();
    void updateShader();
    void resizeTexture(QSizeF);
    void renderToTexture();
    void prepareConstraint();
    void recalcSize();
    auto approximateSize() -> QSizeF;
    auto measureText() -> QSizeF;

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem*            m_rootModelItem;
    QString                   m_text;
    QRect                     m_constraint;
    QFont                     m_font;
    QPen*                     m_textPen;
    QBrush*                   m_outlineBrush;
    QTextOption               m_options;
    QOpenGLPaintDevice*       m_paintDevice;
    QOpenGLFramebufferObject* m_fbo;
    SpriteBatch*              m_batch;
    int                       m_outlineWidth;
    int                       m_columnLimit;
    int                       m_rowLimit;
    float                     m_lastWidth;
    float                     m_lastHeight;
    float                     m_blurFactor;
    bool                      m_textUpdate;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Text
/// \ingroup Graphics
///
/// This class renders text, optionally with an outline. Supports all
/// all transformations that ITransformable provides.
///
/// \code
/// Text text;
/// text.setText("Hello world!");
/// text.setFont(QFont(...));
/// text.setOutlineWidth(4);
/// text.setAngle(20);
/// text.create(this);
///
/// ...
///
/// text.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

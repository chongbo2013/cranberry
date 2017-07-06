﻿////////////////////////////////////////////////////////////////////////////////
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


#pragma once
#ifndef CRANBERRY_TEXT_HPP
#define CRANBERRY_TEXT_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/ITexture.hpp>

// Qt headers
#include <QFont>
#include <QTextOption>


// Forward declarations
CRANBERRY_FORWARD_Q(QPen)
CRANBERRY_FORWARD_Q(QBrush)
CRANBERRY_FORWARD_Q(QStaticText)
CRANBERRY_FORWARD_Q(QPainterPath)
CRANBERRY_FORWARD_Q(QPainter)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class is capable of rendering text.
///
/// \class Text
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Text
    : public IRenderable
    , public ITransformable
{
public:

    CRANBERRY_DISABLE_COPY(Text)
    CRANBERRY_DISABLE_MOVE(Text)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new Text instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Text();

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all resources used by this Text instance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~Text();


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
    /// Specifies the string to be displayed.
    ///
    /// \param str String to display.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setText(const QString& str);

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
    /// Creates the OpenGL resources for this object.
    ///
    /// \param renderTarget Target to render text on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(Window* renderTarget);

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


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void createTexture();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QString       m_text;
    QFont         m_font;
    QPen*         m_textPen;
    QBrush*       m_outlineBrush;
    QTextOption   m_options;
    ITexture*     m_texture;
    int           m_outlineWidth;
    bool          m_textUpdate;
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

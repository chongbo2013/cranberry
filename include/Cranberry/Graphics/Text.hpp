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


#pragma once
#ifndef CRANBERRY_TEXT_HPP
#define CRANBERRY_TEXT_HPP


// Cranberry headers
#include <Cranberry/Graphics/System/Drawable.hpp>
#include <Cranberry/Graphics/System/Fadable.hpp>
#include <Cranberry/Graphics/System/Transformable.hpp>

// Qt headers
#include <QFont>
#include <QPen>
#include <QStaticText>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a renderable text.
///
/// \class Text
/// \author Nicolas Kogler
/// \date March 22, 2017
///
class CRANBERRY_EXPORT Text
        : public Drawable
        , public Fadable
        , public Transformable
{
public:

    CRANBERRY_DISABLE_MOVE(Text)
    CRANBERRY_DEFAULT_COPY(Text)
    CRANBERRY_DEFAULT_DTOR(Text)

    ///
    /// Initializes a new instance of Text.
    ///
    Text();


    ///
    /// Retrieves the currently displayed text.
    ///
    /// \returns the current text.
    ///
    const QString& text() const;

    ///
    /// Retrieves the currently used font.
    ///
    /// \returns the current font.
    ///
    const QFont& font() const;

    ///
    /// Retrieves the text color.
    ///
    /// \returns the text color.
    ///
    QColor textColor() const;

    ///
    /// Retrieves the outline color.
    ///
    /// \returns the outline color.
    ///
    QColor outlineColor() const;

    ///
    /// Retrieves the outline width.
    ///
    /// \returns the outline width.
    ///
    int32_t outlineWidth() const;


    ///
    /// Specifies the string to be displayed.
    ///
    /// \param str String to display.
    ///
    void setText(const QString& str);

    ///
    /// Specifies the font to render the text with.
    ///
    /// \param font Font used for rendering.
    ///
    void setFont(const QFont& font);

    ///
    /// Specifies the color of the text.
    ///
    /// \param color New text color.
    ///
    void setTextColor(const QColor& color);

    ///
    /// Specifies the text options such as wrap
    /// or text alignment.
    ///
    /// \param option Text options to set.
    ///
    void setTextOptions(const QTextOption& option);

    ///
    /// Specifies the color of the outline.
    ///
    /// \param color New outline color.
    ///
    void setOutlineColor(const QColor& color);

    ///
    /// Specifies the width of the outline.
    /// By default, this value is zero.
    ///
    /// \param width New outline width.
    ///
    void setOutlineWidth(int32_t width);


    ///
    /// Creates the OpenGL resources for this object.
    ///
    /// \param renderTarget The target to render text on.
    ///
    void create(Window* renderTarget);

    ///
    /// Destroys the OpenGL resources for this object.
    ///
    void destroy() override;

    ///
    /// Updates the transform and opacity of this object.
    ///
    /// \param time Contains the delta time.
    ///
    void update(const GameTime& time) override;

    ///
    /// Renders this object.
    ///
    void render() override;


private:

    // Members
    QString     m_text;
    QFont       m_font;
    QPen        m_textPen;
    QPen        m_outlinePen;
    int32_t     m_outlineSize;
    int32_t     m_textHeight;
    QStaticText m_staticText;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_TEXT_HPP

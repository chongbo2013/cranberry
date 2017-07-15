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
#ifndef CRANBERRY_LABEL_HPP
#define CRANBERRY_LABEL_HPP


// Cranberry headers
#include <Cranberry/Gui/Base/Widget.hpp>

// Qt headers

// Standard headers


// Forward declarations
CRANBERRY_FORWARD_C(Text)
CRANBERRY_FORWARD_C(ITexture)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a label with a text and an optional image.
///
/// \class Label
/// \author Nicolas Kogler
/// \date July 12, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GUI_EXPORT Label : public Widget
{
public:

    CRANBERRY_DISABLE_COPY(Label)
    CRANBERRY_DISABLE_MOVE(Label)


    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of Label.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Label(Widget* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Last resort to destroy remaining OpenGL resources.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~Label();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this label is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the text and the texture object.
    ///
    /// \param renderTarget Target to render label on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(Window* renderTarget) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the text and the texture object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current label text.
    ///
    /// \returns the label text.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& text() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the current image of the label.
    ///
    /// \returns the label image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QImage& image() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the alignment of the label text.
    ///
    /// \returns the alignment of the text.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Qt::Alignment textAlignment() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the label text.
    ///
    /// \param text New label text.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setText(const QString& text);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the label image.
    ///
    /// \param img New label image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setImage(const QImage& img);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the alignment of the label text.
    ///
    /// \param align New text alignment.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTextAlignment(Qt::Alignment align);


protected:

    ////////////////////////////////////////////////////////////////////////////
    /// Recalculates some positions if a size change occured.
    ///
    /// \param newSize New size of the widget.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void onSizeChanged(const QSize& newSize) override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void recalculatePositions();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Qt::Alignment m_align;
    QImage        m_image;
    Text*         m_text;
    ITexture*     m_texture;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Label
/// \ingroup Gui
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif
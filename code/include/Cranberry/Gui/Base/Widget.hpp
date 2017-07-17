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
#ifndef CRANBERRY_WIDGET_HPP
#define CRANBERRY_WIDGET_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/IRenderable.hpp>
#include <Cranberry/Graphics/Base/ITransformable.hpp>

// Qt headers
#include <QOpenGLWidget>


// Forward declarations
CRANBERRY_FORWARD_C(SpriteBatch)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines the base widget for all \ref Gui widgets.
///
/// \class Widget
/// \author Nicolas Kogler
/// \date July 12, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GUI_EXPORT Widget
    : public IRenderable
    , public ITransformable
{
public:

    CRANBERRY_DISABLE_COPY(Widget)
    CRANBERRY_DISABLE_MOVE(Widget)


    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of Widget.
    ///
    /// \param parent The parent widget.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Widget(Widget* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Last resort to free any remaining OpenGL resources.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~Widget();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this widget is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the sprite batch for this widget.
    ///
    /// \param renderTarget Target to render widget on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool create(Window* renderTarget = nullptr) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the sprite batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the widget and all its transformations.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the wigdet's sprite batch.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;


protected:

    virtual void onSizeChanged(const QSize&) { }
    bool addObjectToBatch(IRenderable* obj);
    bool insertObjectToBatch(int layer, IRenderable* obj);
    bool removeObjectFromBatch(IRenderable* obj);


private slots:

    void posChanged();
    void sizeChanged();


private:


    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QOpenGLWidget* m_widget;
    SpriteBatch*   m_batch;

    Q_OBJECT
};


////////////////////////////////////////////////////////////////////////////////
/// \class Widget
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

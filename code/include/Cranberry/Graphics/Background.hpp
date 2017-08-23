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


#pragma once
#ifndef CRANBERRY_BACKGROUND_HPP
#define CRANBERRY_BACKGROUND_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/TextureBase.hpp>
#include <Cranberry/System/Emitters/BackgroundEmitter.hpp>

// Qt headers
#include <QRectF>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a background that is scrollable. It is recommended to use images
/// of which multiple instances can be put nicely together (repetitive).
///
/// \class Background
/// \author Nicolas Kogler
/// \date June 24, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT Background final : public TextureBase
{
public:

    CRANBERRY_DECLARE_CTOR(Background)
    CRANBERRY_DISABLE_COPY(Background)
    CRANBERRY_DISABLE_MOVE(Background)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this background is scrolling.
    ///
    /// \returns true if background is scrolling.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isScrolling() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the view in which the scrolling background should be shown.
    /// By passing a null rectangle ["QRectF()"] the entire window will be
    /// taken as view. By default, this is the entire window.
    ///
    /// \param view View in which to show background.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollView(const QRectF& view);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scroll position.
    ///
    /// \param pos Scroll position in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollPosition(const QPointF& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scroll speed in both directions.
    ///
    /// \param speedX Speed in X-direction.
    /// \param speedY Speed in Y-direction.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollSpeed(float speedX, float speedY);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scroll mode (none, normal or infinite).
    ///
    /// \param mode New scroll mode.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollMode(ScrollMode mode);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the directions in which the scroll should proceed.
    ///
    /// \param dir Directions to move background in.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollDirection(MoveDirections dir);

    ////////////////////////////////////////////////////////////////////////////
    /// Begins scrolling the background, but only if the scroll mode is
    /// ScrollInfinite.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void beginScroll();

    ////////////////////////////////////////////////////////////////////////////
    /// Starts scrolling the background.
    ///
    /// \param advance QPointF containing X-advance and Y-advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void scrollBy(const QPointF& advance);

    ////////////////////////////////////////////////////////////////////////////
    /// Scrolls the background to the given scroll \p pos.
    ///
    /// \param pos Position to scroll to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void scrollTo(const QPointF& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Terminates the scroll movement manually.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void endScroll();


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Implements custom scrolling behaviour.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the signals for this object.
    ///
    /// \returns the signals.
    ///
    ////////////////////////////////////////////////////////////////////////////
    BackgroundEmitter* signals() override;

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

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes the background vertex data.
    ///
    /// \returns true if successful.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool initializeData() override;


protected:

    ////////////////////////////////////////////////////////////////////////////
    // Protected functions
    ////////////////////////////////////////////////////////////////////////////
    void prepareTexture();
    void updateUVs();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    float             m_speedScrollX;  ///< Horizontal scroll speed
    float             m_speedScrollY;  ///< Vertical scroll speed
    float             m_scrollX;       ///< Current scroll pos X
    float             m_scrollY;       ///< Current scroll pos Y
    float             m_targetScrollX; ///< Target scroll pos X
    float             m_targetScrollY; ///< Target scroll pos Y
    bool              m_isScrolling;   ///< Is currently scrolling?
    QRectF            m_view;          ///< Current view
    ScrollMode        m_scrollMode;    ///< Normal or Infinite?
    MoveDirections    m_scrollDir;     ///< Directions to move background in
    TreeModelItem*    m_rootModelItem; ///< Root item for the debug overlay
    BackgroundEmitter m_emitter;       ///< Holds the signals for this object
};


////////////////////////////////////////////////////////////////////////////////
/// \class Background
/// \ingroup Graphics
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

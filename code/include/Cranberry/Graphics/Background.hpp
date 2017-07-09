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


#pragma once
#ifndef CRANBERRY_BACKGROUND_HPP
#define CRANBERRY_BACKGROUND_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/ITexture.hpp>
#include <Cranberry/System/Emitters/BackgroundEmitter.hpp>

// Forward declarations


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
class CRANBERRY_GRAPHICS_EXPORT Background : public ITexture
{
public:

    CRANBERRY_DISABLE_COPY(Background)
    CRANBERRY_DISABLE_MOVE(Background)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of the Background class and sets all members
    /// to their logical default values.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Background();


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the emitter for this object.
    ///
    /// \returns a pointer to the emitter.
    ///
    ////////////////////////////////////////////////////////////////////////////
    BackgroundEmitter* backgroundEmitter();

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
    void setScrollPosition(const QVector2D& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the scroll speed in both directions.
    ///
    /// \param speed QVector2D containing X-speed and Y-speed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setScrollSpeed(const QVector2D& speed);

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
    /// Starts scrolling the background.
    ///
    /// \note The parameter is ignored if ScrollMode == ScrollInfinite.
    /// \param advance QVector2D containing X-advance and Y-advance.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void startScrolling(const QVector2D& advance = QVector2D());

    ////////////////////////////////////////////////////////////////////////////
    /// Terminates the scroll movement manually.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void stopScrolling();

    ////////////////////////////////////////////////////////////////////////////
    /// Implements custom scrolling behaviour.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) override;


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the string representation of this object.
    ///
    /// \returns the string representation.
    ///
    ////////////////////////////////////////////////////////////////////////////
    operator QString() const;


protected:

    void initializeData() override;
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
    QRectF            m_view;          ///> Current view
    ScrollMode        m_scrollMode;    ///< Normal or Infinite?
    MoveDirections    m_scrollDir;     ///< Directions to move background in
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

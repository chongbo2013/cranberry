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
#ifndef CRANBERRY_INPUT_MOUSESTATE_HPP
#define CRANBERRY_INPUT_MOUSESTATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QHash>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds all the mouse buttons currently pressed.
///
/// \class MouseState
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT MouseState
{
public:

    CRANBERRY_DEFAULT_CTOR(MouseState)
    CRANBERRY_DEFAULT_DTOR(MouseState)
    CRANBERRY_DEFAULT_COPY(MouseState)
    CRANBERRY_DEFAULT_MOVE(MouseState)


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given button is pressed.
    ///
    /// \param button Qt::MouseButton to check for.
    /// \returns true if the button is pressed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isButtonDown(int button) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether the given button is released.
    ///
    /// \param button Qt::MouseButton to check for.
    /// \returns true if the button is released.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isButtonUp(int button) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the state of \p button.
    ///
    /// \param button Qt::MouseButton to set state for.
    /// \param state State to assign to mouse button.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setButtonState(int button, bool state);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QHash<int, bool> m_buttonStates; ///< true = pressed, false = released
};


////////////////////////////////////////////////////////////////////////////////
/// \class MouseState
/// \ingroup Input
///
/// Use one of the four convenient functions to determine whether a button
/// is pressed.
///
/// \code
/// void onMouseButtonDown(const MouseState& mouse)
/// {
///     if (mouse.isButtonDown(Qt::LeftButton))
///     {
///         // do some e.g. shooting logic
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

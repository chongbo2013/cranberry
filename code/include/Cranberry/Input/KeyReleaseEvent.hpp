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
#ifndef CRANBERRY_INPUT_KEYRELEASEEVENT_HPP
#define CRANBERRY_INPUT_KEYRELEASEEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds the key that was just released.
///
/// \class KeyReleaseEvent
/// \author Nicolas Kogler
/// \date June 9, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_INPUT_EXPORT KeyReleaseEvent
{
public:

    CRANBERRY_DEFAULT_DTOR(KeyReleaseEvent)
    CRANBERRY_DEFAULT_COPY(KeyReleaseEvent)
    CRANBERRY_DEFAULT_MOVE(KeyReleaseEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new KeyReleaseEvent with a given key and modifiers.
    ///
    /// \param key Key that was released.
    /// \param mods Any modifiers simutaneously pressed.
    ///
    ////////////////////////////////////////////////////////////////////////////
    KeyReleaseEvent(int key, int mods);

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this specific \p key was released or not.
    ///
    /// \param key Qt::Key to check for.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool wasReleased(int key) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this specific \p modifer was pressed.
    ///
    /// \param mod Qt::Modifier to check for.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool hasModifier(int mod) const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    int m_key;  ///< The released key
    int m_mods; ///< One or multiple modifiers combined
};


////////////////////////////////////////////////////////////////////////////////
/// \class KeyReleaseEvent
/// \ingroup Input
///
/// An instance of this class is passed to Window::onKeyReleased(). There it can
/// be checked against any Qt::Key (and Qt::Modifier) using
/// KeyReleaseEvent::wasReleased().
///
/// \code
/// void onKeyReleased(const KeyReleaseEvent& event)
/// {
///     if (event.wasReleased(Qt::Key_Up))
///     {
///         // do stuff
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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
#ifndef CRANBERRY_GAME_MAPPING_EVENTS_OBJECTEVENT_HPP
#define CRANBERRY_GAME_MAPPING_EVENTS_OBJECTEVENT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Forward declarations
CRANBERRY_FORWARD_C(MapObject)
CRANBERRY_FORWARD_C(MapObjectLayer)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This event is emitted when the player is about to step on an object, is
/// stepping on an object or leaving an object.
///
/// \class ObjectEvent
/// \author Nicolas Kogler
/// \date August 22, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT ObjectEvent final
{
public:

    CRANBERRY_DEFAULT_DTOR(ObjectEvent)
    CRANBERRY_DEFAULT_COPY(ObjectEvent)
    CRANBERRY_DEFAULT_MOVE(ObjectEvent)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new object event with the given cause object.
    ///
    /// \param obj Object that caused the event.
    /// \param layer Layer on which the object resides.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ObjectEvent(const MapObject* obj, const MapObjectLayer* layer);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the associated object.
    ///
    /// \returns the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapObject& object() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the associated layer.
    ///
    /// \returns the layer.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const MapObjectLayer& layer() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this event was accepted. Consider the following code:
    ///
    /// \code
    /// void onAboutStepTile(const TileEvent& event)
    /// {
    ///     event.reject();
    /// }
    /// \endcode
    ///
    /// Rejecting the "onAboutStepTile" event will result in the player not
    /// stepping on the tile, thus no "onStepTile" event is emitted.
    ///
    /// \default By default, this value is true.
    /// \returns true if this event was accepted.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isAccepted() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Accepts the event.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void accept() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Rejects the event.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void reject() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    const MapObject*      m_obj;
    const MapObjectLayer* m_layer;
    mutable bool          m_isAccepted;
};


////////////////////////////////////////////////////////////////////////////////
/// \class ObjectEvent
/// \ingroup Game
///
/// This event can be used to implement special object behaviour when the player
/// is about to approach them or already approached them.
///
/// \code
/// void onAboutStepObject(const ObjectEvent& event)
/// {
///     if (event.object().propertyValue("type").toString() == "warp")
///     {
///         // Fetch other properties to perform warp
///     }
///
///     if (event.object().propertyValue("solid").toBool() == true)
///     {
///         event.reject(); // is solid, can not step on it
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

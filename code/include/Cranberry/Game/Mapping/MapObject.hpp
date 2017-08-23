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
#ifndef CRANBERRY_GAME_MAPPING_MAPOBJECT_HPP
#define CRANBERRY_GAME_MAPPING_MAPOBJECT_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QMap>
#include <QString>
#include <QVariant>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines one map object within the map. Can be a warp field, spawn point or
/// something completely else.
///
/// \class MapObject
/// \author Nicolas Kogler
/// \date August 19, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapObject
{
public:

    CRANBERRY_DECLARE_CTOR(MapObject)
    CRANBERRY_DEFAULT_DTOR(MapObject)
    CRANBERRY_DEFAULT_COPY(MapObject)
    CRANBERRY_DEFAULT_MOVE(MapObject)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this object is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the global ID of this object.
    ///
    /// \returns the global ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int id() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the absolute X-position of this object.
    ///
    /// \returns the X-position, in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int x() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the absolute Y-position of this object.
    ///
    /// \returns the Y-position, in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int y() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the width of this object.
    ///
    /// \returns the width, in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int width() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the height of this object.
    ///
    /// \returns the height, in pixels.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int height() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the value of the property \p property.
    ///
    /// \param property Name of the property.
    /// \returns the value or a null QVariant.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVariant& propertyValue(const QString& property) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the modifiable property map.
    ///
    /// \returns the property map.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QMap<QString, QVariant>& properties();

    void setId(int id);
    void setX(int x);
    void setY(int y);
    void setWidth(int w);
    void setHeight(int h);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    int                     m_id;
    int                     m_x;
    int                     m_y;
    int                     m_w;
    int                     m_h;
    QMap<QString, QVariant> m_properties;
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapObject
/// \ingroup Game
///
/// This class may be used to store e.g. spawn points or warp fields.
///
/// \code
/// void onStepObject(const ObjectEvent& event)
/// {
///     if (!event.object().isNull())
///     {
///         // We are stepping on an actual object now.
///         if (event.object().propertyValue("type").toString() == "warp")
///         {
///             // Extract map/position/whatever out of other properties.
///         }
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

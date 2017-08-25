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
#ifndef CRANBERRY_GAME_MAPPING_MAPTILEPROPERTIES_HPP
#define CRANBERRY_GAME_MAPPING_MAPTILEPROPERTIES_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QMap>
#include <QString>
#include <QVariant>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Holds properties for a single block inside a tileset.
///
/// \class MapTileProperties
/// \author Nicolas Kogler
/// \date August 15, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT MapTileProperties final
{
public:

    CRANBERRY_DEFAULT_CTOR(MapTileProperties)
    CRANBERRY_DEFAULT_DTOR(MapTileProperties)
    CRANBERRY_DEFAULT_COPY(MapTileProperties)
    CRANBERRY_DEFAULT_MOVE(MapTileProperties)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this tile properties are valid.
    ///
    /// \returns true if valid.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isValid() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves value of the property called \p name.
    ///
    /// \param name Name of the property.
    /// \returns the value of the property.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVariant& value(const QString& name) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves a modifiable reference to the properties.
    ///
    /// \returns a reference to the properties.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QMap<QString, QVariant>& properties();

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies a new value for the property called \p name.
    ///
    /// \param name Name of the property.
    /// \param value New value of the property.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setValue(const QString& name, const QVariant& value);

    ////////////////////////////////////////////////////////////////////////////
    /// Adds a new tile property.
    ///
    /// \param name Name of the new property.
    /// \param value Value of the new property.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void addProperty(const QString& name, const QVariant& value);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the property called \p name.
    ///
    /// \param name Name of the property to remove.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeProperty(const QString& name);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    QMap<QString, QVariant> m_properties; ///< Properties associated to the tile
};


////////////////////////////////////////////////////////////////////////////////
/// \class MapTileProperties
/// \ingroup Game
///
/// Tile properties can be used to trigger certain effects when about to step
/// on a tile or already stepped on it. Use cases might be collision detection,
/// warping or animations.
///
/// \code
/// void onAboutStepTile(const TileEvent& event)
/// {
///     if (event.properties().value("solid").toBool())
///     {
///         event.reject(); // do not step on tile
///     }
/// }
///
/// void onStepTile(const TileEvent& event)
/// {
///     if (event.properties().value("warp").isValid())
///     {
///         // do some warp stuff
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

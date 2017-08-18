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
#ifndef CRANBERRY_GAME_MAPPING_ENUMERATIONS_HPP
#define CRANBERRY_GAME_MAPPING_ENUMERATIONS_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QColor>
#include <QMap>
#include <QString>
#include <QVariant>

// Forward declarations
CRANBERRY_FORWARD_Q(QDomElement)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the property types within TMX files.
///
/// \enum PropertyType
///
////////////////////////////////////////////////////////////////////////////////
enum PropertyType
{
    PropertyTypeInvalid = -1,
    PropertyTypeString,
    PropertyTypeInteger,
    PropertyTypeFloat,
    PropertyTypeBoolean,
    PropertyTypeColor,
    PropertyTypeFile
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the orientation of the map.
///
/// \enum MapOrientation
///
////////////////////////////////////////////////////////////////////////////////
enum MapOrientation
{
    MapOrientationInvalid = -1,
    MapOrientationOrthogonal,
    MapOrientationIsometric,
    MapOrientationStaggered,
    MapOrientationHexagonal
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the encoding type of the layer data.
///
/// \enum LayerEncoding
///
////////////////////////////////////////////////////////////////////////////////
enum LayerEncoding
{
    LayerEncodingNone,
    LayerEncodingBase64,
    LayerEncodingCsv
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the compression mode of the layer data.
///
/// \enum CompressionMode
///
////////////////////////////////////////////////////////////////////////////////
enum CompressionMode
{
    CompressionModeNone,
    CompressionModeGzip,
    CompressionModeZlib
};

////////////////////////////////////////////////////////////////////////////////
/// This enum specifies the flip flags of the tile.
///
/// \enum FlipFlag
///
////////////////////////////////////////////////////////////////////////////////
enum FlipFlag
{
    FlipFlagNone       = 0x00000000,
    FlipFlagDiagonal   = 0x20000000,
    FlipFlagVertical   = 0x40000000,
    FlipFlagHorizontal = 0x80000000,
    FlipFlagAll        = FlipFlagDiagonal |
                         FlipFlagVertical |
                         FlipFlagHorizontal
};


////////////////////////////////////////////////////////////////////////////////
// Qt flags
////////////////////////////////////////////////////////////////////////////////
Q_DECLARE_FLAGS(PropertyTypes, PropertyType)
Q_DECLARE_FLAGS(MapOrientations, MapOrientation)
Q_DECLARE_FLAGS(LayerEncodings, LayerEncoding)
Q_DECLARE_FLAGS(CompressionModes, CompressionMode)
Q_DECLARE_FLAGS(FlipFlags, FlipFlag)


////////////////////////////////////////////////////////////////////////////////
// Useful functions
////////////////////////////////////////////////////////////////////////////////
MapOrientation getOrientationFromString(const QString& type);
PropertyType getPropertyTypeFromString(const QString& type);
LayerEncoding getEncodingFromString(const QString& type);
CompressionMode getCompressionFromString(const QString& type);
QVariant getPropertyValue(PropertyType type, const QString& value);
QColor getColorFromString(QString str);
void getTmxProperties(QDomElement* element, QMap<QString, QVariant>& p);


CRANBERRY_END_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Qt flag operators
////////////////////////////////////////////////////////////////////////////////
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::MapOrientations)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::LayerEncodings)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::CompressionModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(CRANBERRY_NAMESPACE::FlipFlags)


#endif

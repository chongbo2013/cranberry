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


// Cranberry headers
#include <Cranberry/Game/Mapping/Enumerations.hpp>

// Qt headers
#include <QtXml/QDomElement>


QColor cran::getColorFromString(QString str)
{
    if (str.isEmpty()) return QColor();

    return QColor(
           str.mid(3, 2).toInt(0, 16),
           str.mid(5, 2).toInt(0, 16),
           str.mid(7, 2).toInt(0, 16),
           str.mid(1, 2).toInt(0, 16));
}


cran::MapOrientation cran::getOrientationFromString(const QString& type)
{
    if (type == "orthogonal") return MapOrientationOrthogonal;
    if (type == "isometric")  return MapOrientationIsometric;
    if (type == "staggered")  return MapOrientationStaggered;
    if (type == "hexagonal")  return MapOrientationHexagonal;

    return MapOrientationInvalid;
}


cran::PropertyType cran::getPropertyTypeFromString(const QString& type)
{
    // Fix: No type defaults to "string".
    if (type == "")       return PropertyTypeString;
    if (type == "string") return PropertyTypeString;
    if (type == "int")    return PropertyTypeInteger;
    if (type == "float")  return PropertyTypeFloat;
    if (type == "bool")   return PropertyTypeBoolean;
    if (type == "color")  return PropertyTypeColor;
    if (type == "file")   return PropertyTypeFile;

    return PropertyTypeInvalid;
}


cran::LayerEncoding cran::getEncodingFromString(const QString& type)
{
    if (type == "base64") return LayerEncodingBase64;
    if (type == "csv")    return LayerEncodingCsv;

    return LayerEncodingNone;
}


cran::CompressionMode cran::getCompressionFromString(const QString& type)
{
    if (type == "gzip") return CompressionModeGzip;
    if (type == "zlib") return CompressionModeZlib;

    return CompressionModeNone;
}


QVariant cran::getPropertyValue(PropertyType type, const QString& value)
{
    switch (type)
    {
        case PropertyTypeFile:
        case PropertyTypeString:  return value;
        case PropertyTypeInteger: return value.toInt();
        case PropertyTypeFloat:   return value.toFloat();
        case PropertyTypeBoolean: return value == "true" || value == "1";
        case PropertyTypeColor:   return getColorFromString(value);
        default:                  return QVariant();
    }
}


void cran::getTmxProperties(QDomElement* element, QMap<QString, QVariant>& props)
{
    if (!element->isNull())
    {
        auto elemList = element->elementsByTagName("property");
        for (int i = 0; i < elemList.size(); i++)
        {
            QDomElement elem = elemList.at(i).toElement();
            QString name = elem.attribute("name");
            QString type = elem.attribute("type");
            QString value = elem.attribute("value");
            PropertyType t = getPropertyTypeFromString(type);

            if (value.isEmpty())
            {
                // TMX appearantly saves multi-line strings inside the element.
                props.insert(name, getPropertyValue(t, elem.nodeValue()));
            }
            else
            {
                props.insert(name, getPropertyValue(t, value));
            }
        }
    }
}

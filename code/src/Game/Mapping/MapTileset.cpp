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
#include <Cranberry/Game/Mapping/MapTileset.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QOpenGLTexture>
#include <QtXml/QDomElement>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "TMX (tileset): \"firstgid\" attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_02, "TMX (tileset): \"tilewidth\" attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_03, "TMX (tileset): \"tileheight\" attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_04, "TMX (tileset): \"tilecount\" attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_05, "TMX (image): \"source\" attribute is missing.")
CRANBERRY_CONST_VAR(QString, e_06, "TMX (image): Path to image invalid.")


CRANBERRY_USING_NAMESPACE


MapTileset::MapTileset()
    : m_globalId(-1)
    , m_texture(nullptr)
    , m_tileWidth(-1)
    , m_tileHeight(-1)
    , m_tileSpacing(0)
    , m_tileMargin(0)
    , m_tileCount(0)
{
}


MapTileset::~MapTileset()
{
    delete m_texture;
}


int MapTileset::globalId() const
{
    return m_globalId;
}


const QString& MapTileset::imagePath() const
{
    return m_imagePath;
}


const QString& MapTileset::name() const
{
    return m_name;
}


QOpenGLTexture* MapTileset::texture() const
{
    return m_texture;
}


int MapTileset::tileWidth() const
{
    return m_tileWidth;
}


int MapTileset::tileHeight() const
{
    return m_tileHeight;
}


int MapTileset::tileSpacing() const
{
    return m_tileSpacing;
}


int MapTileset::tileMargin() const
{
    return m_tileMargin;
}


int MapTileset::tileCount() const
{
    return m_tileCount;
}


const QMap<QString, QVariant>& MapTileset::properties() const
{
    return m_properties;
}


const MapTileProperties& MapTileset::tileProperties(int tileId) const
{
    auto it = m_tileProps.find(tileId);
    return it.value();
}


bool MapTileset::parse(QDomElement* xmlElement)
{
    // <tileset>...</tileset>
    // Parses the attributes.
    QString strFirstgid = xmlElement->attribute("firstgid");
    if (strFirstgid.isEmpty())
    {
        return cranError(e_01);
    }

    QString strTilewidth = xmlElement->attribute("tilewidth");
    if (strTilewidth.isEmpty())
    {
        return cranError(e_02);
    }

    QString strTileheight = xmlElement->attribute("tileheight");
    if (strTileheight.isEmpty())
    {
        return cranError(e_03);
    }

    QString strTilecount = xmlElement->attribute("tilecount");
    if (strTilecount.isEmpty())
    {
        return cranError(e_04);
    }

    m_globalId = strFirstgid.toInt();
    m_tileWidth = strTilewidth.toInt();
    m_tileHeight = strTileheight.toInt();
    m_tileCount = strTilecount.toInt();
    m_tileSpacing = xmlElement->attribute("spacing").toInt();
    m_tileMargin = xmlElement->attribute("margin").toInt();
    m_name = xmlElement->attribute("name");

    // Parses the properties element.
    QDomElement xmlProps = xmlElement->elementsByTagName("properties").at(0).toElement();
    getTmxProperties(&xmlProps, m_properties);

    // Parses the tile elements. TODO: tile animations.
    auto tileElems = xmlElement->elementsByTagName("tile");
    for (int i = 0; i < tileElems.size(); i++)
    {
        MapTileProperties mtp;
        QDomElement elem = tileElems.at(i).toElement();
        QString id = elem.attribute("id");
        QDomElement props = elem.elementsByTagName("properties").at(0).toElement();

        getTmxProperties(&props, mtp.properties());
        m_tileProps.insert(id.toInt(), mtp);
    }

    // Parses the image element.
    QDomElement xmlImg = xmlElement->elementsByTagName("image").at(0).toElement();
    if (xmlImg.isNull())
    {
        return cranError(e_05);
    }

    QString strSource = xmlImg.attribute("source");
    if (strSource.isEmpty())
    {
        return cranError(e_05);
    }

    QImage img(strSource);
    if (img.isNull())
    {
        return cranError(e_06);
    }

    m_texture = new QOpenGLTexture(img, QOpenGLTexture::DontGenerateMipMaps);
    m_texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    return true;
}

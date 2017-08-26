////////////////////////////////////////////////////////////////////////////////
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
#ifndef CRANBERRY_GAME_MAPPING_MAPOBJECT_HPP
#define CRANBERRY_GAME_MAPPING_MAPOBJECT_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/TransformBase.hpp>

// Qt headers
#include <QMap>
#include <QString>
#include <QVariant>

// Forward declarations
CRANBERRY_FORWARD_C(RenderBase)


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
class CRANBERRY_GAME_EXPORT MapObject : public TransformBase
{
public:

    CRANBERRY_DECLARE_CTOR(MapObject)
    CRANBERRY_DECLARE_DTOR(MapObject)
    CRANBERRY_DISABLE_COPY(MapObject)
    CRANBERRY_DISABLE_MOVE(MapObject)

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
    /// Retrieves the name of this object.
    ///
    /// \returns the name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the type of this object (e.g. "npc" or "warp").
    ///
    /// \returns the type.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& type() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the value of the property \p property.
    ///
    /// \param property Name of the property.
    /// \returns the value or a null QVariant.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVariant& propertyValue(const QString& property) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the associated render object of this object.
    ///
    /// \returns the render object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    RenderBase* renderObject() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the modifiable property map.
    ///
    /// \returns the property map.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QMap<QString, QVariant>& properties();

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the global ID of this object.
    ///
    /// \param id Global ID.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setId(int id);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the name of this object.
    ///
    /// \param name New name of the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the type of this object.
    ///
    /// \param type New type of the object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setType(const QString& type);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the object to be rendered when calling render(). The transform
    /// properties of it will be replaced by the transform properties of this
    /// class. If you do not want this, synchronise transformations with this
    /// object by issuing "mapObject->synchroniseWith(myRenderObject);".
    ///
    /// \param obj RenderBase object to render.
    /// \param takeOwnership If true, the object will be deleted automatically.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setRenderObject(RenderBase* obj, bool takeOwnership = false);


public overridable:

    ////////////////////////////////////////////////////////////////////////////
    // Virtual functions
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time);
    virtual void render();


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    int                     m_id;
    QString                 m_name;
    QString                 m_type;
    QMap<QString, QVariant> m_properties;
    RenderBase*             m_renderObject;
    bool                    m_takeOwnership;

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
///         // We are stepping on an actual object now
///         if (event.object().propertyValue("type").toString() == "warp")
///         {
///             // Extract warp data out of other properties
///         }
///     }
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

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
#ifndef CRANBERRY_TREEMODEL_HPP
#define CRANBERRY_TREEMODEL_HPP


// Cranberry headers
#include <Cranberry/System/Models/TreeModelItem.hpp>

// Forward declarations
CRANBERRY_FORWARD_P(TreeModelPrivate)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a tree model that holds several items.
///
/// \class TreeModel
/// \author Nicolas Kogler
/// \date July 23, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT TreeModel final
{
public:

    CRANBERRY_DECLARE_CTOR(TreeModel)
    CRANBERRY_DECLARE_DTOR(TreeModel)
    CRANBERRY_DEFAULT_COPY(TreeModel)
    CRANBERRY_DEFAULT_MOVE(TreeModel)

    ////////////////////////////////////////////////////////////////////////////
    /// Adds a top item to the tree model. The item can also contain child items
    /// which again contain child items. This is useful for displaying structure
    /// members, like a rectangle with x, y, width and height members.
    ///
    /// \param item Item to add.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void addItem(TreeModelItem* item);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all items from this model.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllItems();

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the items.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update();

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the underlying model. Note that using this function requires
    /// the dependency QtGui. In reality you would never need this, since only
    /// cranberry needs to call this function.
    ///
    /// \returns the underlying Qt model.
    ///
    ////////////////////////////////////////////////////////////////////////////
    priv::TreeModelPrivate* model() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    priv::TreeModelPrivate* m_priv;
};


////////////////////////////////////////////////////////////////////////////////
/// \class TreeModel
/// \ingroup System
///
/// Primarily used for building up the property tree for the debug overlay.
///
/// \code
/// void parseProperties(TreeModel* model)
/// {
///     TreeItem* item = new TreeItem("MyRenderBaseObject", "");
///     TreeItem* mem1 = new TreeItem("fbo", m_fbo, item);
///     TreeItem* mem2 = new TreeItem("tex", m_tex, item);
///
///     ...
///
///     model->addItem(item);
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

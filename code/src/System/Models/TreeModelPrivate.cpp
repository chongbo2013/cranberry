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
#include <Cranberry/System/Models/TreeModelItem.hpp>
#include <Cranberry/System/Models/TreeModelPrivate.hpp>


CRANBERRY_USING_NAMESPACE


priv::TreeModelPrivate::TreeModelPrivate()
    : QAbstractItemModel()
    , m_rootItem(new TreeModelItem("Properties", ""))
{
}


priv::TreeModelPrivate::~TreeModelPrivate()
{
    delete m_rootItem;
}


int priv::TreeModelPrivate::columnCount(const QModelIndex&) const
{
    return 2;
}


QVariant priv::TreeModelPrivate::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (index.column() == 0)
    {
        return static_cast<TreeModelItem*>(index.internalPointer())->member();
    }
    else if (index.column() == 1)
    {
        return static_cast<TreeModelItem*>(index.internalPointer())->value();
    }

    return QVariant();
}


Qt::ItemFlags priv::TreeModelPrivate::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}


QVariant priv::TreeModelPrivate::headerData(
    int section,
    Qt::Orientation orientation,
    int role
    ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return m_rootItem->member();
        }
        else if (section == 1)
        {
            return m_rootItem->value();
        }
    }

    return QVariant();
}


QModelIndex priv::TreeModelPrivate::index(
    int row,
    int column,
    const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeModelItem* parentItem;
    TreeModelItem* childItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<TreeModelItem*>(parent.internalPointer());
    }

    childItem = parentItem->childAt(row);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}


QModelIndex priv::TreeModelPrivate::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeModelItem* childItem = static_cast<TreeModelItem*>(index.internalPointer());
    TreeModelItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}


int priv::TreeModelPrivate::rowCount(const QModelIndex& parent) const
{
    TreeModelItem* parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<TreeModelItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

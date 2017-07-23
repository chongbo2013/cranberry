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


CRANBERRY_USING_NAMESPACE


TreeModelItem::TreeModelItem(QVariant member, QVariant value, TreeModelItem* parent)
    : m_member(member)
    , m_value(value)
    , m_parent(parent)
{
}


TreeModelItem::~TreeModelItem()
{
    qDeleteAll(m_items);
}


void TreeModelItem::appendChild(TreeModelItem* child)
{
    m_items.append(child);
}


int TreeModelItem::childCount() const
{
    return m_items.count();
}


int TreeModelItem::row() const
{
    if (m_parent)
    {
        return m_parent->m_items.indexOf(const_cast<TreeModelItem*>(this));
    }

    return 0;
}


TreeModelItem* TreeModelItem::childAt(int row) const
{
    return m_items.value(row);
}


TreeModelItem* TreeModelItem::parentItem() const
{
    return m_parent;
}


QVariant TreeModelItem::member() const
{
    return m_member;
}


QVariant TreeModelItem::value() const
{
    return m_value;
}

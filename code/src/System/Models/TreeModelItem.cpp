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


// Cranberry headers
#include <Cranberry/System/Models/TreeModelItem.hpp>


CRANBERRY_USING_NAMESPACE


TreeModelItem::TreeModelItem(QVariant member, QVariant value, TreeModelItem* parent)
    : m_member(member)
    , m_value(value)
    , m_parent(parent)
{
}


TreeModelItem::TreeModelItem(const TreeModelItem& other)
    : m_items(other.m_items)
    , m_member(other.m_member)
    , m_value(other.m_value)
    , m_parent(other.m_parent)
{
}


TreeModelItem::~TreeModelItem()
{
    qDeleteAll(m_items);
}


void TreeModelItem::appendChild(TreeModelItem* child)
{
    m_items.append(child);
    child->m_parent = this;
}


void TreeModelItem::insertChild(int index, TreeModelItem* child)
{
    m_items.insert(index, child);
    child->m_parent = this;
}


void TreeModelItem::removeChild(int index)
{
    if (index >= 0 && index < m_items.size())
    {
        auto* item = m_items.takeAt(index);
        delete item;
    }
}


void TreeModelItem::removeAllChildren()
{
    qDeleteAll(m_items);
    m_items.clear();
}


int TreeModelItem::childCount() const
{
    return m_items.count();
}


int TreeModelItem::totalChildCount() const
{
    int count = childCount();

    Q_FOREACH (TreeModelItem* i, m_items)
    {
        count += i->totalChildCount();
    }

    return count;
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


void TreeModelItem::setMember(const QVariant& m)
{
    m_member = m;
}


void TreeModelItem::setValue(const QVariant& v)
{
    m_value = v;
}

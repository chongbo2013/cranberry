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
#ifndef CRANBERRY_SYSTEM_MODELS_TREEMODELPRIVATE_HPP
#define CRANBERRY_SYSTEM_MODELS_TREEMODELPRIVATE_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QAbstractItemModel>
#include <QHash>
#include <QModelIndex>
#include <QVariant>

// Forward declarations
CRANBERRY_FORWARD_C(TreeModel)
CRANBERRY_FORWARD_C(TreeModelItem)


CRANBERRY_BEGIN_PRIV_NAMESPACE


class TreeModelPrivate final : public QAbstractItemModel
{
public:

    CRANBERRY_DECLARE_CTOR(TreeModelPrivate)
    CRANBERRY_DECLARE_DTOR(TreeModelPrivate)
    CRANBERRY_DEFAULT_COPY(TreeModelPrivate)
    CRANBERRY_DISABLE_MOVE(TreeModelPrivate)

    void appendChild(TreeModelItem* item);
    void finalizeInsertion();
    void update();
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem*        m_rootItem;
    QList<TreeModelItem*> m_insertionQueue;

    Q_OBJECT
    friend class cran::TreeModel;
};



CRANBERRY_END_PRIV_NAMESPACE


Q_DECLARE_METATYPE(cran::priv::TreeModelPrivate*)


#endif

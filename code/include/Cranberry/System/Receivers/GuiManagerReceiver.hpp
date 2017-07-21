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
#ifndef CRANBERRY_SYSTEM_RECEIVERS_GUIMANAGERRECEIVER_HPP
#define CRANBERRY_SYSTEM_RECEIVERS_GUIMANAGERRECEIVER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_FORWARD_C(GuiManager)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Receives signals for GuiManager objects.
///
/// \class GuiManagerReceiver
/// \author Nicolas Kogler
/// \date July 18, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT GuiManagerReceiver : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(GuiManagerReceiver)
    CRANBERRY_DEFAULT_DTOR(GuiManagerReceiver)
    CRANBERRY_DEFAULT_COPY(GuiManagerReceiver)
    CRANBERRY_DEFAULT_MOVE(GuiManagerReceiver)

    void setGuiManager(GuiManager* manager);


private Q_SLOTS:

    void loadComponents();
    void requestUpdate();
    void resizeFbo();


private:

    GuiManager* m_manager = nullptr;

    friend class GuiManager;
    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

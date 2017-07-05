////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt5 framework.
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
#ifndef CRANBERRY_${CLASS}EMITTER_HPP
#define CRANBERRY_${CLASS}EMITTER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for ${Class} objects.
///
/// \class ${Class}Emitter
/// \author Nicolas Kogler
/// \date June 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT ${Class}Emitter : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(${Class}Emitter)
    CRANBERRY_DEFAULT_DTOR(${Class}Emitter)
    CRANBERRY_DEFAULT_COPY(${Class}Emitter)
    CRANBERRY_DEFAULT_MOVE(${Class}Emitter)


    inline void emitAbc() { emit abc(); }

signals:

    void abc();


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

﻿////////////////////////////////////////////////////////////////////////////////
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
#ifndef CRANBERRY_SYSTEM_EMITTERS_TRANSFORMBASEEMITTER_HPP
#define CRANBERRY_SYSTEM_EMITTERS_TRANSFORMBASEEMITTER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for TransformBase objects.
///
/// \class TransformbaseEmitter
/// \author Nicolas Kogler
/// \date June 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT TransformBaseEmitter : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(TransformBaseEmitter)
    CRANBERRY_DEFAULT_DTOR(TransformBaseEmitter)
    CRANBERRY_DEFAULT_COPY(TransformBaseEmitter)
    CRANBERRY_DEFAULT_MOVE(TransformBaseEmitter)

    inline void emitFinishedMove() { Q_EMIT finishedMove(); }
    inline void emitFinishedRotate() { Q_EMIT finishedRotate(); }
    inline void emitFinishedScale() { Q_EMIT finishedScale(); }
    inline void emitFinishedFade() { Q_EMIT finishedFade(); }
    inline void emitPositionChanged() { Q_EMIT positionChanged(); }
    inline void emitSizeChanged() { Q_EMIT sizeChanged(); }


Q_SIGNALS:

    void finishedMove();
    void finishedRotate();
    void finishedScale();
    void finishedFade();
    void positionChanged();
    void sizeChanged();


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

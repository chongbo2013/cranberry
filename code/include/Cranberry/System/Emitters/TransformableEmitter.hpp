﻿////////////////////////////////////////////////////////////////////////////////
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
#ifndef CRANBERRY_SYSTEM_EMITTERS_TRANSFORMABLEEMITTER_HPP
#define CRANBERRY_SYSTEM_EMITTERS_TRANSFORMABLEEMITTER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for ITransformable objects.
///
/// \class TransformableEmitter
/// \author Nicolas Kogler
/// \date June 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT TransformableEmitter : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(TransformableEmitter)
    CRANBERRY_DEFAULT_DTOR(TransformableEmitter)
    CRANBERRY_DEFAULT_COPY(TransformableEmitter)
    CRANBERRY_DEFAULT_MOVE(TransformableEmitter)


    inline void emitStoppedMoving() { Q_EMIT stoppedMoving(); }
    inline void emitStoppedRotating() { Q_EMIT stoppedRotating(); }
    inline void emitStoppedScaling() { Q_EMIT stoppedScaling(); }
    inline void emitStoppedFading() { Q_EMIT stoppedFading(); }
    inline void emitPositionChanged() { Q_EMIT positionChanged(); }
    inline void emitSizeChanged() { Q_EMIT sizeChanged(); }


Q_SIGNALS:

    void stoppedMoving();
    void stoppedRotating();
    void stoppedScaling();
    void stoppedFading();
    void positionChanged();
    void sizeChanged();


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

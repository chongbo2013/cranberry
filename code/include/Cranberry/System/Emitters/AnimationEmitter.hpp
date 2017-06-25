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
#ifndef CRANBERRY_ANIMATIONEMITTER_HPP
#define CRANBERRY_ANIMATIONEMITTER_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for IAnimation objects.
///
/// \class AnimationEmitter
/// \author Nicolas Kogler
/// \date June 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT AnimationEmitter : public QObject
{
public:

    CRANBERRY_DEFAULT_CTOR(AnimationEmitter)
    CRANBERRY_DEFAULT_DTOR(AnimationEmitter)
    CRANBERRY_DEFAULT_COPY(AnimationEmitter)
    CRANBERRY_DEFAULT_MOVE(AnimationEmitter)


    inline void emitStoppedAnimating() { emit stoppedAnimating(); }

signals:

    void stoppedAnimating();


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

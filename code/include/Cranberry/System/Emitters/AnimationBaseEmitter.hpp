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
#ifndef CRANBERRY_SYSTEM_EMITTERS_ANIMATIONBASEEMITTER_HPP
#define CRANBERRY_SYSTEM_EMITTERS_ANIMATIONBASEEMITTER_HPP


// Cranberry headers
#include <Cranberry/System/Emitters/RenderBaseEmitter.hpp>

// Qt headers
#include <QObject>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Emits signals for AnimationBase objects.
///
/// \class AnimationBaseEmitter
/// \author Nicolas Kogler
/// \date June 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_SYSTEM_EXPORT AnimationBaseEmitter : public RenderBaseEmitter
{
public:

    CRANBERRY_DEFAULT_CTOR(AnimationBaseEmitter)
    CRANBERRY_DEFAULT_DTOR(AnimationBaseEmitter)
    CRANBERRY_DEFAULT_COPY(AnimationBaseEmitter)
    CRANBERRY_DEFAULT_MOVE(AnimationBaseEmitter)


    inline void emitFinishedAnimation() { Q_EMIT finishedAnimation(); }

Q_SIGNALS:

    void finishedAnimation();


private:

    Q_OBJECT
};


CRANBERRY_END_NAMESPACE


#endif

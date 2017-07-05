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
#ifndef CRANBERRY_GIFANIMATION_HPP
#define CRANBERRY_GIFANIMATION_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/IAnimation.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders an animation from a GIF file.
///
/// \class GifAnimation
/// \author Nicolas Kogler
/// \date July 4, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT GifAnimation final : public IAnimation
{
public:

    CRANBERRY_DEFAULT_CTOR(GifAnimation)
    CRANBERRY_DEFAULT_DTOR(GifAnimation)
    CRANBERRY_DISABLE_COPY(GifAnimation)
    CRANBERRY_DISABLE_MOVE(GifAnimation)


    ////////////////////////////////////////////////////////////////////////////
    /// Creates the animation by loading all data from a GIF file.
    ///
    /// \param path Path to a *.gif file.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& path, Window* renderTarget) override;
};


////////////////////////////////////////////////////////////////////////////////
/// \class GifAnimation
/// \ingroup Graphics
///
/// Loads and plays GIF files.
///
/// \code
/// GifAnimation gif;
/// gif.create(":/anims/foo.gif", this);
/// gif.setPosition(QVector2D(100, 100));
/// gif.setEffect(EffectSepia);
/// gif.startAnimation(AnimateForever);
///
/// ...
///
/// gif.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

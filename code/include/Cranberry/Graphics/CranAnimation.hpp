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
#ifndef CRANBERRY_CRANANIMATION_HPP
#define CRANBERRY_CRANANIMATION_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/IAnimation.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Renders an animation from a CBA file.
///
/// \class CranAnimation
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT CranAnimation : public IAnimation
{
public:

    CRANBERRY_DEFAULT_CTOR(CranAnimation)
    CRANBERRY_DEFAULT_DTOR(CranAnimation)
    CRANBERRY_DISABLE_COPY(CranAnimation)
    CRANBERRY_DISABLE_MOVE(CranAnimation)


    ////////////////////////////////////////////////////////////////////////////
    /// Creates the animation by loading all data from a CBA file.
    ///
    /// \param path Path to a *.cba file.
    /// \param renderTarget Target to render animation on.
    /// \returns true if created successfully.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& path, Window* renderTarget) override;
};


////////////////////////////////////////////////////////////////////////////////
/// \class CranAnimation
/// \ingroup Graphics
///
/// Loads and plays CBA files.
///
/// \code
/// CranAnimation cba;
/// cba.create(":/anims/foo.cba", this);
/// cba.setPosition(QVector2D(100, 100));
/// cba.setEffect(EffectSepia);
/// cba.startAnimation(AnimateOnce)
///
/// ...
///
/// cba.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

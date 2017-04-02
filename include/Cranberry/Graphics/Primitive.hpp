//
//  cranberry: C++ game engine using the Qt framework and OpenGL/ES.
//  Copyright (C) 2017 Nicolas Kogler
//
//  Cranberry is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cranberry is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with cranberry. If not, see <http://www.gnu.org/licenses/>.
//


#pragma once
#ifndef CRANBERRY_PRIMITIVE_HPP
#define CRANBERRY_PRIMITIVE_HPP


// Cranberry headers
#include <Cranberry/Graphics/System/Drawable.hpp>
#include <Cranberry/Graphics/System/Fadable.hpp>
#include <Cranberry/Graphics/System/GraphicsStructs.hpp>
#include <Cranberry/Graphics/System/Transformable.hpp>

// Standard headers
#include <array>

// Forward declarations
class QOpenGLVertexArrayObject;
class QOpenGLTexture;


CRANBERRY_BEGIN_NAMESPACE


///
/// Renders a simple primitive.
///
/// \class Primitive
/// \author Nicolas Kogler
/// \date March 3, 2017
///
class CRANBERRY_EXPORT Primitive
        : public Drawable
        , public Transformable
        , public Fadable
{
public:

    CRANBERRY_DISABLE_MOVE(Primitive)

    ///
    /// Initializes a new instance of Primitive.
    ///
    Primitive();

    ///
    /// Copies one Primitive instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Primitive instance.
    ///
    Primitive(const Primitive& other);

    ///
    /// Copies one Primitive instance to another
    /// while sharing the OpenGL resources.
    ///
    /// \param other The other Primitive instance.
    ///
    Primitive& operator=(const Primitive& other);

    ///
    /// Destroys this Primitive instance.
    ///
    virtual ~Primitive();


    ///
    /// Determines whether this shape is filled.
    ///
    /// \returns true if filled.
    ///
    bool isShapeFilled() const;

    ///
    /// Either renders this primitive filled or wired.
    ///
    /// \param filled True to fill primitive.
    ///
    void setShapeFilled(bool filled);


    ///
    /// Must either specify a render target by calling
    /// Drawable::setRenderTarget() beforehand or
    /// by passing a window to draw this primitive on
    /// to this function.
    ///
    /// \param target The window to draw the primitive on.
    /// \returns true if successfully created.
    ///
    bool create(Window* target = nullptr);

    ///
    /// Destroys the underlying OpenGL objects.
    ///
    void destroy() override;

    ///
    /// Updates movement, rotation, scale and fading.
    ///
    /// \param time Contains the delta time.
    ///
    void update(const GameTime& time) override;

    ///
    /// Renders the object in its current state.
    ///
    void render() override;


protected:

    virtual uint32_t renderModeWired() const;
    virtual uint32_t renderModeFilled() const;
    void requestUpdate();
    std::vector<VxPrimitive>& vertices();


private:

    // Members
    uint32_t*       m_refCount;
    bool            m_fillShape;
    bool            m_needsUpdate;

    std::vector<VxPrimitive> m_vertices;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_PRIMITIVE_HPP

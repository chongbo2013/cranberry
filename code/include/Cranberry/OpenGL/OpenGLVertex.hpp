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
#ifndef CRANBERRY_GLVERTEX_HPP
#define CRANBERRY_GLVERTEX_HPP


// Cranberry headers
#include <Cranberry/Config.hpp>

// Qt headers
#include <QColor>
#include <QVector2D>
#include <QVector3D>

// Standard headers
#include <array>
#include <vector>


CRANBERRY_BEGIN_PRIV_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines a single vertex having a XYZ position and a RGBA color. Future- and
/// compiler-proof since it does not rely on the order or alignment of members.
///
/// \class GLVertex
/// \author Nicolas Kogler
/// \date May 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class Vertex
{
public:

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the vertex.
    ///
    /// \param x X-position.
    /// \param y Y-position.
    /// \param z Z-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void xyz(float x, float y, float z = 0.f);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the vertex.
    ///
    /// \param pos 3D-Vector representing the triple.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void xyz(const QVector3D& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color of the vertex.
    ///
    /// \param x X-position.
    /// \param y Y-position.
    /// \param z Z-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void rgba(float r, float g, float b, float a = 1.f);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color of the vertex.
    ///
    /// \param color QColor object representing the quadruple.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void rgba(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the raw floating-point data.
    ///
    /// \returns a pointer to the raw data.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const float* data() const;

    // Attribute offsets and sizes
    static constexpr uint  xyzAttrib()  { return 0; }
    static constexpr uint  rgbaAttrib() { return 1; }
    static constexpr uint  xyzLength()  { return 3; }
    static constexpr uint  rgbaLength() { return 4; }
    static constexpr uint  size()       { return sizeof(float) * 7; }
    static constexpr void* xyzOffset()  { return (void*) (nullptr); }
    static constexpr void* rgbaOffset() { return (void*) (sizeof(float) * 3); }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    float m_fields[7] = { 0, 0, 0, 0, 0, 0, 1 };
};

////////////////////////////////////////////////////////////////////////////////
/// Defines a single vertex having a XYZ position, a RGBA color and UV texture
/// coordinate components. Future- and compiler-proof since it does not rely
/// on the order or alignment of members.
///
/// \class GLVertex
/// \author Nicolas Kogler
/// \date May 25, 2017
///
////////////////////////////////////////////////////////////////////////////////
class TextureVertex
{
public:

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the vertex.
    ///
    /// \param x X-position.
    /// \param y Y-position.
    /// \param z Z-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void xyz(float x, float y, float z);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the position of the vertex.
    ///
    /// \param pos 3D-Vector representing the XYZ triple.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void xyz(const QVector3D& pos);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color of the vertex.
    ///
    /// \param x X-position.
    /// \param y Y-position.
    /// \param z Z-position.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void rgba(float r, float g, float b, float a);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color of the vertex.
    ///
    /// \param color QColor object representing the RGBA quadruple.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void rgba(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the texture coordinates of the vertex.
    ///
    /// \param u U-coordinate.
    /// \param v V-coordinate.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void uv(float u, float v);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the texture coordinates of the vertex.
    ///
    /// \param coords 2D-Vector representing the UV tuple.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void uv(const QVector2D& coords);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the raw floating-point data.
    ///
    /// \returns a pointer to the raw data.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const float* data() const;

    // Attribute offsets and sizes
    static constexpr uint  xyzAttrib()  { return 0; }
    static constexpr uint  uvAttrib()   { return 1; }
    static constexpr uint  rgbaAttrib() { return 2; }
    static constexpr uint  xyzLength()  { return 3; }
    static constexpr uint  uvLength()   { return 2; }
    static constexpr uint  rgbaLength() { return 4; }
    static constexpr uint  size()       { return sizeof(float) * 9; }
    static constexpr void* xyzOffset()  { return (void*) (nullptr); }
    static constexpr void* uvOffset()   { return (void*) (sizeof(float) * 3); }
    static constexpr void* rgbaOffset() { return (void*) (sizeof(float) * 5); }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
     float m_fields[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
};


template <size_t n> using Vertices = std::array<Vertex, n>;
template <size_t n> using TexVertices = std::array<TextureVertex, n>;
using QuadVertices = std::array<TextureVertex, 4>;
using VarVertices = std::vector<Vertex>;


// Convert
#define QUADS_TO_TRIANGLES(x) (x / 2 * 3)


////////////////////////////////////////////////////////////////////////////////
/// \class Vertex
/// \ingroup OpenGL
///
/// The vertex class is useful for creating vertex data for OpenGL objects. The
/// following example illustrates its use:
///
/// \code
/// // header
/// class Triangle : public IRenderable, public ITransformable
/// {
/// private:
///
///     priv::Vertices<3> m_vertices;
///     QPointF m_pointA;
///
///     ...
/// };
///
/// // source
/// void Triangle::setPosition(const QPointF& pos)
/// {
///     m_vertices[0].xyz(
///             m_pointA.x() + pos.x(),
///             m_pointA.y() + pos.y()
///             );
///
///     ...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_PRIV_NAMESPACE


#endif

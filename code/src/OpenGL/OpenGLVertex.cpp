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


// Cranberry headers
#include <Cranberry/OpenGL/OpenGLVertex.hpp>


CRANBERRY_USING_NAMESPACE


priv::Vertex::Vertex()
    : m_fields({ 0, 0, 0, 0, 0, 0, 1 })
{
}


void priv::Vertex::xyz(float x, float y, float z)
{
    m_fields[0] = x;
    m_fields[1] = y;
    m_fields[2] = z;
}


void priv::Vertex::xyz(const QVector3D& pos)
{
    xyz(pos.x(), pos.y(), pos.z());
}


void priv::Vertex::rgba(float r, float g, float b, float a)
{
    m_fields[3] = r;
    m_fields[4] = g;
    m_fields[5] = b;
    m_fields[6] = a;
}


void priv::Vertex::rgba(const QColor& color)
{
    rgba(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}


const float* priv::Vertex::data() const
{
    return m_fields.data();
}


priv::TextureVertex::TextureVertex()
    : m_fields({ 0, 0, 0, 0, 0, 0, 0, 0, 1 })
{
}


void priv::TextureVertex::xyz(float x, float y, float z)
{
    m_fields[0] = x;
    m_fields[1] = y;
    m_fields[2] = z;
}


void priv::TextureVertex::xyz(const QVector3D& pos)
{
    xyz(pos.x(), pos.y(), pos.z());
}


void priv::TextureVertex::rgba(float r, float g, float b, float a)
{
    m_fields[5] = r;
    m_fields[6] = g;
    m_fields[7] = b;
    m_fields[8] = a;
}


void priv::TextureVertex::rgba(const QColor& color)
{
    rgba(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}


void priv::TextureVertex::uv(float u, float v)
{
    m_fields[3] = u;
    m_fields[4] = v;
}


void priv::TextureVertex::uv(const QVector2D& coords)
{
    uv(coords.x(), coords.y());
}


const float* priv::TextureVertex::data() const
{
    return m_fields.data();
}

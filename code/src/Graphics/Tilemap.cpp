////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt 5.8 framework.
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
#include <Cranberry/Graphics/Tilemap.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/OpenGL/OpenGLShader.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Texture could not be created.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Vertex buffer could not be created.")


Tilemap::Tilemap()
    : m_texture(nullptr)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_setWidth(0)
    , m_setHeight(0)
    , m_mapWidth(0)
    , m_mapHeight(0)
    , m_currentX(0)
    , m_currentY(0)
    , m_update(false)
{
}


Tilemap::~Tilemap()
{
    destroy();
}


bool Tilemap::isNull() const
{
    return RenderBase::isNull() ||
           m_vertices.empty()   ||
           m_texture == nullptr ||
          !m_texture->isCreated();
}


bool Tilemap::create(
    const QString& tileset,
    const QSize& tileSize,
    const QSize& mapSize,
    const QRect& view,
    Window* rt
    )
{
    if (!RenderBase::create(rt)) return false;

    // Attempts to create the texture.
    m_texture = new QOpenGLTexture(QImage(tileset));
    if (!m_texture->create())
    {
        return cranError(ERRARG(e_01));
    }

    // Attempts to create the vertex buffer.
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_vertexBuffer->create() || !m_vertexBuffer->bind())
    {
        return cranError(ERRARG(e_02));
    }

    // Specifies all members.
    m_tileWidth  = static_cast<uint>(tileSize.width());
    m_tileHeight = static_cast<uint>(tileSize.height());
    m_setWidth   = static_cast<uint>(m_texture->width()  / m_tileWidth);
    m_setHeight  = static_cast<uint>(m_texture->height() / m_tileHeight);
    m_mapWidth   = static_cast<uint>(mapSize.width());
    m_mapHeight  = static_cast<uint>(mapSize.height());
    m_view = view;

    // Allocates the correct amount of vertices for the tiles.
    m_vertexBuffer->allocate(m_mapWidth * m_mapHeight * 6 * priv::MapVertex::size());

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    setSize(m_mapWidth * m_tileWidth, m_mapHeight * m_tileHeight);
    setOrigin(width() / 2, height() / 2);

    return true;
}


void Tilemap::destroy()
{
    delete m_texture;
    delete m_vertexBuffer;

    m_texture = nullptr;
    m_vertexBuffer = nullptr;

    RenderBase::destroy();
}


void Tilemap::update(const GameTime& time)
{
    updateTransform(time);
}


void Tilemap::render()
{
    if (!prepareRendering())
    {
        return;
    }

    bindObjects();
    writeVertices();
    modifyProgram();
    modifyAttribs();
    drawElements();
    releaseObjects();
}


void Tilemap::bindObjects()
{
    // Binds the texture to unit 0.
    glDebug(gl->glActiveTexture(GL_TEXTURE0));
    glDebug(m_texture->bind());

    glDebug(m_vertexBuffer->bind());
    glDebug(shaderProgram()->bind());
}


void Tilemap::releaseObjects()
{
    glDebug(m_texture->release());
    glDebug(m_vertexBuffer->release());
    glDebug(shaderProgram()->release());
}


void Tilemap::writeVertices()
{
    if (m_update)
    {
        glDebug(m_vertexBuffer->write(
            GL_ZERO,
            m_vertices.data(),
            m_vertices.size() * priv::MapVertex::size())
            );

        m_update = false;
    }
}


void Tilemap::modifyProgram()
{
    OpenGLShader* program = shaderProgram();

    glDebug(program->setSampler(GL_TEXTURE0));
    glDebug(program->setMvpMatrix(matrix(this)));
    glDebug(program->setOpacity(opacity()));
    glDebug(program->setWindowSize(renderTarget()->size()));
}


void Tilemap::modifyAttribs()
{
    glDebug(gl->glEnableVertexAttribArray(priv::MapVertex::xyAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::MapVertex::uvAttrib()));

    glDebug(gl->glVertexAttribPointer(
                priv::MapVertex::xyAttrib(),
                priv::MapVertex::xyLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::MapVertex::size(),
                priv::MapVertex::xyOffset()
                ));

    glDebug(gl->glVertexAttribPointer(
                priv::MapVertex::uvAttrib(),
                priv::MapVertex::uvLength(),
                GL_FLOAT,
                GL_FALSE,
                priv::MapVertex::size(),
                priv::MapVertex::uvOffset()
                ));
}


void Tilemap::drawElements()
{
    // Renders all tiles at once.
    glDebug(gl->glDrawArrays(
                GL_TRIANGLES,
                GL_ZERO,
                m_vertices.size()
                ));
}


bool Tilemap::setTiles(const QVector<uint>& tiles)
{
    removeAllTiles();

    for (uint index : tiles)
    {
        if (!appendTile(index))
        {
            return false;
        }
    }

    return true;
}


bool Tilemap::appendTile(uint tileIndex)
{
    if (m_currentX >= m_mapWidth)
    {
        m_currentX = 0;
        m_currentY++;
    }

    if (m_currentY >= m_mapHeight)
    {
        // Out of map bounds.
        return false;
    }

    priv::MapVertex t11, t12, t13, t21, t22, t23;

    // Calculates the tile position from the tile index.
    float uvX = ((tileIndex % m_setWidth) * m_tileWidth)  / (qreal) m_texture->width();
    float uvY = ((tileIndex / m_setWidth) * m_tileHeight) / (qreal) m_texture->height();
    float uvW = uvX + ((qreal) m_tileWidth  / m_texture->width());
    float uvH = uvY + ((qreal) m_tileHeight / m_texture->height());
    float xyX = m_currentX * m_tileWidth;
    float xyY = m_currentY * m_tileHeight;
    float xyW = xyX + m_tileWidth;
    float xyH = xyY + m_tileHeight;

    // Modifies the vertices.
    t11.xy(xyX, xyY); t11.uv(uvX, uvY);
    t12.xy(xyW, xyY); t12.uv(uvW, uvY);
    t13.xy(xyX, xyH); t13.uv(uvX, uvH);
    t21.xy(xyX, xyH); t21.uv(uvX, uvH);
    t22.xy(xyW, xyY); t22.uv(uvW, uvY);
    t23.xy(xyW, xyH); t23.uv(uvW, uvH);

    m_vertices.push_back(t11);
    m_vertices.push_back(t12);
    m_vertices.push_back(t13);
    m_vertices.push_back(t21);
    m_vertices.push_back(t22);
    m_vertices.push_back(t23);

    m_currentX++;
    m_update = true;

    return true;
}


void Tilemap::removeAllTiles()
{
    m_currentX = 0;
    m_currentY = 0;
    m_vertices.clear();
    m_update = true;
}

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
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Texture could not be created.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Vertex buffer could not be created.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Only up to 10 tilesets are supported.")


Tilemap::Tilemap()
    : m_tileWidth(0)
    , m_tileHeight(0)
    , m_mapWidth(0)
    , m_mapHeight(0)
    , m_currentX(0)
    , m_currentY(0)
    , m_update(false)
    , m_ownTextures(true)
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
           m_textures.empty();
}


bool Tilemap::create(
    const QVector<QString>& tilesets,
    const QSize& tileSize,
    const QSize& mapSize,
    const QRect& view,
    Window* rt
    )
{
    // Specifies all members.
    m_tileWidth  = static_cast<int>(tileSize.width());
    m_tileHeight = static_cast<int>(tileSize.height());
    m_mapWidth   = static_cast<int>(mapSize.width());
    m_mapHeight  = static_cast<int>(mapSize.height());
    m_view = view;

    if (!createInternal(rt)) return false;

    // We now have an active context; create texture.
    for (const QString& path : tilesets)
    {
        QImage img(path);
        if (img.isNull())
        {
            return cranError(ERRARG(e_01));
        }

        m_textures.append(new QOpenGLTexture(img));
    }

    return getUniformLocations();
}


bool Tilemap::create(
    const QVector<QOpenGLTexture*>& textures,
    const QSize& tileSize,
    const QSize& mapSize,
    const QRect& view,
    Window* rt
    )
{
    if (textures.size() > TILEMAP_MAX_SETS)
    {
        return cranError(e_03);
    }

    // Specifies all members.
    m_tileWidth  = static_cast<int>(tileSize.width());
    m_tileHeight = static_cast<int>(tileSize.height());
    m_mapWidth   = static_cast<int>(mapSize.width());
    m_mapHeight  = static_cast<int>(mapSize.height());
    m_ownTextures = false;
    m_textures = textures;
    m_view = view;

    return createInternal(rt) && getUniformLocations();
}


bool Tilemap::createInternal(Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    // Attempts to create the vertex buffer.
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_vertexBuffer->create() || !m_vertexBuffer->bind())
    {
        return cranError(ERRARG(e_02));
    }

    m_vertexBuffer->allocate(m_mapWidth * m_mapHeight * 6 * priv::MapVertex::size());

    // Attempts to create the sampler buffer.
    m_textureBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!m_textureBuffer->create() || !m_textureBuffer->bind())
    {
        return cranError(ERRARG(e_02));
    }

    m_textureBuffer->allocate(m_mapWidth * m_mapHeight * 6 * sizeof(int));

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.tilemap"));
    setSize(m_mapWidth * m_tileWidth, m_mapHeight * m_tileHeight);
    setOrigin(width() / 2, height() / 2);

    return true;
}


bool Tilemap::getUniformLocations()
{
    m_uniformLocs.clear();

    OpenGLShader* program = shaderProgram();
    for (int i = 0; i < m_textures.size(); i++)
    {
        m_uniformLocs.append(program->uniformLocation("u_set" + QString::number(i)));
    }

    return true;
}


void Tilemap::destroy()
{
    if (m_ownTextures)
    {
        for (QOpenGLTexture* t : m_textures)
        {
            delete t;
        }
    }

    delete m_vertexBuffer;
    delete m_textureBuffer;

    m_vertexBuffer = nullptr;
    m_textureBuffer = nullptr;

    m_ids.clear();
    m_vertices.clear();
    m_textures.clear();

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
    // Binds the texture to the units.
    for (int i = 0; i < m_textures.size(); i++)
    {
        glDebug(gl->glActiveTexture(GL_TEXTURE0 + i));
        glDebug(gl->glBindTexture(GL_TEXTURE_2D, m_textures.at(i)->textureId()));
    }

    glDebug(shaderProgram()->bind());
}


void Tilemap::releaseObjects()
{
    // Unbinds the texture to the units.
    for (int i = 0; i < m_textures.size(); i++)
    {
        glDebug(gl->glActiveTexture(GL_TEXTURE0 + i));
        glDebug(gl->glBindTexture(GL_TEXTURE_2D, GL_ZERO));
    }

    glDebug(shaderProgram()->release());
}


void Tilemap::writeVertices()
{
    if (m_update)
    {
        glDebug(m_vertexBuffer->bind());
        glDebug(m_vertexBuffer->write(
            GL_ZERO,
            m_vertices.data(),
            m_vertices.size() * priv::MapVertex::size())
            );

        glDebug(m_textureBuffer->bind());
        glDebug(m_textureBuffer->write(
            GL_ZERO,
            m_ids.data(),
            m_ids.size() * sizeof(int))
            );

        m_update = false;
    }
}


void Tilemap::modifyProgram()
{
    OpenGLShader* program = shaderProgram();

    for (int i = 0; i < m_textures.size(); i++)
    {
        glDebug(program->setUniformValue(m_uniformLocs.at(i), i));
    }

    glDebug(program->setMvpMatrix(matrix(this)));
    glDebug(program->setOpacity(opacity()));
}


void Tilemap::modifyAttribs()
{
    glDebug(gl->glEnableVertexAttribArray(priv::MapVertex::xyAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::MapVertex::uvAttrib()));
    glDebug(gl->glEnableVertexAttribArray(priv::MapVertex::idAttrib()));

    glDebug(m_vertexBuffer->bind());
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

    glDebug(m_textureBuffer->bind());
    glDebug(gl->glVertexAttribPointer(
                priv::MapVertex::idAttrib(),
                priv::MapVertex::idLength(),
                GL_FLOAT,
                GL_FALSE,
                GL_ZERO,
                priv::MapVertex::idOffset()
                ));
}


void Tilemap::drawElements()
{
    // For really big maps, it is way better to split up the work in order to
    // only draw as many vertices as actually needed for the entire window.

    // Calculates the carry and the max tiles on screen.
    int carryX = static_cast<int>(-x() / m_tileWidth);
    int carryY = static_cast<int>(-y() / m_tileHeight);
    int screenX = static_cast<int>(renderTarget()->width() / m_tileWidth);
    int screenY = static_cast<int>(renderTarget()->height() / m_tileHeight);

    // Calculates the visible parts of the map on screen.
    int visibleX = qMax(0, carryX);
    int visibleY = qMax(0, carryY);
    int visibleW = qMin(m_mapWidth  - visibleX, screenX) + 1;
    int visibleH = qMin(m_mapHeight - visibleY, screenY) + 1;

    for (; visibleY < visibleH; visibleY++)
    {
        int start = 6 * (visibleY * m_mapWidth + visibleX);
        int count = 6 * visibleW;
        glDebug(gl->glDrawArrays(GL_TRIANGLES, start, count));
    }
}


bool Tilemap::setTiles(const QVector<QPair<int, int>>& tiles)
{
    removeAllTiles();

    for (const auto& t : tiles)
    {
        if (!appendTile(t.first, t.second))
        {
            return false;
        }
    }

    return true;
}


bool Tilemap::appendTile(int tileIndex, int tileset)
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
    QOpenGLTexture* const tex = m_textures.at(tileset);

    // Calculates the tile position from the tile index.
    int swid = tex->width()  / m_tileWidth;
    float uvX = ((tileIndex % swid) * m_tileWidth)  / (qreal) tex->width();
    float uvY = ((tileIndex / swid) * m_tileHeight) / (qreal) tex->height();
    float uvW = uvX + ((qreal) m_tileWidth  / tex->width());
    float uvH = uvY + ((qreal) m_tileHeight / tex->height());
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

    for (int i = 0; i < 6; i++)
    {
        m_ids.push_back(tileset);
    }

    m_currentX++;
    m_update = true;

    return true;
}


void Tilemap::appendNullTile()
{
    if (m_currentX >= m_mapWidth)
    {
        m_currentX = 0;
        m_currentY++;
    }

    for (int i = 0; i < 6; i++)
    {
        m_vertices.push_back(priv::MapVertex());
        m_ids.push_back(0);
    }

    m_currentX++;
}


void Tilemap::removeAllTiles()
{
    m_currentX = 0;
    m_currentY = 0;
    m_vertices.clear();
    m_ids.clear();
    m_update = true;
}

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


#pragma once
#ifndef CRANBERRY_GRAPHICS_BASE_SHAPEBASE_HPP
#define CRANBERRY_GRAPHICS_BASE_SHAPEBASE_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/Enumerations.hpp>
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/OpenGL/OpenGLVertex.hpp>

// Qt headers
#include <QVector>

// Forward declarations
CRANBERRY_FORWARD_Q(QOpenGLBuffer)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Defines the base class for rendering primitives.
///
/// \class ShapeBase
/// \author Nicolas Kogler
/// \date July 5, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GRAPHICS_EXPORT ShapeBase : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(ShapeBase)
    CRANBERRY_DECLARE_DTOR(ShapeBase)
    CRANBERRY_DISABLE_COPY(ShapeBase)
    CRANBERRY_DISABLE_MOVE(ShapeBase)

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the amount of vertices for this object.
    ///
    /// \returns the amount of vertices.
    ///
    ////////////////////////////////////////////////////////////////////////////
    uint vertexCount() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this shape is filled.
    ///
    /// \returns true if filled.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isShapeFilled() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this shape is smooth.
    ///
    /// \returns true if smooth.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isSmooth() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the line width in pixels. Guaranteed to be at least 1.
    ///
    /// \returns the line width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int lineWidth() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders this primitive filled or wired.
    ///
    /// \param filled True to fill primitive.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setShapeFilled(bool filled);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies whether this shape should be smoothened (anti-aliased). By
    /// default, this is true.
    ///
    /// \param smooth True to smoothen shape.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setSmooth(bool smooth);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the line width. Must be at least 1.
    ///
    /// \param width New line width.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setLineWidth(int width);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the color that will be applied on this object.
    ///
    /// \param color Color to use for blending.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setColor(const QColor& color);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the colors that will be applied on this object. Note that the
    /// amount of
    ///
    /// \param tl Top left vertex.
    /// \param tr Top right vertex.
    /// \param br Bottom right vertex.
    /// \param bl Bottom left vertex.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setColor(const QVector<QColor>& colors);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Reimplements IRenderable::isNull(). Adds the condition that the
    /// vertex buffer is valid.
    ///
    /// \returns true if the object is invalid.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all OpenGL resources allocated for this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the transformations of this object.
    ///
    /// \param time Contains the delta time.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders this object.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void render() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual void updateProperties() override;


protected overridable:

    virtual uint renderModeWired() const = 0;
    virtual uint renderModeFilled() const = 0;
    virtual bool isShapeClosed() const = 0;

    bool createInternal(const QVector<QPointF>& points, Window* renderTarget);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    bool createBuffer();
    auto findCenter(const QVector<QPointF>&) -> QPointF;
    auto findSize(const QVector<QPointF>&) -> QPointF;
    void bindObjects();
    void releaseObjects();
    void writeVertices();
    void modifyProgram();
    void modifyAttribs();
    void drawElements();
    void extrudeSegment(
            const QPointF& p0,
            const QPointF& p1,
            const QPointF& p2,
            const QPointF& p3
            );

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem*    m_rootModelItem;
    priv::VarVertices m_vertices;
    QOpenGLBuffer*    m_vertexBuffer;
    QVector<QColor>   m_colorBuffer;
    QVector<QPointF>  m_points;
    int               m_lineWidth;
    bool              m_filled;
    bool              m_colorUpdate;
    bool              m_smooth;
    bool              m_update;
};


////////////////////////////////////////////////////////////////////////////////
/// \class ShapeBase
/// \ingroup Graphics
///
/// This class is the base for all shape objects. The renderModeWired() and the
/// renderModeFilled() expect values like GL_LINES, GL_TRIANGLES and so on.
///
/// \code
/// class Polygon : public ShapeBase
/// {
/// public:
///
///     bool create(...);
///
///
/// protected:
///
///    uint renderModeWired() const override;
///    uint renderModeFilled() const override;
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

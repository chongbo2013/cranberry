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
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/Gui/Base/Widget.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>

// Qt headers

// Standard headers


CRANBERRY_USING_NAMESPACE


Widget::Widget(Widget* parent)
    : IRenderable()
    , ITransformable()
    , m_widget(new QWidget((parent) ? parent->m_widget : nullptr))
    , m_batch(new SpriteBatch)
{
    m_widget->setAttribute(Qt::WA_DontShowOnScreen);

    setParent(parent);
}


Widget::~Widget()
{
    destroy();

    delete m_widget;
}


bool Widget::isNull() const
{
    return IRenderable::isNull() || m_batch->isNull();
}


bool Widget::create(Window* renderTarget)
{
    if (!IRenderable::create(renderTarget)) return false;

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    m_widget->show();

    return m_batch->create(renderTarget);
}


void Widget::destroy()
{
    if (m_batch != nullptr)
    {
        delete m_batch;
        m_batch = nullptr;
    }

    if (m_parent != nullptr)
    {
        m_parent->m_children.removeOne(this);
    }

    IRenderable::destroy();
}


void Widget::update(const GameTime& time)
{
    updateTransform(time);

    m_batch->setShaderProgram(shaderProgram());
    m_batch->setAngle(angle());
    m_batch->setOpacity(opacity());
    m_batch->setOrigin(origin().toVector2D());
    m_batch->setScale(scaleX(), scaleY());
}


void Widget::render()
{
    const QRect& g = m_widget->geometry();

    // Determines whether layout must be updated.
    if (g.x() != x() || g.y() != y())
    {
        setPosition(g.x(), g.y());
        m_batch->setPosition(g.x(), g.y());
    }

    if (g.width() != width() || g.height() != height())
    {
        onSizeChanged(g.size());
        setSize(g.width(), g.height());
        m_batch->setGeometry(rect());
    }

    // Renders the batch and all child widgets.
    m_batch->render();

    for (Widget* w : m_children)
    {
        w->render();
    }
}


QWidget* Widget::toQtWidget() const
{
    return m_widget;
}


QLayout* Widget::layout() const
{
    return m_widget->layout();
}


Widget* Widget::parent() const
{
    return m_parent;
}


const QColor& Widget::backgroundColor() const
{
    return m_batch->backgroundColor();
}


void Widget::setLayout(QLayout* layout)
{
    m_widget->setLayout(layout);
}


void Widget::setParent(Widget* parent)
{
    m_parent = parent;

    if (parent != nullptr)
    {
        parent->m_children.append(this);
    }
}


void Widget::setBackgroundColor(const QColor& color)
{
    m_batch->setBackgroundColor(color);
}


bool Widget::addObjectToBatch(IRenderable* obj)
{
    return m_batch->addObject(obj);
}


bool Widget::insertObjectToBatch(int layer, IRenderable* obj)
{
    return m_batch->insertObject(layer, obj);
}


bool Widget::removeObjectFromBatch(IRenderable* obj)
{
    return m_batch->removeObject(obj);
}

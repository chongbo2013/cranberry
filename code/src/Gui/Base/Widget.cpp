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


CRANBERRY_USING_NAMESPACE


// Interval tolerances
#define TLRN_MOVE   0.1
#define TLRN_ROTATE 0.01
#define TLRN_SCALE  0.01
#define TLRN_FADE   0.001


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

    const QRect& g = m_widget->geometry();

    // Determines whether layout must be updated.
    if (g.width() != m_oldGeo.width() || g.height() != m_oldGeo.height())
    {
        onSizeChanged(g.size());
        setSize(g.width(), g.height());

        m_oldGeo.setWidth(g.width());
        m_oldGeo.setHeight(g.height());
        m_batch->setGeometry(rect());
    }

    // Copies all properties to the batch.
    m_batch->setPosition(getX(), getY());
    m_batch->setAngle(getAngleX(), getAngleY(), getAngleZ());
    m_batch->setScale(getScaleX(), getScaleY());
    m_batch->setOpacity(getOpacity());
    m_batch->setShaderProgram(shaderProgram());
    m_batch->setOrigin(origin().toVector2D());
}


void Widget::render()
{
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


float Widget::getX()
{
    auto* current = this;
    float px = 0;

    while (current != nullptr)
    {
        px += (current->x() + current->m_widget->x());
        current = current->m_parent;
    }

    return px;
}


float Widget::getY()
{
    auto* current = this;
    float py = 0;

    while (current != nullptr)
    {
        py += (current->y() + current->m_widget->y());
        current = current->m_parent;
    }

    return py;
}


float Widget::getAngleX()
{
    auto* current = this;
    float px = 0;

    while (current != nullptr)
    {
        px += current->angleX();
        current = current->m_parent;
    }

    return px;
}


float Widget::getAngleY()
{
    auto* current = this;
    float py = 0;

    while (current != nullptr)
    {
        py += current->angleY();
        current = current->m_parent;
    }

    return py;
}


float Widget::getAngleZ()
{
    auto* current = this;
    float pz = 0;

    while (current != nullptr)
    {
        pz += current->angleZ();
        current = current->m_parent;
    }

    return pz;
}


float Widget::getScaleX()
{
    auto* current = this;
    float px = 1;

    while (current != nullptr)
    {
        px *= current->scaleX();
        current = current->m_parent;
    }

    return px;
}


float Widget::getScaleY()
{
    auto* current = this;
    float py = 1;

    while (current != nullptr)
    {
        py *= current->scaleY();
        current = current->m_parent;
    }

    return py;
}


float Widget::getOpacity()
{
    auto* current = this;
    float po = 1;

    while (current != nullptr)
    {
        po *= current->opacity();
        current = current->m_parent;
    }

    return po;
}

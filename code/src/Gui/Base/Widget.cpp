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
    : QOpenGLWidget(parent->m_widget)
    , IRenderable()
    , ITransformable()
    , m_widget(new QOpenGLWidget)
    , m_batch(new SpriteBatch)
{
    m_widget->setAttribute(Qt::WA_DontShowOnScreen);

    QObject::connect(
            transformableEmitter(),
            &TransformableEmitter::positionChanged,
            this,
            &Widget::posChanged
            );

    QObject::connect(
            transformableEmitter(),
            &TransformableEmitter::sizeChanged,
            this,
            &Widget::sizeChanged
            );
}


Widget::~Widget()
{
    destroy();
}


bool Widget::isNull() const
{
    return IRenderable::isNull() || m_batch->isNull();
}


bool Widget::create(Window* renderTarget)
{
    if (!IRenderable::create(renderTarget)) return false;

    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));

    return m_batch->create(renderTarget);
}


void Widget::destroy()
{
    if (m_batch != nullptr)
    {
        delete m_batch;
        m_batch = nullptr;
    }

    IRenderable::destroy();
}


void Widget::update(const GameTime& time)
{
    updateTransform(time);

    m_batch->setShaderProgram(shaderProgram());
    m_batch->setPosition(pos());
    m_batch->setAngle(angle());
    m_batch->setOpacity(opacity());
    m_batch->setOrigin(origin().toVector2D());
    m_batch->setScale(scaleX(), scaleY());
}


void Widget::render()
{
    m_batch->setPosition(m_widget->x(), m_widget->y());
    m_batch->setSize(m_widget->width(), m_widget->height());
    m_batch->render();

    // Renders all child widgets.
    auto list = m_widget->findChildren<Widget*>(QString(), Qt::FindDirectChildrenOnly);
    for (auto* w : list)
    {
        w->render();
    }
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


void Widget::posChanged()
{
    QPointF p = ITransformable::rect().topLeft();
    m_widget->move(QPoint(p.x(), p.y()));
}


void Widget::sizeChanged()
{
    QSizeF s = ITransformable::rect().size();
    m_widget->resize(QSize(s.width(), s.height()));
    onSizeChanged(size());
}

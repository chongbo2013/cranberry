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
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/Gui/GuiManager.hpp>
#include <Cranberry/OpenGL/OpenGLDebug.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/System/Debug.hpp>
#include <Cranberry/System/Models/TreeModel.hpp>
#include <Cranberry/Window/Window.hpp>

// Qt headers
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QQuickWindow>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - Qml root item is invalid.")


CRANBERRY_USING_NAMESPACE


GuiManager::GuiManager()
    : m_batch(new SpriteBatch)
    , m_offscreenSurface(new QOffscreenSurface)
    , m_renderControl(new QQuickRenderControl)
    , m_renderWindow(new QQuickWindow(m_renderControl))
    , m_qmlEngine(new QQmlEngine)
    , m_qmlComponent(nullptr)
    , m_rootItem(nullptr)
    , m_fbo(nullptr)
    , m_requiresUpdate(false)
    , m_isInitialized(false)
    , m_isVisible(true)
    , m_noKeyInput(false)
{
    if (!m_qmlEngine->incubationController())
         m_qmlEngine->setIncubationController(m_renderWindow->incubationController());

    m_receiver.setGuiManager(this);
    m_renderWindow->setClearBeforeRendering(false);
    m_renderWindow->setMouseGrabEnabled(true);
    m_renderWindow->create();

    // Signals & slots
    QObject::connect(
        m_renderControl,
        &QQuickRenderControl::renderRequested,
        &m_receiver,
        &GuiManagerReceiver::requestUpdate
        );

    QObject::connect(
        m_renderControl,
        &QQuickRenderControl::sceneChanged,
        &m_receiver,
        &GuiManagerReceiver::requestUpdate
        );
}


GuiManager::~GuiManager()
{
    destroy();

    delete m_batch;
    delete m_offscreenSurface;
    delete m_renderControl;
    delete m_renderWindow;
    delete m_qmlEngine;
}


bool GuiManager::isVisible() const
{
    return m_isVisible;
}


bool GuiManager::isTransparentToKeyInput() const
{
    return m_noKeyInput;
}


bool GuiManager::isTransparentToMouseInput() const
{
    return m_noMouseInput;
}


QQuickWindow* GuiManager::window() const
{
    return m_renderWindow;
}


QQuickItem* GuiManager::rootItem() const
{
    return m_rootItem;
}


QQmlContext* GuiManager::context() const
{
    return m_qmlEngine->rootContext();
}


QPointF GuiManager::topLeft() const
{
    return m_rootItem->position();
}


void GuiManager::setVisible(bool visible)
{
    m_isVisible = visible;
}


void GuiManager::setTransparentToKeyInput(bool transparent)
{
    m_noKeyInput = transparent;
}


void GuiManager::setTransparentToMouseInput(bool transparent)
{
    m_noMouseInput = transparent;
}


void GuiManager::setEffect(Effect effect)
{
    m_batch->setEffect(effect);
}


bool GuiManager::create(const QString& qml, Window* rt)
{
    if (!RenderBase::create(rt)) return false;

    // Tries to create the surface.
    m_offscreenSurface->setFormat(renderTarget()->context()->format());
    m_offscreenSurface->create();

    // Loads the Qml file.
    m_qmlComponent = new QQmlComponent(m_qmlEngine, QUrl(qml));
    if (m_qmlComponent->isLoading())
    {
        QObject::connect(
            m_qmlComponent,
            &QQmlComponent::statusChanged,
            &m_receiver,
            &GuiManagerReceiver::loadComponents
            );
    }
    else
    {
        loadComponents();
    }

    renderTarget()->registerQmlWindow(this);
    setDefaultShaderProgram(OpenGLDefaultShaders::get("cb.glsl.texture"));
    requestUpdate();

    return true;
}


bool GuiManager::isNull() const
{
    return RenderBase::isNull()      ||
           m_batch->isNull()         ||
           m_qmlComponent == nullptr ||
           m_qmlComponent->isNull()  ||
           m_rootItem == nullptr     ||
           m_fbo == nullptr          ||
          !m_isInitialized           ||
          !m_fbo->isValid();
}


void GuiManager::destroy()
{
    makeCurrent();
    renderTarget()->unregisterQmlWindow(this);

    delete m_fbo;
    delete m_qmlComponent;
    delete m_rootItem;

    m_fbo = nullptr;
    m_qmlComponent = nullptr;
    m_rootItem = nullptr;

    m_batch->destroy();
}


void GuiManager::update(const GameTime& time)
{
    updateTransform(time);

    // Copies all transformations.
    m_batch->setShaderProgram(shaderProgram());
    m_batch->copyTransform(this, m_batch);
}


void GuiManager::render()
{
    makeCurrent();
    clearFbo();

    if (m_requiresUpdate)
    {
        m_renderControl->polishItems();
        m_renderControl->sync();
        m_renderControl->render();
        m_requiresUpdate = false;
    }
    else
    {
        m_renderControl->render();
    }

    if (RenderBase::prepareRendering())
    {
        // Needs to pass OS renderer through.
        renderTarget()->restoreOpenGLSettings();
        m_batch->setOffscreenRenderer(offscreenRenderer());
        m_batch->render();
    }
}


TreeModelItem* GuiManager::rootModelItem()
{
    return m_rootModelItem;
}


void GuiManager::createProperties(TreeModel* model)
{
    m_batch->createProperties(nullptr);

    TreeModelItem* tmiInit = new TreeModelItem("Is initialized?", m_isInitialized);
    TreeModelItem* tmiVisi = new TreeModelItem("Is visible?", m_isVisible);
    TreeModelItem* tmiKeyi = new TreeModelItem("Allow key input?", !m_noKeyInput);
    TreeModelItem* tmiUpda = new TreeModelItem("Requires update?", m_requiresUpdate);
    TreeModelItem* tmiGFbo = new TreeModelItem("Qml frame buffer", m_fbo->handle());

    m_rootModelItem = new TreeModelItem("GuiManager");
    m_rootModelItem->appendChild(tmiInit);
    m_rootModelItem->appendChild(tmiVisi);
    m_rootModelItem->appendChild(tmiKeyi);
    m_rootModelItem->appendChild(tmiUpda);
    m_rootModelItem->appendChild(tmiGFbo);
    m_rootModelItem->appendChild(m_batch->rootModelItem());

    model->addItem(m_rootModelItem);

    RenderBase::createProperties(model);
}


void GuiManager::updateProperties()
{
    m_batch->updateProperties();

    m_rootModelItem->childAt(0)->setValue(m_isInitialized);
    m_rootModelItem->childAt(1)->setValue(m_isVisible);
    m_rootModelItem->childAt(2)->setValue(!m_noKeyInput);
    m_rootModelItem->childAt(3)->setValue(m_requiresUpdate);
    m_rootModelItem->childAt(4)->setValue(m_fbo->handle());

    RenderBase::updateProperties();
}


void GuiManager::makeCurrent()
{
    auto* ct = renderTarget()->context();
    auto* cc = QOpenGLContext::currentContext();

    // Also proceeds if surfaces are different.
    if (cc != ct || cc->surface() != ct->surface())
    {
        ct->makeCurrent(m_offscreenSurface);
    }
}


void GuiManager::loadComponents()
{
    QObject::disconnect(
        m_qmlComponent,
        &QQmlComponent::statusChanged,
        &m_receiver,
        &GuiManagerReceiver::loadComponents
        );

    if (m_qmlComponent->isError())
    {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        for (const QQmlError &error : errorList)
            qWarning() << error.url() << error.line() << error;
    }
    else
    {
        m_rootItem = qobject_cast<QQuickItem*>(m_qmlComponent->create());
        if (m_qmlComponent->isError())
        {
            const QList<QQmlError> errorList = m_qmlComponent->errors();
            for (const QQmlError &error : errorList)
                qWarning() << error.url() << error.line() << error;
        }
        else if (m_rootItem == nullptr)
        {
            cranError(ERRARG(e_01));
        }
        else
        {
            m_rootItem->setAcceptHoverEvents(true);
            m_rootItem->setParentItem(m_renderWindow->contentItem());
            m_renderWindow->setGeometry(0, 0, m_rootItem->width(), m_rootItem->height());

            createFbo();
            makeCurrent();

            // Hilarious hack to trick QSGRenderContext xD
            renderTarget()->context()->setProperty("_q_sgrendercontext", QVariant());

            m_renderControl->initialize(renderTarget()->context());
            m_isInitialized = true;

            setSize(m_rootItem->width(), m_rootItem->height());
            setOrigin(width() / 2, height() / 2);
        }
    }
}


void GuiManager::clearFbo()
{
    if (m_fbo != nullptr)
    {
        glDebug(m_fbo->bind());
        glDebug(gl->glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        glDebug(gl->glClear(GL_COLOR_BUFFER_BIT));
        glDebug(m_fbo->release());
    }
}


void GuiManager::createFbo()
{
    makeCurrent();

    // Creates the fbo and binds it to the window.
    m_fbo = new QOpenGLFramebufferObject(
                m_rootItem->width(),
                m_rootItem->height(),
                QOpenGLFramebufferObject::CombinedDepthStencil
                );

    m_renderWindow->setRenderTarget(m_fbo);

    // React to view changes.
    QObject::connect(
        m_renderWindow,
        &QQuickWindow::widthChanged,
        &m_receiver,
        &GuiManagerReceiver::resizeFbo
        );

    QObject::connect(
        m_renderWindow,
        &QQuickWindow::heightChanged,
        &m_receiver,
        &GuiManagerReceiver::resizeFbo
        );

    // Create the underlying sprite batch.
    m_batch->create(m_fbo, renderTarget());
}


void GuiManager::resizeFbo()
{
    if (m_rootItem)
    {
        makeCurrent();
        m_batch->destroy();
        createFbo();
    }
}


void GuiManager::requestUpdate()
{
    m_requiresUpdate = true;
}

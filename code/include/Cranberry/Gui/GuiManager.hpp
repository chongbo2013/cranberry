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
#ifndef CRANBERRY_GUIMANAGER_HPP
#define CRANBERRY_GUIMANAGER_HPP


// Cranberry headers
#include <Cranberry/Graphics/Base/RenderBase.hpp>
#include <Cranberry/System/Receivers/GuiManagerReceiver.hpp>


// Forward declarations
CRANBERRY_FORWARD_C(SpriteBatch)
CRANBERRY_FORWARD_Q(QQmlComponent)
CRANBERRY_FORWARD_Q(QQmlEngine)
CRANBERRY_FORWARD_Q(QQuickItem)
CRANBERRY_FORWARD_Q(QQuickRenderControl)
CRANBERRY_FORWARD_Q(QQuickWindow)
CRANBERRY_FORWARD_Q(QOffscreenSurface)
CRANBERRY_FORWARD_Q(QOpenGLContext)
CRANBERRY_FORWARD_Q(QOpenGLFramebufferObject)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// Loads, updates and renders an user interface based on Qml.
///
/// \class GuiManager
/// \author Nicolas Kogler
/// \date July 18, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GUI_EXPORT GuiManager : public RenderBase
{
public:

    CRANBERRY_DISABLE_COPY(GuiManager)
    CRANBERRY_DISABLE_MOVE(GuiManager)

    ////////////////////////////////////////////////////////////////////////////
    /// Initializes a new instance of GuiManager.
    ///
    ////////////////////////////////////////////////////////////////////////////
    GuiManager();

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys all allocated resources for this GuiManager.
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~GuiManager();


    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this GuiManager is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new GuiManager that manages the given Qml Gui.
    ///
    /// \param qml Path to Qml file.
    /// \param renderTarget Target to render Gui on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& qml, Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// Destroys the Qml Gui.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void destroy() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the transformations.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void update(const GameTime& time) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Renders the Qml Gui.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void render() override;


    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);


    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the underlying QQuickWindow.
    ///
    /// \returns the underlying window.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QQuickWindow* window() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the root QQuickItem.
    ///
    /// \returns the root item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QQuickItem* rootItem() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the top-left position of the root item.
    ///
    /// \returns the top-left pos of the root item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QPointF topLeft() const;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////////
    void makeCurrent();
    void loadComponents();
    void clearFbo();
    void createFbo();
    void resizeFbo();
    void requestUpdate();

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    GuiManagerReceiver        m_receiver;
    SpriteBatch*              m_batch;
    QOffscreenSurface*        m_offscreenSurface;
    QQuickRenderControl*      m_renderControl;
    QQuickWindow*             m_renderWindow;
    QQmlEngine*               m_qmlEngine;
    QQmlComponent*            m_qmlComponent;
    QQuickItem*               m_rootItem;
    QOpenGLFramebufferObject* m_fbo;
    bool                      m_requiresUpdate;
    bool                      m_isInitialized;
    bool                      m_isReady;

    friend class GuiManagerReceiver;
};


////////////////////////////////////////////////////////////////////////////////
/// \class GuiManager
/// \ingroup Gui
///
/// More detailed description, code examples.
///
/// \code
/// ...
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

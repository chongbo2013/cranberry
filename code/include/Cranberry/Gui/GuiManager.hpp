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
CRANBERRY_FORWARD_P(WindowPrivate)
CRANBERRY_FORWARD_Q(QQmlContext)
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
class CRANBERRY_GUI_EXPORT GuiManager final : public RenderBase
{
public:

    CRANBERRY_DECLARE_CTOR(GuiManager)
    CRANBERRY_DECLARE_DTOR(GuiManager)
    CRANBERRY_DISABLE_COPY(GuiManager)
    CRANBERRY_DISABLE_MOVE(GuiManager)

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this GuiManager is visible.
    ///
    /// \returns true if visible.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isVisible() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this Gui is transparent to key input.
    ///
    /// \returns true if transparent.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isTransparentToKeyInput() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this Gui is transparent to mouse input.
    ///
    /// \returns true if transparent.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isTransparentToMouseInput() const;

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
    /// Retrieves the root Qml context.
    ///
    /// \returns the root context.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QQmlContext* context() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the top-left position of the root item.
    ///
    /// \returns the top-left pos of the root item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QPointF topLeft() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the visibility of this Gui. If false is passed, the Gui will
    /// not be focusable from the underlying QOpenGLWindow, i.e. it gets
    /// transparent for input events.
    ///
    /// \param visible Should Gui be visible?
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setVisible(bool visible);

    ////////////////////////////////////////////////////////////////////////////
    /// If this is set to true, the Gui will not receive any key events, they
    /// will be forwarded to the underlying cranberry window instead.
    ///
    /// \param transparent Transparent to key input?
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTransparentToKeyInput(bool transparent);

    ////////////////////////////////////////////////////////////////////////////
    /// If this is set to true, the Gui will behave as "display-only" Gui, i.e.
    /// it cannot be interacted with.
    ///
    /// \param transparent Transparent to mouse input?
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setTransparentToMouseInput(bool transparent);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the effect to render this object with.
    ///
    /// \param effect EffectNone does not modify the image.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setEffect(Effect effect);

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new GuiManager that manages the given Qml Gui.
    ///
    /// \param qml Path to Qml file.
    /// \param renderTarget Target to render Gui on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool create(const QString& qml, Window* renderTarget);


public overridden:

    ////////////////////////////////////////////////////////////////////////////
    /// Determines whether this GuiManager is null.
    ///
    /// \returns true if null.
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool isNull() const override;

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
    /// Retrieves the root model item of this instance.
    ///
    /// \returns the root model item.
    ///
    ////////////////////////////////////////////////////////////////////////////
    TreeModelItem* rootModelItem() override;

    ////////////////////////////////////////////////////////////////////////////
    /// Creates the property items and appends them to the model. Any items
    /// appended to the model are owned by it - no custom deletion required.
    ///
    /// \param model Model to append property items to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void createProperties(TreeModel* model) override;

    ////////////////////////////////////////////////////////////////////////////
    /// Updates the property items. Make sure to have at least an instance of the
    /// root item stored somewhere in the class. If you reimplement this method,
    /// you are able to see your objects change live.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void updateProperties() override;


private:

    ////////////////////////////////////////////////////////////////////////////
    // Functions
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
    TreeModelItem*            m_rootModelItem;
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
    bool                      m_isVisible;
    bool                      m_noKeyInput;
    bool                      m_noMouseInput;

    friend class GuiManagerReceiver;
    friend class priv::WindowPrivate;
};


////////////////////////////////////////////////////////////////////////////////
/// \class GuiManager
/// \ingroup Gui
///
/// The GuiManager class allows you to load, display and interact with Qml GUIs.
/// As of today, there is a bug in which the QQuickWindow - associated with the
/// GuiManager - steals the focus of the main window and greys out the title bar
/// under Windows, for example.
///
/// \code
/// // Creates a "display-only" Gui
/// GuiManager gui;
/// gui.create(":/gui/main.qml");
/// gui.setTransparentToMouseInput(true); // no focus possible
/// ...
/// gui.render();
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

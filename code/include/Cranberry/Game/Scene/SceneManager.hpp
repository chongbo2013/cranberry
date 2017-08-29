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
#ifndef CRANBERRY_GAME_SCENE_SCENEMANAGER_HPP
#define CRANBERRY_GAME_SCENE_SCENEMANAGER_HPP


// Cranberry headers
#include <Cranberry/Game/Scene/Scene.hpp>


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class holds a bunch of scenes.
///
/// \class SceneManager
/// \author Nicolas Kogler
/// \date August 29, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT SceneManager
{
public:

    CRANBERRY_DECLARE_DTOR(SceneManager)
    CRANBERRY_DEFAULT_COPY(SceneManager)
    CRANBERRY_DEFAULT_MOVE(SceneManager)

    ////////////////////////////////////////////////////////////////////////////
    /// Creates a new scene manager with the given render target.
    ///
    /// \param renderTarget Target to render scenes on.
    ///
    ////////////////////////////////////////////////////////////////////////////
    SceneManager(Window* renderTarget);

    ////////////////////////////////////////////////////////////////////////////
    /// Adds the given scene to the manager. It will now receive all events that
    /// the scene manager receives too.
    ///
    /// \param scene Scene to add.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void addScene(Scene* scene);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts the given scene at \p index. It will now receive all events that
    /// the scene manager receives too. This function changes the render order.
    ///
    /// \param index Index to insert scene to.
    /// \param scene Scene to insert.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void insertScene(int index, Scene* scene);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given scene from the manager. It will now not receive any
    /// events anymore. Note: The scene will still remain in the manager, so
    /// that OpenGL resources must not be reinitialized when addScene() on that
    /// scene is used again. If you wish to permanently remove the scene from
    /// the manager (and clean up all the resources), issue a call to
    /// removeScenePermanent().
    ///
    /// \param scene Scene to remove.
    /// \returns the previous position of the scene.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int removeScene(Scene* scene);

    ////////////////////////////////////////////////////////////////////////////
    /// Cleans up all OpenGL and other resources of the given scene and removes
    /// it from the manager. If you plan to reuse the scene at a later point,
    /// you should use removeScene() to save valuable CPU time.
    ///
    /// \param scene Scene to remove permanently.
    /// \returns the previous position of the scene.
    ///
    ////////////////////////////////////////////////////////////////////////////
    int removeScenePermanent(Scene* scene);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all scenes from the manager. Effectively calls removeScene() on
    /// every scene in the manager.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllScenes();

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all scenes from the manager \em and frees all resources.
    /// Effectively calls removeScenePermanent() on every scene in the manager.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllScenesPermanent();

    void onUpdate(const GameTime& time);
    void onRender();
    void onMouseMoved(const MouseMoveEvent& event);
    void onMouseButtonDown(const MouseState& mouse);
    void onMouseButtonReleased(const MouseReleaseEvent& event);
    void onMouseDoubleClicked(const MouseReleaseEvent& event);
    void onKeyDown(const KeyboardState& keyboard);
    void onKeyReleased(const KeyReleaseEvent& event);
    void onKeyCharacter(const QString& event);
    void onGamepadButtonDown(const GamepadState& event);
    void onGamepadButtonReleased(const GamepadReleaseEvent& event);


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Window*         m_renderTarget;
    QVector<Scene*> m_activeScenes;
    QVector<Scene*> m_inactiveScenes;

    friend class Scene;
};


////////////////////////////////////////////////////////////////////////////////
/// \class SceneManager
/// \ingroup Game
///
/// The scene manager holds multiple scenes. All scenes that are currently
/// active (i.e. they have been added via addScene() and not yet been removed
/// by removeScene()) will receive events from all methods in format "on.*()".
///
/// You must furthermore \em not delete scenes that were added to the manager
/// through addScene(). Even if you explicitly call removeScene(), the scene
/// will remain in the manager, though inactive and will still be deleted when
/// the scene manager is destroyed.
///
/// \code
/// void MyWindow::onInit()
/// {
///     m_sceneManager->addScene(new FirstScene);
///     m_sceneManager->addScene(new SecondScene);
/// }
///
/// void MyWindow::onRender()
/// {
///     // FirstScene and SecondScene will be rendered.
///     // SecondScene is rendered above FirstScene.
///     m_sceneManager->onRender();
/// }
/// \endcode
///
/// The idea behind this is to effectively take out more and more functionality
/// out of the overridden cran::Window methods and split it up to multiple
/// scenes (which can even have a hierarchy). This way we ensure a clear and
/// structured code for the game.
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

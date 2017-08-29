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
#ifndef CRANBERRY_GAME_SCENE_SCENE_HPP
#define CRANBERRY_GAME_SCENE_SCENE_HPP


// Cranberry headers
#include <Cranberry/Input/KeyboardState.hpp>
#include <Cranberry/Input/KeyReleaseEvent.hpp>
#include <Cranberry/Input/MouseMoveEvent.hpp>
#include <Cranberry/Input/MouseReleaseEvent.hpp>
#include <Cranberry/Input/MouseState.hpp>
#include <Cranberry/Input/GamepadReleaseEvent.hpp>
#include <Cranberry/Input/GamepadState.hpp>
#include <Cranberry/System/GameTime.hpp>

// Qt headers
#include <QVector>

// Forward declarations
CRANBERRY_FORWARD_C(SceneManager)
CRANBERRY_FORWARD_C(Window)


CRANBERRY_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/// This class is the base class for all scenes. It receives events from the
/// scene manager, contains child scenes and provides functionality to jump to
/// ancestors (parent, children, siblings). If a scene is deleted, all of its
/// child scenes are also deleted.
///
/// \class Scene
/// \author Nicolas Kogler
/// \date August 29, 2017
///
////////////////////////////////////////////////////////////////////////////////
class CRANBERRY_GAME_EXPORT Scene
{
public:

    CRANBERRY_DECLARE_DTOR(Scene)
    CRANBERRY_DISABLE_COPY(Scene)
    CRANBERRY_DISABLE_MOVE(Scene)

    ////////////////////////////////////////////////////////////////////////////
    /// Constructs a new scene and sets the given \parent as parent.
    ///
    /// \param parent Parent of this scene.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Scene(Scene* parent = nullptr);

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the render target of the scene. Is always the same as the
    /// scene manager's render target.
    ///
    /// \returns the render target.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Window* renderTarget() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the scene manager of the scene.
    ///
    /// \returns the scene manager.
    ///
    ////////////////////////////////////////////////////////////////////////////
    SceneManager* sceneManager() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the name of this scene.
    ///
    /// \returns the name.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QString& name() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves the parent of this scene.
    ///
    /// \returns the parent.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Scene* parent() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Finds the child with the given name and returns it.
    ///
    /// \param name Name of the child.
    /// \returns a nullptr if child with that name does not exist.
    ///
    ////////////////////////////////////////////////////////////////////////////
    Scene* childByName(const QString& name) const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all of this scene's children.
    ///
    /// \returns the children.
    ///
    ////////////////////////////////////////////////////////////////////////////
    const QVector<Scene*>& children() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Retrieves all of this scene's siblings.
    ///
    /// \returns the siblings.
    ///
    ////////////////////////////////////////////////////////////////////////////
    QVector<Scene*> siblings() const;

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies the name of this scene. Must be used in order to find scenes
    /// by name.
    ///
    /// \param name Name of the scene.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setName(const QString& name);

    ////////////////////////////////////////////////////////////////////////////
    /// Specifies a new parent for this scene.
    ///
    /// \param parent New parent.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void setParent(Scene* parent);

    ////////////////////////////////////////////////////////////////////////////
    /// Adds a new child to the scene.
    ///
    /// \param child Child to add.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void addChild(Scene* child);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes the given child from the scene. This is a no-operation if the
    /// given child does not exist within this scene. Will not delete the child.
    ///
    /// \param child Child to remove.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeChild(Scene* child);

    ////////////////////////////////////////////////////////////////////////////
    /// Inserts a new child at the given position.
    ///
    /// \param index Position to insert child to.
    /// \param child Child to insert.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void insertChild(int index, Scene* child);

    ////////////////////////////////////////////////////////////////////////////
    /// Removes all the children from this scene.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void removeAllChildren();

    ////////////////////////////////////////////////////////////////////////////
    /// Exits the scene and triggers onExit(). If this method does not jump to
    /// any scene, the scene will "die" and can only be reactivated by issuing
    /// a call to SceneManager::addScene(thisScene) again.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void exitScene();

    ////////////////////////////////////////////////////////////////////////////
    /// Jumps to the given \p scene. Should be called from onExit().
    ///
    /// \param scene Scene to jump to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void jumpToScene(Scene* scene);

    ////////////////////////////////////////////////////////////////////////////
    /// Jumps to the scene with the given \p name. Should be called from
    /// onExit(). Does nothing if no scene called \p name exits.
    ///
    /// \param name Name of the scene to jump to.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void jumpToScene(const QString& name);


protected overridable:

    virtual void onCreate() = 0;
    virtual void onDestroy() { }
    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onUpdate(const GameTime& time) = 0;
    virtual void onRender() = 0;
    virtual void onMouseMoved(const MouseMoveEvent&) { }
    virtual void onMouseButtonDown(const MouseState&) { }
    virtual void onMouseButtonReleased(const MouseReleaseEvent&) { }
    virtual void onMouseDoubleClicked(const MouseReleaseEvent&) { }
    virtual void onKeyDown(const KeyboardState&) { }
    virtual void onKeyReleased(const KeyReleaseEvent&) { }
    virtual void onKeyCharacter(const QString&) { }
    virtual void onGamepadButtonDown(const GamepadState&) { }
    virtual void onGamepadButtonReleased(const GamepadReleaseEvent&) { }


private:

    ////////////////////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////////////////////
    Scene*          m_parent;
    SceneManager*   m_manager;
    Window*         m_window;
    QVector<Scene*> m_children;
    QString         m_name;

    friend class SceneManager;
};


////////////////////////////////////////////////////////////////////////////////
/// \class Scene
/// \ingroup Game
///
/// Simplifies building complicated scene structures. The jumpToScene()
/// overloads should be used to transition between two scenes.
///
/// Why distinguish between onCreate() and onInit()?
/// The reason is simple: A scene might be removed and readded at a later point.
/// In order not to recreate all the OpenGL resources etc. used for that scene,
/// the onCreate() only gets called the first time the scene was added to the
/// manager, and onDestroy() is called when the scene manager is destroyed.
/// onInit() and onExit() are called every time a scene is added/removed.
///
/// \code
/// class MyScene : public Scene
/// {
/// protected overridden:
///
///     void onCreate() override
///     {
///         // onCreate() should contain calls to object::create().
///         m_text = new Text;
///         m_text->create(renderTarget());
///     }
///
///     void onInit() override
///     {
///         // onInit() should contain setup code.
///         m_text->setText("Hello world!");
///         m_text->setFont(...);
///     }
///
///     void onDestroy() override
///     {
///         // onDestroy() should contain calls to object::destroy() or delete.
///         // Is also called from the destructor, therefore make sure that your
///         // pointers equal NULLPTR afterwards (delete checks against nullptr.)
///         delete m_text;
///         m_text = nullptr;
///     }
///
///     void onExit() override
///     {
///         // onExit() should contain changes to the scene state and such.
///         jumpToScene("childScene");
///     }
///
/// private:
///
///     Text* m_text = nullptr;
/// };
/// \endcode
///
////////////////////////////////////////////////////////////////////////////////


CRANBERRY_END_NAMESPACE


#endif

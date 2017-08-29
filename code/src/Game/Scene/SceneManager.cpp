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
#include <Cranberry/Game/Scene/SceneManager.hpp>


CRANBERRY_USING_NAMESPACE


SceneManager::SceneManager(Window* renderTarget)
    : m_renderTarget(renderTarget)
{
}


SceneManager::~SceneManager()
{
    for (Scene* s : m_activeScenes)
    {
        s->onDestroy();
    }

    for (Scene* s : m_inactiveScenes)
    {
        s->onDestroy();
    }

    qDeleteAll(m_activeScenes);
    qDeleteAll(m_inactiveScenes);
}


void SceneManager::addScene(Scene* scene)
{
    insertScene(m_activeScenes.size(), scene);
}


void SceneManager::insertScene(int index, Scene* scene)
{
    if (m_activeScenes.contains(scene))
    {
        return;
    }

    if (!m_inactiveScenes.contains(scene))
    {
        m_inactiveScenes.removeOne(scene);

        // Scene in for the first time? Create all resources.
        scene->m_manager = this;
        scene->m_window = m_renderTarget;
        scene->onCreate();
    }

    scene->onInit();

    // Cap max index at the current vector size.
    if (index > m_activeScenes.size())
    {
        index = m_activeScenes.size();
    }

    m_activeScenes.insert(index, scene);
}


int SceneManager::removeScene(Scene* scene)
{
    int pos = m_activeScenes.indexOf(scene);
    if (pos != -1)
    {

        // Puts scene to 'sleep'.
        m_activeScenes.removeAt(pos);
        m_inactiveScenes.append(scene);
    }

    return pos;
}


int SceneManager::removeScenePermanent(Scene* scene)
{
    int pos = m_activeScenes.indexOf(scene);
    if (pos != -1)
    {
        m_activeScenes.removeAt(pos);

        scene->onDestroy();
    }
    else if (m_inactiveScenes.contains(scene))
    {
        m_inactiveScenes.removeOne(scene);

        scene->onDestroy();
    }

    return pos;
}


void SceneManager::removeAllScenes()
{
    for (Scene* s : m_activeScenes)
    {
        removeScene(s);
    }
}


void SceneManager::removeAllScenesPermanent()
{
    for (Scene* s : m_activeScenes)
    {
        removeScenePermanent(s);
    }
}


void SceneManager::onUpdate(const GameTime& time)
{
    for (Scene* s : m_activeScenes)
    {
        s->onUpdate(time);
    }
}


void SceneManager::onRender()
{
    for (Scene* s : m_activeScenes)
    {
        s->onRender();
    }
}


void SceneManager::onMouseMoved(const MouseMoveEvent& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onMouseMoved(event);
    }
}


void SceneManager::onMouseButtonDown(const MouseState& mouse)
{
    for (Scene* s : m_activeScenes)
    {
        s->onMouseButtonDown(mouse);
    }
}


void SceneManager::onMouseButtonReleased(const MouseReleaseEvent& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onMouseButtonReleased(event);
    }
}


void SceneManager::onMouseDoubleClicked(const MouseReleaseEvent& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onMouseDoubleClicked(event);
    }
}


void SceneManager::onKeyDown(const KeyboardState& keyboard)
{
    for (Scene* s : m_activeScenes)
    {
        s->onKeyDown(keyboard);
    }
}


void SceneManager::onKeyReleased(const KeyReleaseEvent& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onKeyReleased(event);
    }
}


void SceneManager::onKeyCharacter(const QString& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onKeyCharacter(event);
    }
}


void SceneManager::onGamepadButtonDown(const GamepadState& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onGamepadButtonDown(event);
    }
}


void SceneManager::onGamepadButtonReleased(const GamepadReleaseEvent& event)
{
    for (Scene* s : m_activeScenes)
    {
        s->onGamepadButtonReleased(event);
    }
}

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
#include <Cranberry/Game/Scene/Scene.hpp>
#include <Cranberry/Game/Scene/SceneManager.hpp>

// Qt headers
#include <Cranberry/Window/Window.hpp>


CRANBERRY_USING_NAMESPACE


Scene::Scene(Scene* parent)
    : m_parent(parent)
    , m_manager(nullptr)
    , m_window(nullptr)
{
}


Scene::~Scene()
{
    qDeleteAll(m_children);
    m_children.clear();

    onDestroy();
}


Window* Scene::renderTarget() const
{
    return m_window;
}


SceneManager* Scene::sceneManager() const
{
    return m_manager;
}


const QString& Scene::name() const
{
    return m_name;
}


Scene* Scene::parent() const
{
    return m_parent;
}


Scene* Scene::childByName(const QString& name) const
{
    for (Scene* scene : m_children)
    {
        if (scene->name() == name)
        {
            return scene;
        }
    }

    return nullptr;
}


const QVector<Scene*>& Scene::children() const
{
    return m_children;
}


QVector<Scene*> Scene::siblings() const
{
    QVector<Scene*> s;
    if (m_parent == nullptr)
    {
        return s;
    }

    return s << m_parent->children();
}


void Scene::setName(const QString& name)
{
    m_name = name;
}


void Scene::setParent(Scene* parent)
{
    m_parent = parent;
}


void Scene::addChild(Scene* child)
{
    if (!m_children.contains(child))
    {
        m_children.append(child);
    }
}


void Scene::removeChild(Scene* child)
{
    // removeOne already checks whether it exists or not.
    m_children.removeOne(child);
}


void Scene::insertChild(int index, Scene* child)
{
    if (!m_children.contains(child))
    {
        m_children.insert(index, child);
    }
}


void Scene::removeAllChildren()
{
    m_children.clear();
}


void Scene::exitScene()
{
    onExit();
}


void Scene::jumpToScene(Scene* scene)
{
    // TODO: Transition possibilities
    int prev = m_manager->removeScene(this);
    m_manager->insertScene(prev, scene);
}


void Scene::jumpToScene(const QString& name)
{
    // Attempts to get any relative scene by name.
    if (m_parent && m_parent->name() == name)
    {
        jumpToScene(m_parent);
    }
    else
    {
        auto* scene = childByName(name);
        if (scene != nullptr)
        {
            jumpToScene(scene);
        }
        else if (m_parent != nullptr)
        {
            auto* sibling = m_parent->childByName(name);
            if (sibling != nullptr)
            {
                jumpToScene(sibling);
            }
        }
    }

    // No scene to jump to =(
}

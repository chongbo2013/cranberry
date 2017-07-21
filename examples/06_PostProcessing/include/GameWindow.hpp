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


#pragma once
#ifndef CRANBERRY_GAMEWINDOW_HPP
#define CRANBERRY_GAMEWINDOW_HPP


// Cranberry headers
#include <Cranberry/Graphics/SpriteBatch.hpp>
#include <Cranberry/Graphics/Text.hpp>
#include <Cranberry/Graphics/Base/ITexture.hpp>
#include <Cranberry/OpenGL/OpenGLDefaultShaders.hpp>
#include <Cranberry/Window/Window.hpp>


CRANBERRY_USING_NAMESPACE


class GameWindow : public Window
{
public:

    GameWindow();
    ~GameWindow();


protected:

    void onInit() override; // initialization
    void onExit() override; // disposal
    void onCrash() override; // save game data at unexpected termination
    void onUpdate(const GameTime&) override; // transform objects etc
    void onRender(); // render objects


private:

    SpriteBatch* m_batch;
    ITexture*    m_texture;
    Text*        m_linkText;

    Q_OBJECT
};


#endif

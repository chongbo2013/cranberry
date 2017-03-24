//
//  cranberry: C++ game engine using the Qt framework and OpenGL/ES.
//  Copyright (C) 2017 Nicolas Kogler
//
//  Cranberry is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cranberry is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with cranberry. If not, see <http://www.gnu.org/licenses/>.
//


#pragma once
#ifndef CRANBERRY_TEST_HPP
#define CRANBERRY_TEST_HPP


// Cranberry headers
#include <Cranberry/Window/Window.hpp>
#include <Cranberry/Graphics/Background.hpp>
#include <Cranberry/Graphics/Text.hpp>


class Test : public cran::Window
{
public:

    Test();


    void init() override;
    void exit() override;
    void keyUp(const cran::KeyReleaseEvent& ev) override;
    void update(const cran::GameTime& time) override;
    void render() override;


private:

    cran::Background m_bg;
    cran::Text m_text;
    QColor m_currentColor;
    uint32_t m_direction;
};


#define TEST_FADE_RED   0
#define TEST_FADE_GREEN 1
#define TEST_FADE_BLUE  2


#endif

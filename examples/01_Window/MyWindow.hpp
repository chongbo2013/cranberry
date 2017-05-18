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
#ifndef CRANBERRY_MYWINDOW_HPP
#define CRANBERRY_MYWINDOW_HPP


// Cranberry headers
#include <Cranberry/Window/Window.hpp>


CRANBERRY_BEGIN_NAMESPACE


///
/// Defines a custom window.
///
/// \class MyWindow
/// \author Nicolas Kogler
/// \date May 18, 2017
///
class MyWindow : public Window
{
public:

    ///
    /// Initializes a new instance of MyWindow.
    ///
    MyWindow();

    ///
    /// Destroys this MyWindow instance.
    ///
    ~MyWindow();


protected:

    void init() override;
    void exit() override;
};


CRANBERRY_END_NAMESPACE


#endif  // CRANBERRY_MYWINDOW_HPP

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


// 01_Window headers
#include "MyWindow.hpp"


CRANBERRY_BEGIN_NAMESPACE


MyWindow::MyWindow()
    : Window()
{
    resize(800, 600);
    setTitle("01_Window");
    setVerticalSync(true);
}


MyWindow::~MyWindow()
{
}


void MyWindow::init()
{
    qDebug("Window initialized.");
}


void MyWindow::exit()
{
    qDebug("Window destroyed.");
}


CRANBERRY_END_NAMESPACE

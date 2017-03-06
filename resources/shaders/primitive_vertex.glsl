﻿//
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


#version %0

// Input variables
layout(location = 0) in vec3 in_xyz;
layout(location = 1) in vec4 in_rgba;

// Forwarded variables
out vec4 out_rgba;

// Uniform variables
uniform mat4 uni_mvp;


void main()
{
    out_rgba = in_rgba;
    gl_Position = uni_mvp * vec4(in_xyz, 1.0);
}
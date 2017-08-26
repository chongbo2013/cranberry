[![License](https://img.shields.io/badge/license-LGPL%203.0-blue.svg?style=flat)](https://github.com/glClear/QGameBoy/blob/master/LICENSE)
[![Version](https://img.shields.io/badge/version-v1.0.0-blue.svg)](https://github.com/glClear/QGameBoy/releases/latest)
[![Qt-Framework](https://img.shields.io/badge/Qt-5.9+-green.svg)](http://qt.io)
[![Builds correctly](https://img.shields.io/badge/build-passing-green.svg)]()
# cranberry
C++ game engine using the Qt framework and OpenGL/ES.

## Introduction
Qt is, in my opinion, a framework which is completely underrated in terms of game engineering. This section will
solely describe the reasons why I use Qt to develop this game engine and why it is useful for this purpose.

Qt provides one of the best window management abstractions I have ever seen in my life. With its tool `qmake` it
is easy to compile your code for various platforms, such as mobile ones, which makes it attractive for modern game engineering.

The `Qt designer` can also be used to create graphical user interfaces. The process to create GUI's is streamlined, plus it is extremely flexible and extensible.

The `Qt resource system` is ideal for game engines, as it is capable of storing a diverse collection of assets within the executable
itself. You can access them by going to `:/<some_path>/<your_img>.png` 

Qt also puts an abstract layer onto OpenGL and OpenGL/ES. That means that if we design the code in such a way that
it uses the modern OpenGL pipeline, we can render stuff for desktop and mobile devices without doing redundant `#ifdef`'s or abstract
classes.

Lastly, the Qt core classes offers us `QObject` with it's `signal and slot` system (and also parent/child mechanisms) and `QString`,
which supports all unicode characters. By using functions of Qt such as the `Qt language system` in combination with `QObject::tr()`
and the `Qt linguist` you are able to to internationalize your game or modules. Implementing `QSql` or `QNetworking` will allow you to 
add multiplayer support to your game. With such advantages game engineering will surely be a pleasure!

# cranberry
C++ game engine using the Qt framework and OpenGL/ES.

## Introduction
Qt is, in my opinion, a framework which is completely underrated, also in terms of game engineering. This section will
solely describe the reason why I use Qt for this game engine and why it is so useful for the purpose we are trying to fulfill.

First of all, Qt provides one of the best window management abstractions I have ever seen in my life. With its tool `qmake` it
is absolutely easy to compile your code for various platforms, including mobile ones, which makes it attractive for modern game engineering.

Secondly, the `Qt designer` can be used to create all kinds of graphical user interfaces. It is easy and straight forward to design
GUIs, plus it is extremely flexible and extensible.

Thirdly, the `Qt resource system` is perfect for game engines, as it is capable of storing all kinds of assets within the executable
itself. Accessing them by `:/<some_path>/<your_img>.png` is also simple.

To mention another advantage, Qt also puts an abstract layer onto OpenGL and OpenGL/ES. That means if we design the code in a way
it uses the modern OpenGL pipeline, we can render stuff for desktop and mobile devices without doing annoying `#ifdef`'s or abstract
classes.

Finally, the Qt core classes offer us `QObject` with its `signal and slot` system (and also parent/child mechanisms) and `QString`,
which supports all unicode characters. By using some other stuff of Qt, like the `Qt language system` in combination with `QObject::tr()`
and the `Qt linguist` to internationalize your game or modules like `QSql` or `QNetworking` to add multiplayer support to your game,
game engineering will surely be a pleasure!

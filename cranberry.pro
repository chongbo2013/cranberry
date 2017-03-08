####################################################################
#
#   cranberry: C++ game engine using the Qt framework and OpenGL/ES.
#   Copyright (C) 2017 Nicolas Kogler
#   License: Lesser General Public License 3.0
#
####################################################################

####################################################################
#
# QMAKE SETTINGS (GENERAL)
####################################################################
TARGET      =   cranberry
TEMPLATE    =   lib
CONFIG     +=   c++11
DEFINES    +=   CRANBERRY_BUILD
QT         +=   widgets network sql xml

####################################################################
#
# QMAKE SETTINGS (WINDOWS)
####################################################################
win32 {
    QMAKE_TARGET_COMPANY        =   Nicolas Kogler
    QMAKE_TARGET_PRODUCT        =   cranberry
    QMAKE_TARGET_DESCRIPTION    =   Game engine in C++, Qt and OpenGL/ES
    QMAKE_TARGET_COPYRIGHT      =   Copyright (C) 2017 Nicolas Kogler
}

####################################################################
#
# COMPILER SETTINGS (GCC)
####################################################################
gcc {
    QMAKE_LFLAGS    +=  -static-libgcc -static-libstdc++
}

####################################################################
#
# OUTPUT DIRECTORIES
####################################################################
debug:DESTDIR       =   debug
debug:OBJECTS_DIR   =   debug/obj/
debug:MOC_DIR       =   debug/moc/
debug:RCC_DIR       =   debug/res/
debug:UI_DIR        =   debug/ui/
release:DESTDIR     =   release
release:OBJECTS_DIR =   release/obj/
release:MOC_DIR     =   release/moc/
release:RCC_DIR     =   release/res/
release:UI_DIR      =   release/ui/

####################################################################
#
# INCLUDE PATHS
####################################################################
INCLUDEPATH +=  include

####################################################################
#
# RESOURCES AND OTHER FILES
####################################################################
RESOURCES   +=  resources/res_shaders.qrc
DISTFILES   +=  templates/header.txt \
                templates/license_notice.txt \
                templates/source.txt

####################################################################
#
# HEADER FILES
####################################################################
HEADERS +=  include/Cranberry/Config.hpp \
            include/Cranberry/System/GameTime.hpp \
            include/Cranberry/System/GLDebug.hpp \
            include/Cranberry/System/GLShader.hpp \
            include/Cranberry/Window/Window.hpp \
            include/Cranberry/Window/Input/KeyboardState.hpp \
            include/Cranberry/Window/Input/KeyReleaseEvent.hpp \
            include/Cranberry/Window/Input/MouseState.hpp \
            include/Cranberry/Window/Input/MouseReleaseEvent.hpp \
            include/Cranberry/Window/Input/MouseMoveEvent.hpp \
            include/Cranberry/Graphics/System/GraphicsEnums.hpp \
            include/Cranberry/Graphics/System/Drawable.hpp \
            include/Cranberry/Graphics/System/GraphicsStructs.hpp \
            include/Cranberry/Graphics/Image.hpp \
            include/Cranberry/Graphics/System/GraphicsConstants.hpp \
            include/Cranberry/Graphics/System/Transformable.hpp \
            include/Cranberry/Graphics/System/Fadable.hpp \
            include/Cranberry/Graphics/Primitive.hpp \
            include/Cranberry/Graphics/Background.hpp \
            include/Cranberry/Graphics/Shapes/Line.hpp \
            include/Cranberry/Graphics/Shapes/Triangle.hpp \
            include/Cranberry/Graphics/Shapes/Polygon.hpp \
            include/Cranberry/Graphics/Shapes/Ellipse.hpp \
            include/Cranberry/Game/Game.hpp

####################################################################
#
# SOURCE FILES
####################################################################
SOURCES +=  src/System/GameTime.cpp \
            src/System/GLDebug.cpp \
            src/System/GLShader.cpp \
            src/Window/Window.cpp \
            src/Window/Input/KeyboardState.cpp \
            src/Window/Input/KeyReleaseEvent.cpp \
            src/Window/Input/MouseMoveEvent.cpp \
            src/Window/Input/MouseReleaseEvent.cpp \
            src/Window/Input/MouseState.cpp \
            src/Graphics/System/Drawable.cpp \
            src/Graphics/Image.cpp \
            src/Graphics/System/Transformable.cpp \
            src/Graphics/System/Fadable.cpp \
            src/Graphics/Primitive.cpp \
            src/Graphics/Background.cpp \
            src/Graphics/Shapes/Line.cpp \
            src/Graphics/Shapes/Triangle.cpp \
            src/Graphics/Shapes/Polygon.cpp \
            src/Graphics/Shapes/Ellipse.cpp \
            src/Game/Game.cpp

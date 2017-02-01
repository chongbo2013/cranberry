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
RESOURCES   +=
DISTFILES   +=  resources/templates/template_source \
                resources/templates/template_header

####################################################################
#
# HEADER FILES
####################################################################
HEADERS +=  include/Cranberry/Config.hpp \
            include/Cranberry/Window/cbWindow.hpp \
            include/Cranberry/Window/Events/cbMouseMoveEvent.hpp \
            include/Cranberry/Window/Events/cbMouseButtonEvent.hpp \
            include/Cranberry/Window/Events/cbKeyUpEvent.hpp \
            include/Cranberry/Window/Events/cbKeyboardState.hpp \
            include/Cranberry/Graphics/System/cbDrawable.hpp \
            include/Cranberry/System/cbGameTime.hpp \
            include/Cranberry/System/Enums/cbGraphicsEnums.hpp

####################################################################
#
# SOURCE FILES
####################################################################
SOURCES +=  src/Window/Events/cbMouseMoveEvent.cpp \
            src/Window/Events/cbMouseButtonEvent.cpp \
            src/Window/Events/cbKeyUpEvent.cpp \
            src/Window/Events/cbKeyboardState.cpp \
            src/Window/cbWindow.cpp \
            src/System/cbGameTime.cpp \
            src/Graphics/System/cbDrawable.cpp

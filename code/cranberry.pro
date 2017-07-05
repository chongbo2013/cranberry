################################################################################
##
## Cranberry - C++ game engine based on the Qt5 framework.
## Copyright (C) 2017 Nicolas Kogler
## License - Lesser General Public License (LGPL) 3.0
##
################################################################################


################################################################################
## GENERAL SETTINGS
##
################################################################################
QT             +=       widgets gamepad xml
CONFIG         +=       c++11 exceptions
DEFINES        +=       CRANBERRY_BUILD
TEMPLATE        =       lib
TARGET          =       cranberry


################################################################################
## WINDOWS SETTINGS
##
################################################################################
win32 {
    QMAKE_TARGET_COMPANY        =       Nicolas Kogler
    QMAKE_TARGET_PRODUCT        =       cranberry
    QMAKE_TARGET_DESCRIPTION    =       C++ game engine based on the Qt5 framework.
    QMAKE_TARGET_COPYRIGHT      =       Copyright (C) 2017 Nicolas Kogler
}


################################################################################
## COMPILER SETTINGS
##
################################################################################
gcc {
    QMAKE_LFLAGS        +=      -static-libgcc -static-libstdc++
}


################################################################################
## MISCELLANEOUS
##
################################################################################
INCLUDEPATH         +=      include
RESOURCES           +=      resources/glsl.qrc


################################################################################
## HEADER FILES
##
################################################################################
HEADERS     +=      include/Cranberry/Config.hpp \
                    include/Cranberry/System/Debug.hpp \
                    include/Cranberry/System/GameTime.hpp \
                    include/Cranberry/System/Random.hpp \
                    include/Cranberry/System/Emitters/RenderableEmitter.hpp \
                    include/Cranberry/System/Emitters/TransformableEmitter.hpp \
                    include/Cranberry/System/Emitters/BackgroundEmitter.hpp \
                    include/Cranberry/OpenGL/OpenGLDebug.hpp \
                    include/Cranberry/OpenGL/OpenGLVertex.hpp \
                    include/Cranberry/OpenGL/OpenGLShader.hpp \
                    include/Cranberry/OpenGL/OpenGLDefaultShaders.hpp \
                    include/Cranberry/Graphics/Base/IRenderable.hpp \
                    include/Cranberry/Graphics/Base/ITransformable.hpp \
                    include/Cranberry/Graphics/Base/ITexture.hpp \
                    include/Cranberry/Graphics/Base/Enumerations.hpp \
                    include/Cranberry/Graphics/Background.hpp \
                    include/Cranberry/Input/KeyReleaseEvent.hpp \
                    include/Cranberry/Input/KeyboardState.hpp \
                    include/Cranberry/Input/MouseMoveEvent.hpp \
                    include/Cranberry/Input/MouseReleaseEvent.hpp \
                    include/Cranberry/Input/MouseState.hpp \
                    include/Cranberry/Input/Enumerations.hpp \
                    include/Cranberry/Input/GamepadState.hpp \
                    include/Cranberry/Input/GamepadReleaseEvent.hpp \
                    include/Cranberry/Window/WindowSettings.hpp \
                    include/Cranberry/Window/Window.hpp \
                    include/Cranberry/Graphics/Base/TextureAtlas.hpp \
                    include/Cranberry/Graphics/Base/IAnimation.hpp \
                    include/Cranberry/System/Emitters/AnimationEmitter.hpp \
                    include/Cranberry/Graphics/GifAnimation.hpp \
                    include/Cranberry/Graphics/CranAnimation.hpp \
                    include/Cranberry/Graphics/Base/IShape.hpp \
                    include/Cranberry/Graphics/Polygon.hpp \
                    include/Cranberry/Graphics/Ellipse.hpp \
                    include/Cranberry/Game/Game.hpp \
                    include/Cranberry/Graphics/Text.hpp


################################################################################
## SOURCE FILES
##
################################################################################
SOURCES     +=      src/System/Debug.cpp \
                    src/System/GameTime.cpp \
                    src/System/Random.cpp \
                    src/OpenGL/OpenGLVertex.cpp \
                    src/OpenGL/OpenGLDebug.cpp \
                    src/OpenGL/OpenGLShader.cpp \
                    src/OpenGL/OpenGLDefaultShaders.cpp \
                    src/Graphics/Base/IRenderable.cpp \
                    src/Graphics/Base/ITransformable.cpp \
                    src/Graphics/Background.cpp \
                    src/Graphics/Base/ITexture.cpp \
                    src/Input/KeyReleaseEvent.cpp \
                    src/Input/KeyboardState.cpp \
                    src/Input/MouseMoveEvent.cpp \
                    src/Input/MouseReleaseEvent.cpp \
                    src/Input/MouseState.cpp \
                    src/Input/GamepadState.cpp \
                    src/Input/GamepadReleaseEvent.cpp \
                    src/Window/WindowSettings.cpp \
                    src/Window/Window.cpp \
                    src/Graphics/Base/TextureAtlas.cpp \
                    src/Graphics/Base/IAnimation.cpp \
                    src/Graphics/GifAnimation.cpp \
                    src/Graphics/CranAnimation.cpp \
                    src/Graphics/Base/IShape.cpp \
                    src/Graphics/Polygon.cpp \
                    src/Graphics/Ellipse.cpp \
                    src/Game/Game.cpp \
                    src/Graphics/Text.cpp

################################################################################
## OUTPUT
##
################################################################################
include(platforms.pri)
message(Writing library to: $${PWD}/../bin/$${kgl_path})

DESTDIR     = $${PWD}/../bin/$${kgl_path}
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR     = $${OBJECTS_DIR}
RCC_DIR     = $${OBJECTS_DIR}
UI_DIR      = $${OBJECTS_DIR}

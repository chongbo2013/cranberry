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
QT             +=       widgets gamepad qml quick
CONFIG         +=       c++11 exceptions no_keywords
DEFINES        +=       CRANBERRY_BUILD
DEFINES        +=       CRANBERRY_VERSION=\\\"1.0.0\\\"
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
} win32 {
    LIBS                +=      -lDbgHelp
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
                    include/Cranberry/System/Emitters/BackgroundEmitter.hpp \
                    include/Cranberry/System/Receivers/SpriteReceiver.hpp \
                    include/Cranberry/System/Receivers/GuiManagerReceiver.hpp \
                    include/Cranberry/OpenGL/OpenGLDebug.hpp \
                    include/Cranberry/OpenGL/OpenGLVertex.hpp \
                    include/Cranberry/OpenGL/OpenGLShader.hpp \
                    include/Cranberry/OpenGL/OpenGLDefaultShaders.hpp \
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
                    include/Cranberry/Graphics/Base/Enumerations.hpp \
                    include/Cranberry/Graphics/Background.hpp \
                    include/Cranberry/Graphics/Base/TextureAtlas.hpp \
                    include/Cranberry/Graphics/GifAnimation.hpp \
                    include/Cranberry/Graphics/CranAnimation.hpp \
                    include/Cranberry/Graphics/Polygon.hpp \
                    include/Cranberry/Graphics/Ellipse.hpp \
                    include/Cranberry/Graphics/Text.hpp \
                    include/Cranberry/Graphics/SpriteBatch.hpp \
                    include/Cranberry/Graphics/Sprite.hpp \
                    include/Cranberry/Graphics/RawAnimation.hpp \
                    include/Cranberry/Game/Game.hpp \
                    include/Cranberry/Gui/GuiManager.hpp \
                    include/Cranberry/Graphics/Base/AnimationFrame.hpp \
                    include/Cranberry/Graphics/Base/AnimationBase.hpp \
                    include/Cranberry/Graphics/Base/RenderBase.hpp \
                    include/Cranberry/Graphics/Base/ShapeBase.hpp \
                    include/Cranberry/Graphics/Base/TextureBase.hpp \
                    include/Cranberry/Graphics/Base/TransformBase.hpp \
                    include/Cranberry/Graphics/Base/SpriteMovement.hpp \
                    include/Cranberry/System/Emitters/RenderBaseEmitter.hpp \
                    include/Cranberry/System/Emitters/TransformBaseEmitter.hpp \
                    include/Cranberry/System/Emitters/AnimationBaseEmitter.hpp \
                    include/Cranberry/Game/GamePrivate.hpp \

win32 {
HEADERS     +=      include/Cranberry/System/StackWalkers/StackWalkerWin32.hpp
}


################################################################################
## SOURCE FILES
##
################################################################################
SOURCES     +=      src/System/Debug.cpp \
                    src/System/GameTime.cpp \
                    src/System/Random.cpp \
                    src/System/Receivers/SpriteReceiver.cpp \
                    src/System/Receivers/GuiManagerReceiver.cpp \
                    src/OpenGL/OpenGLVertex.cpp \
                    src/OpenGL/OpenGLDebug.cpp \
                    src/OpenGL/OpenGLShader.cpp \
                    src/OpenGL/OpenGLDefaultShaders.cpp \
                    src/Input/KeyReleaseEvent.cpp \
                    src/Input/KeyboardState.cpp \
                    src/Input/MouseMoveEvent.cpp \
                    src/Input/MouseReleaseEvent.cpp \
                    src/Input/MouseState.cpp \
                    src/Input/GamepadState.cpp \
                    src/Input/GamepadReleaseEvent.cpp \
                    src/Window/WindowSettings.cpp \
                    src/Window/Window.cpp \
                    src/Graphics/Background.cpp \
                    src/Graphics/Base/TextureAtlas.cpp \
                    src/Graphics/GifAnimation.cpp \
                    src/Graphics/CranAnimation.cpp \
                    src/Graphics/Polygon.cpp \
                    src/Graphics/Ellipse.cpp \
                    src/Graphics/Text.cpp \
                    src/Graphics/SpriteBatch.cpp \
                    src/Graphics/Sprite.cpp \
                    src/Graphics/RawAnimation.cpp \
                    src/Game/Game.cpp \
                    src/Gui/GuiManager.cpp \
                    src/Graphics/Base/AnimationFrame.cpp \
                    src/Graphics/Base/AnimationBase.cpp \
                    src/Graphics/Base/RenderBase.cpp \
                    src/Graphics/Base/ShapeBase.cpp \
                    src/Graphics/Base/TextureBase.cpp \
                    src/Graphics/Base/TransformBase.cpp \
                    src/Graphics/Base/SpriteMovement.cpp \
                    src/Game/GamePrivate.cpp \

win32 {
SOURCES     +=      src/System/StackWalkers/StackWalkerWin32.cpp
}

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

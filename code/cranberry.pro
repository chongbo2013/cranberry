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
QT             +=       widgets xml
CONFIG         +=       c++11
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
#RESOURCES           +=      resources/shader.qrc


################################################################################
## HEADER FILES
##
################################################################################
HEADERS     +=      include/Cranberry/Config.hpp \
                    include/Cranberry/System/Debug.hpp \
                    include/Cranberry/System/GameTime.hpp \
                    include/Cranberry/System/Random.hpp \
                    include/Cranberry/OpenGL/OpenGLDebug.hpp \
                    include/Cranberry/OpenGL/OpenGLVertex.hpp \
                    include/Cranberry/OpenGL/OpenGLShader.hpp \
                    include/Cranberry/Graphics/Base/IRenderable.hpp \
                    include/Cranberry/Graphics/Base/ITransformable.hpp \
                    include/Cranberry/Graphics/Base/Enumerations.hpp \
                    include/Cranberry/System/Emitters/RenderableEmitter.hpp \
                    include/Cranberry/System/Emitters/TransformableEmitter.hpp \
    include/Cranberry/Graphics/Base/ITexture.hpp \
    include/Cranberry/OpenGL/OpenGLDefaultShaders.hpp


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
                    src/Graphics/Base/IRenderable.cpp \
                    src/Graphics/Base/ITransformable.cpp \
    src/OpenGL/OpenGLDefaultShaders.cpp \
    src/Graphics/Base/ITexture.cpp

################################################################################
## OUTPUT
##
################################################################################
include(platforms.pri)
message(Writing library to: $${PWD}/bin/$${kgl_path})

DESTDIR     = $${PWD}/bin/$${kgl_path}
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR     = $${OBJECTS_DIR}
RCC_DIR     = $${OBJECTS_DIR}
UI_DIR      = $${OBJECTS_DIR}

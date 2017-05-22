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
QT             +=       widgets network sql xml
CONFIG         +=       c++11
CONFIG         -=       debug_and_release debug_and_release_target
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
    include/Cranberry/System/Random.hpp


################################################################################
## MISCELLANEOUS
##
################################################################################
SOURCES     +=      src/System/Debug.cpp \
                    src/System/GameTime.cpp \
    src/System/Random.cpp

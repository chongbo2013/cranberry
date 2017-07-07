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
###############################################################################
QT             +=       core gui widgets
CONFIG         +=       c++11 exceptions
TEMPLATE        =       app
TARGET          =       %n_%AppName


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
INCLUDEPATH         +=      include ../../code/include
RESOURCES           +=


################################################################################
## HEADER FILES
##
################################################################################
HEADERS     +=


################################################################################
## SOURCE FILES
##
################################################################################
SOURCES     +=


################################################################################
## OUTPUT
##
################################################################################
include(platforms.pri)

LIBS       += -L$${PWD}/../../bin/$${kgl_path} -lcranberry
DESTDIR     = $${PWD}/bin/$${kgl_path}
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR     = $${OBJECTS_DIR}
RCC_DIR     = $${OBJECTS_DIR}
UI_DIR      = $${OBJECTS_DIR}

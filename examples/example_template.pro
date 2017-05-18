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
TARGET          =       0X_XXYYZZ
TEMPLATE        =       app
CONFIG         +=       c++11
CONFIG         -=       debug_and_release debug_and_release_target
INCLUDEPATH    +=       ../../include/

####################################################################
#
# COMPILER SETTINGS (GCC)
####################################################################
gcc {
    QMAKE_LFLAGS    +=  -static-libgcc -static-libstdc++
}

####################################################################
#
# HEADER FILES
####################################################################
HEADERS += \

####################################################################
#
# SOURCE FILES
####################################################################
SOURCES += \

###########################################################
#
# LINKER SETTINGS
###########################################################
os = foo
cc = g++
arch = foo
mode = foo
lib = cranberry

CONFIG(debug, debug|release) {
    mode = debug
} else {
    mode = release
}

arch = $${QMAKE_HOST.arch}
os = $${lower($${QMAKE_HOST.os})}

win32 {
    win32-g++ {
        cc = mingw
    } else {
        cc = msvc
    }
}

linux {
    linux-g++ {
        cc = g++
    }
}

android {
    arch = ANDROID_TARGET_ARCH
    cc = g++
}

LIBS += -L$${PWD}/../../bin/$${os}_$${arch}_$${cc}/$${mode}/ -l$${lib}
message(Linking against: ../../bin/$${os}_$${arch}_$${cc}/$${mode}/$${lib}.a)

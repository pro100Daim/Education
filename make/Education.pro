TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../src/src/main.cpp

HEADERS += \
    ../src/inc/functions.hpp \
    ../src/inc/my_vector.hpp

INCLUDEPATH += \
    ../src/inc

TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    graph.cpp \
    graphloopdetector.cpp
HEADERS += \
    graph.h \
    graphloopdetector.h \
    graphnode.h \
    utils.h

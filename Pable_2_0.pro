TEMPLATE = subdirs

SUBDIRS += \
    PableApplication\
    PableLib \
    PableTests

# CONFIG += ordered

PableApplication.file = PableApplication/PableApplication.pro
PableLib.file = PableLib/PableLib.pro

# what subproject depends on others
PableApplication.depends = PableLib
PableTests.depends = PableLib

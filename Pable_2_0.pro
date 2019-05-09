TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    PableLib\
    PableTests\
    PableApplication

PableApplication.file = PableApplication/PableApplication.pro
PableLib.file = PableLib/PableLib.pro

# what subproject depends on others
PableApplication.depends = PableLib
PableTests.depends = PableLib

#-------------------------------------------------
#
# Project created by QtCreator 2019-06-29T15:14:42
#
#-------------------------------------------------

QT       += widgets dbus

TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../..

TARGET = $$qtLibraryTarget(area)
DESTDIR = ../../../pluginlibs

LIBS += -L/usr/lib/ -lgsettings-qt

##加载gio库和gio-unix库，用于处理时间
CONFIG        += link_pkgconfig \
                 C++11
PKGCONFIG     += gio-2.0 \
                 gio-unix-2.0


#DEFINES += QT_DEPRECATED_WARNINGS

target.path = $$[QT_INSTALL_PREFIX]/lib/control-center/plugins/
INSTALLS += target


SOURCES += \
        area.cpp

HEADERS += \
        area.h

FORMS += \
        area.ui
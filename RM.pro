#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T01:51:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RM
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += include/

SOURCES += main.cpp \
    mainwindow.cpp \
    src/lib_json/json_reader.cpp \
    src/lib_json/json_value.cpp \
    src/lib_json/json_writer.cpp \
    src/item.cpp \
    src/utils.cpp \
    itemform.cpp \
    skillform.cpp \
    src/skill.cpp

HEADERS  += \
    mainwindow.h \
    include/json/autolink.h \
    include/json/config.h \
    include/json/features.h \
    include/json/forwards.h \
    include/json/json.h \
    include/json/reader.h \
    include/json/value.h \
    include/json/writer.h \
    include/array2d.h \
    include/character.h \
    include/enum.h \
    include/eventcontroller.h \
    include/gmap.h \
    include/inventory.h \
    include/item.h \
    include/mapobject.h \
    include/monster.h \
    include/monsterfactory.h \
    include/objplayer.h \
    include/point.h \
    include/role.h \
    include/rolefactory.h \
    include/skill.h \
    include/team.h \
    include/utils.h \
    src/lib_json/json_batchallocator.h \
    itemform.h \
    skillform.h

FORMS    += \
    mainwindow.ui \
    itemform.ui \
    skillform.ui

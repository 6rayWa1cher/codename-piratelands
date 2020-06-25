QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    battle.cpp \
    battleitemeffect.cpp \
    battlelostwindow.cpp \
    battlewindow.cpp \
    battlewonwindow.cpp \
    copyrightwindow.cpp \
    enemystatsmodel.cpp \
    game.cpp \
    hero.cpp \
    item.cpp \
    itemcollectedwindow.cpp \
    itemmapmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    mapwindow.cpp \
    shop.cpp \
    shopwindow.cpp \
    world.cpp

HEADERS += \
    battle.h \
    battleitemeffect.h \
    battlelostwindow.h \
    battlewindow.h \
    battlewonwindow.h \
    copyrightwindow.h \
    enemystatsmodel.h \
    game.h \
    hero.h \
    item.h \
    itemcollectedwindow.h \
    itemmapmodel.h \
    mainwindow.h \
    mapwindow.h \
    shop.h \
    shopwindow.h \
    world.h

FORMS += \
    battlelostwindow.ui \
    battlewindow.ui \
    battlewonwindow.ui \
    copyrightwindow.ui \
    itemcollectedwindow.ui \
    mainwindow.ui \
    mapwindow.ui \
    shopwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/fogE1.png \
    resources/island1.png \
    resources/island2.png \
    resources/island3.png \
    resources/island4.png \
    resources/islandE1.png \
    resources/map.png \
    resources/sea.png \
    resources/treasure.png

RESOURCES += \
    RoomImages.qrc

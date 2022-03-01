QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errormanager.cpp \
    main.cpp \
    playm4interface.cpp \
    qtvsplayer.cpp \
    videoctrls.cpp

HEADERS += \
    errormanager.h \
    include/LinuxPlayM4.h \
    include/PlayM4.h \
    playm4interface.h \
    qtvsplayer.h \
    videoctrls.h

FORMS += \
    qtvsplayer.ui \
    videoctrls.ui

TRANSLATIONS += \
    QtVsPlayer_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    lib/aarch64/libAudioRender.so \
    lib/aarch64/libPlayCtrl.so \
    lib/aarch64/libStreamTransClient.so \
    lib/aarch64/libSuperRender.so \
    lib/aarch64/libcalci.so

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/QtVsPlayer/ -lAudioRender

INCLUDEPATH += $$PWD/../../../../usr/lib/QtVsPlayer
DEPENDPATH += $$PWD/../../../../usr/lib/QtVsPlayer

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/QtVsPlayer/ -lcalci

INCLUDEPATH += $$PWD/../../../../usr/lib/QtVsPlayer
DEPENDPATH += $$PWD/../../../../usr/lib/QtVsPlayer

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/QtVsPlayer/ -lPlayCtrl

INCLUDEPATH += $$PWD/../../../../usr/lib/QtVsPlayer
DEPENDPATH += $$PWD/../../../../usr/lib/QtVsPlayer

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/QtVsPlayer/ -lStreamTransClient

INCLUDEPATH += $$PWD/../../../../usr/lib/QtVsPlayer
DEPENDPATH += $$PWD/../../../../usr/lib/QtVsPlayer

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/QtVsPlayer/ -lSuperRender

INCLUDEPATH += $$PWD/../../../../usr/lib/QtVsPlayer
DEPENDPATH += $$PWD/../../../../usr/lib/QtVsPlayer

RESOURCES += \
    ressources.qrc

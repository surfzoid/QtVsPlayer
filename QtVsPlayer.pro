QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}
else: unix:!android: target.path = /opt/$${TARGET}
!isEmpty(target.path): INSTALLS += target

unix {

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
LIBS += -L$$PWD/lib/$$QMAKE_HOST.arch/ -Wl,-rpath=$$PWD/lib/$$QMAKE_HOST.arch:/HCNetSDKCom:./ -lPlayCtrl -lAudioRender -lSuperRender
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errormanager.cpp \
    filesliste.cpp \
    main.cpp \
    playm4interface.cpp \
    qtvsplayer.cpp \
    videoctrls.cpp

HEADERS += \
    errormanager.h \
    filesliste.h \
    include/LinuxPlayM4.h \
    include/PlayM4.h \
    playm4interface.h \
    qtvsplayer.h \
    videoctrls.h

FORMS += \
    filesliste.ui \
    qtvsplayer.ui \
    videoctrls.ui

TRANSLATIONS += \
    QtVsPlayer_fr_FR.ts

RESOURCES += \
    ressources.qrc

DISTFILES += \
    QtVsPlayer.desktop \
    README.md



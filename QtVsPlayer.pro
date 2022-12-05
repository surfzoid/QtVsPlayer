VERSION = 1.0.25
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
QT       += core gui multimedia multimediawidgets
!android:QT       += dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 shared

# Specifies name of the binary.
TARGET = QtVsPlayer

# Denotes that project is an application.
TEMPLATE = app

#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2

win32 { # For Windows
  #QMAKE_CXXFLAGS += -openmp
  #QMAKE_CXXFLAGS += -declspec
  QMAKE_CXXFLAGS += -D "_CRT_SECURE_NO_WARNINGS"
  QMAKE_CXXFLAGS += -std=c++11
  QMAKE_CC = gcc
  QMAKE_CXX = g++
}

linux { # For Linux
  QMAKE_CXXFLAGS += -fopenmp
  QMAKE_LFLAGS += -fopenmp
}
unix {
    isEmpty(PREFIX) {
        #PREFIX = $$(HOME)/.local/share
        PREFIX = /usr/share
    }

    shortcutfiles.path = $$(PREFIX)/applications/
    shortcutfiles.files = $$PWD/QtVsPlayer.desktop
    iconfiles.path = $$(PREFIX)/icons/
    iconfiles.files = $$PWD/images/QtVsPlayer.png
    #INSTALLS += shortcutfiles
    #INSTALLS += iconfiles
    #For Mageia, adjust
    libfiles.path = $$(PREFIX)/../lib64
    libfiles.files += $$PWD/lib/$$QMAKE_HOST.arch/*
    translationfiles.path = $$(PREFIX)/QtVsPlayer/translations
    translationfiles.files += $$PWD/*.qm
    docfiles.path = $$(PREFIX)/doc/QtVsPlayer
    docfiles.files += $$PWD/README.md
    licfiles.path = $$(PREFIX)/licenses/QtVsPlayer
    licfiles.files += $$PWD/Licenses_playctrl_linux.txt LICENSE
    }
#message("If mkdir of /opt/QtVsPlayer return erorror not permit, please do :")
#message("sudo mkdir /opt/QtVsPlayer & sudo chown -R $USER /opt/QtVsPlayer")


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}
else: unix:!android: target.path = $$(PREFIX)/../bin #/opt/$${TARGET}
target.files += $${TARGET}
debugfiles.path = $$(PREFIX)/../lib/debug/usr/bin
debugfiles.files = $${TARGET}.debug
!isEmpty(target.path): INSTALLS += target shortcutfiles iconfiles libfiles translationfiles debugfiles docfiles licfiles

unix:!macx:!android: {
#CONFIG += separate_debug_info
CONFIG += force_debug_info
# INCLUDEPATH += lib
# DEPENDPATH += lib
LIBS += -L$$PWD/lib/$$QMAKE_HOST.arch/ -Wl,-rpath=lib/$$QMAKE_HOST.arch:/HCNetSDKCom:./ -lPlayCtrl -lAudioRender -lSuperRender
}

macx: {
LIBS += -L$$PWD/lib/$$QMAKE_HOST.arch/ -lPlayCtrl -lSuperRender

INCLUDEPATH += $$PWD/lib/$$QMAKE_HOST.arch/
DEPENDPATH += $$PWD/lib/$$QMAKE_HOST.arch/
ICON = $$PWD/images/QtVsPlayer.icns
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
}

android: {
LIBS += -L$$PWD/lib/armeabi-v7a/ -lPlayCtrl

INCLUDEPATH += $$PWD/lib/armeabi-v7a/
DEPENDPATH += $$PWD/lib/armeabi-v7a/
ICON = $$PWD/images/QtVsPlayer.png
}

# macx: PRE_TARGETDEPS += $$PWD/lib/$$QMAKE_HOST.arch/libbench2.a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errormanager.cpp \
    filesliste.cpp \
    main.cpp \
    playm4interface.cpp \
    qtvsplayer.cpp \
    rtspwindow.cpp \
    settingsform.cpp \
    videoctrls.cpp

HEADERS += \
    errormanager.h \
    filesliste.h \
    include/LinuxPlayM4.h \
    include/PlayM4.h \
    include/WindowsPlayM4.h \
    playm4interface.h \
    qtvsplayer.h \
    rtspwindow.h \
    settingsform.h \
    videoctrls.h

FORMS += \
    filesliste.ui \
    qtvsplayer.ui \
    rtspwindow.ui \
    settingsform.ui \
    videoctrls.ui

!android:DBUS_ADAPTORS += local.QtVsPlayer.xml
!android:DBUS_INTERFACES += local.QtVsPlayer.xml

TRANSLATIONS += \
    QtVsPlayer_fr_FR.ts

RESOURCES += \
    ressources.qrc

DISTFILES += \
    Open Source Software Licenses_playctrl_linux.txt \
    QtVsPlayer-fix.patch \
    QtVsPlayer.desktop \
    QtVsPlayer.spec \
    QtVsPlayermga8wally.spec \
    README.md \
    TODO \
    install-win.md




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/w86_64/ -Wl,-rpath=lib/w86_64:/HCNetSDKCom:./ -lPlayCtrl -lAudioRender -lSuperRender
else:win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/lib/w86_64/ -Wl,-rpath=lib/w86_64:/HCNetSDKCom:./ -lPlayCtrl -lAudioRender -lSuperRender
INCLUDEPATH += $$PWD/lib/w86_64
DEPENDPATH += $$PWD/lib/w86_64

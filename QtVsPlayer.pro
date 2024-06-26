VERSION = 1.0.52
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
RELEASE = 3
DEFINES += APP_RELEASE=\\\"$$RELEASE\\\"

QT       += core gui network
!android:QT       += dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 shared warn_off
OUTPUT += Console
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
  QMAKE_CXXFLAGS += -fopenmp -lm
  QMAKE_LFLAGS += -fopenmp -lm
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
    libfiles.path = $$(PREFIX)/../lib64/QtVsPlayer
    libfiles.files += $$PWD/lib/$$QMAKE_HOST.arch/*.so* $$PWD/lib/$$QMAKE_HOST.arch/HCNetSDKCom
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
INCLUDEPATH += $$PWD/lib/$$QMAKE_HOST.arch/
DEPENDPATH += $$PWD/lib/$$QMAKE_HOST.arch/
LIBS += -L$$PWD/lib/$$QMAKE_HOST.arch/ -Wl,-rpath=/usr/lib64/QtVsPlayer -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender
}

macx: {
LIBS += -L$$PWD/lib/macX86_64/ -lSystemTransform -lhcnetsdk -lPlayCtrl -lSuperRender

INCLUDEPATH += $$PWD/lib/macX86_64/
DEPENDPATH += $$PWD/lib/macX86_64/
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
    cfgdialog.cpp \
    errormanager.cpp \
    filesliste.cpp \
    infos.cpp \
    main.cpp \
    multimedia_shortcuts.cpp \
    pantilcmd.cpp \
    playm4interface.cpp \
    qtvsplayer.cpp \
    rtspwindow.cpp \
    settingsform.cpp \
    simplecrypt.cpp \
    videoctrls.cpp

HEADERS += \
    cfgdialog.h \
    errormanager.h \
    filesliste.h \
    include/ApplePlayM4.h \
    include/HCNetSDK.h \
    include/LinuxPlayM4.h \
    include/PlayM4.h \
    include/WindowsPlayM4.h \
    infos.h \
    multimedia_shortcuts.h \
    pantilcmd.h \
    playm4interface.h \
    qtvsplayer.h \
    rtspwindow.h \
    settingsform.h \
    simplecrypt.h \
    videoctrls.h

FORMS += \
    cfgdialog.ui \
    filesliste.ui \
    infos.ui \
    multimedia_shortcuts.ui \
    pantilcmd.ui \
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
    install-mac.md \
    install-win.md

win32: {
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/w86_64/ -Wl,-rpath=lib/w86_64:/HCNetSDKCom:./ -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender
else:win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/lib/w86_64/ -Wl,-rpath=lib/w86_64:/HCNetSDKCom:./ -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender
INCLUDEPATH += $$PWD/lib/w86_64
DEPENDPATH += $$PWD/lib/w86_64
RC_ICONS = $$PWD/images/QtVsPlayer.ico
}

unix:!macx: LIBS += -lopenal

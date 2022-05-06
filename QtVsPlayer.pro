VERSION = 1.0.0.5
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
QT       += core gui opengl multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 debug_and_release link_prl

# Specifies name of the binary.
TARGET = QtVsPlayer

# Denotes that project is an application.
TEMPLATE = app

unix {
    isEmpty(PREFIX) {
        #PREFIX = $$(HOME)/.local/share
        PREFIX = /usr/share
    }

    shortcutfiles.path = $$PREFIX/applications/
    shortcutfiles.files = $$PWD/QtVsPlayer.desktop
    iconfiles.path = $$PREFIX/icons/
    iconfiles.files = $$PWD/images/QtVsPlayer.png
    #INSTALLS += shortcutfiles
    #INSTALLS += iconfiles
    #For Mageia, adjust
    libfiles.path = /usr/lib64
    libfiles.files += $$PWD/lib/$$QMAKE_HOST.arch/*
    translationfiles.path = /usr/share/QtVsPlayer/translations
    translationfiles.files = $$PWD/*.qm
    }
#message("If mkdir of /opt/QtVsPlayer return erorror not permit, please do :")
#message("sudo mkdir /opt/QtVsPlayer & sudo chown -R $USER /opt/QtVsPlayer")


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}
else: unix:!android: target.path = /usr/bin #/opt/$${TARGET}
target.files += $${TARGET}
!isEmpty(target.path): INSTALLS += target shortcutfiles iconfiles libfiles translationfiles

unix {
CONFIG += separate_debug_info
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
    rtspwindow.cpp \
    settingsform.cpp \
    videoctrls.cpp

HEADERS += \
    errormanager.h \
    filesliste.h \
    include/LinuxPlayM4.h \
    include/PlayM4.h \
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

TRANSLATIONS += \
    QtVsPlayer_fr_FR.ts

RESOURCES += \
    ressources.qrc

DISTFILES += \
    QtVsPlayer.desktop \
    QtVsPlayer.spec \
    README.md \
    TODO




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/w86_64/ -lPlayCtrl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/w86_64/ -lPlayCtrld

INCLUDEPATH += $$PWD/lib/w86_64
DEPENDPATH += $$PWD/lib/w86_64

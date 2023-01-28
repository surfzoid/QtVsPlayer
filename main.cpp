#include "qtvsplayer.h"

#include <QApplication>
#if (!defined(__ANDROID__))
#include "qtvsplayer_adaptor.h"
#endif

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (!defined(__linux__))
    printf("Debug---DBus is bugy under windows");
#elif ((defined(__linux__) | defined(__APPLE__)) &  !defined(__ANDROID__))
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n mac :brew services start dbus\n linux:systemctl start dbus.service\n");
        return 1;
    }
#endif


    QThreadPool::globalInstance()->setMaxThreadCount(1);
    QThreadPool::globalInstance()->setExpiryTimeout(3000);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("QtVsPlayerUniqueInstance");

    if (!sharedMemory.create(1))
    {
        if (argc > 1) {
#if (defined(__linux__) | defined(__APPLE__))
            new QtVsPlayerAdaptor(&a);
            QDBusConnection::sessionBus().registerObject("/", &a);
            //emit message(m_nickname, messageLineEdit->text());
            QDBusMessage msg = QDBusMessage::createSignal("/", "local.QtVsPlayer", "message");
            msg << QString(argv[1]);
            QDBusConnection::sessionBus().send(msg);
#endif
        }

        //check if QtVsplayer crashed and free sharedMemory
        sharedMemory.attach();
        sharedMemory.detach();


        exit(0); // Exit already a process running
    }

    //Translation
    QTranslator qtTranslator;
    bool bsuc = false;
    bsuc = qtTranslator.load( QLocale(), QLatin1String("qt"), QLatin1String("_"),
                              QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    //surpress warning!
    if (bsuc == false ) {
        //I'm happy for yu
    }
    //surpress warning!

    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    bsuc = myappTranslator.load(QLocale(), QLatin1String("QtVsPlayer"), QLatin1String("_"),
                                "/usr/share/QtVsPlayer/translations");

    //surpress warning!
    if (bsuc == false ) {
        //I'm happy for yu
    }
    //surpress warning!
#if (defined(_WIN32))
    bsuc = myappTranslator.load(QLocale(), QLatin1String("QtVsPlayer"), QLatin1String("_"),
                                QCoreApplication::applicationDirPath());
#endif

    a.installTranslator(&myappTranslator);


    //End Translation

    QtVsPlayer w;
    //    a.installEventFilter(&w);
    w.show();

    QStringList list;
    for (int a = 1; a < argc; ++a) {
        list << QString::fromLocal8Bit(argv[a]);
    }

    //if (list.length() > 0) {w.ParseArgs(list);}
    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");
    return a.exec();
    //}
}

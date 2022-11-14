#include "qtvsplayer.h"

#include <QApplication>
#include "qtvsplayer_adaptor.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (defined(_WIN32)| defined(_MACX))
    printf("Debug---DBus is bugy under windows and macx");
#elif defined(__linux__)
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
        return 1;
    }
#endif


    QThreadPool::globalInstance()->setMaxThreadCount(1);
    QThreadPool::globalInstance()->setExpiryTimeout(3000);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("QvSPlayerUniqueInstance");

    if (!sharedMemory.create(1))
    {
        //argv.append("/mnt/cams/cam4/HikExtracted/NVR/20220909/20220909-155012-20220909-155524-00010000024000213.mp4");
        if (argc > 1) {

            new QtVsPlayerAdaptor(&a);
            QDBusConnection::sessionBus().registerObject("/", &a);
            //emit message(m_nickname, messageLineEdit->text());
            QDBusMessage msg = QDBusMessage::createSignal("/", "local.QtVsPlayer", "message");
            msg << QString(argv[1]);
            QDBusConnection::sessionBus().send(msg);
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

    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    bsuc = myappTranslator.load(QLocale(), QLatin1String("QtVsPlayer"), QLatin1String("_"),
                                "/usr/share/QtVsPlayer/translations");
#if (defined(_WIN32))
    bsuc = myappTranslator.load(QLocale(), QLatin1String("QtVsPlayer"), QLatin1String("_"),
                                QCoreApplication::applicationDirPath());
#endif

    a.installTranslator(&myappTranslator);

    //surpress warning!
    if (bsuc == false ) {
        //I'm happy for yu
    }
    //surpress warning!


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

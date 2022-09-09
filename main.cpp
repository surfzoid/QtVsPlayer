#include "qtvsplayer.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    QThreadPool::globalInstance()->setExpiryTimeout(3000);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("QvSPlayerUniqueInstance1");

    if (!sharedMemory.create(1))
    {
        //argv[1]= "/mnt/cams/cam4/HikExtracted/NVR/20220909/20220909-155012-20220909-155524-00010000024000213.mp4";
        QFile file(QStandardPaths::writableLocation(
                       QStandardPaths::GenericCacheLocation)
                   + "/QtVsPlayer");

        file.open(QIODevice::WriteOnly | QIODevice::Text);

        QString Towrite;
        for (int a = 1; a < 2; ++a) {
            Towrite = QString(argv[a]) + "\n";
            file.write(Towrite.toUtf8());
        }
        file.close();
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

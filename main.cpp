#include "qtvsplayer.h"

#include <QApplication>
#include <QMessageBox>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    QThreadPool::globalInstance()->setExpiryTimeout(3000);
    /*QSharedMemory* shared = new QSharedMemory("62d60yyiassassasaasxss669-bb94-4a9gjhsfgjdhiuj4-8ihi8bb-b9648kjuiujhjhtgjdf90a7e04");


        shared->deleteLater(); // this solved the problem

        if( ! shared->create( 512, QSharedMemory::ReadWrite) )
        {
            QApplication::beep();
            QMessageBox::about(0, "QvSPlayer for Hikvision local records", "QvSPlayer already runing");
            exit(0);
        }
        else
        {*/

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

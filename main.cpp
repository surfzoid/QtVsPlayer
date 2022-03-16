#include "qtvsplayer.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVsPlayer w;
    a.installEventFilter(&w);
    w.show();
    while (w.isEnabled() == false) {

    }
    QStringList list;
    for (int a = 1; a < argc; ++a) {
        list << QString::fromLocal8Bit(argv[a]);
    }
    list.append("/home/eric/Vidéos/hiv00000.mp4");
    if (list.length() > 0) {w.ParseArgs(list);}
    return a.exec();
}

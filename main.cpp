#include "qtvsplayer.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVsPlayer w;
    a.installEventFilter(&w);
    w.show();
    QStringList list;
    for (int a = 0; a < argc; ++a) {
        list << QString::fromLocal8Bit(argv[a]);
    }
    w.ParseArgs(list);
    return a.exec();
}

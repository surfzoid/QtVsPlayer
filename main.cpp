#include "qtvsplayer.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVsPlayer w;
    a.installEventFilter(&w);
    w.show();
    return a.exec();
}

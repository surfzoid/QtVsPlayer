#ifndef QTVSPLAYER_H
#define QTVSPLAYER_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QSlider>
#include "playm4interface.h"
#include "videoctrls.h"
#include "filesliste.h"
#include <QFileDialog>
#include <QtCore>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class QtVsPlayer; }
QT_END_NAMESPACE

class QtVsPlayer : public QMainWindow
{
    Q_OBJECT

public:
    QtVsPlayer(QWidget *parent = nullptr);
    ~QtVsPlayer();
    void KeyPress(QWidget *parent = 0);
    playm4interface *nPlaym4Interface = new playm4interface ();
    void FullScr();
    void HideCtrl();
    VideoCtrls *WVideoCtrls;
    FilesListe *filesLs;
    void ParseArgs(QStringList args);
    void DisplayStatus(QString  StatuTxt);
    void SetWindowTitle(QString Title);
    static int LastPlayIdx;
    void PlayNextFile(bool FromFsList, int idx);
    void WinIdWorkarround();
    static QStringList fileNames;
    void InitPort(int port);

private slots:
    void on_actionOuvrir_triggered();

    void on_actionA_propos_triggered();

    void DisplayError(unsigned int  ErrMess);

    void on_actionPlein_ecran_triggered();

    void on_actionMasquer_les_controles_triggered();

    void on_actionListe_de_lecture_toggled(bool arg1);

    void on_actionListe_de_lecture_triggered();

    void on_actionDossier_triggered();

    void on_actionConsole_triggered();

private:
    Ui::QtVsPlayer *ui;

    QStringList Hist;
    static QString Lastpath;
    static QString Lastfs;
    QString GetmimeType( const QString &filePath );
    void Play (QStringList Files);
    QStringList Scandir( const QString &dir );

    void DisplayFsName(QString Name);

    QRect originH;
    bool Zoomed;


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
#endif // QTVSPLAYER_H

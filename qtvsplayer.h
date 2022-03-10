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
    VideoCtrls *WVideoCtrls = new VideoCtrls (this);
    QSlider * VTimeSlider = WVideoCtrls->findChild<QSlider *>("TimeSlider");
    FilesListe *filesListe = new FilesListe (this);
    QTabWidget* tableWidget_2_localfilist = filesListe->findChild<QTabWidget*>("tableWidget_2_localfilist");
    void ParseArgs(QStringList args);
    void DisplayStatus(QString  StatuTxt);

private slots:
    void on_actionOuvrir_triggered();

    void on_actionA_propos_triggered();

    void DisplayError(unsigned int  ErrMess);

    void on_actionPlein_ecran_triggered();

    void on_actionMasquer_les_controles_triggered();

    void on_actionListe_de_lecture_toggled(bool arg1);

private:
    Ui::QtVsPlayer *ui;

    QFileDialog *FsDialog = new QFileDialog();
    QStringList Hist;
    QString Lastpath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    QStringList fileNames;
    QString GetmimeType( const QString &filePath );
    void Play (QStringList Files);
    QStringList Scandir( const QString &dir );
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
#endif // QTVSPLAYER_H

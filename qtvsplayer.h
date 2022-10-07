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
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class QtVsPlayer; }
QT_END_NAMESPACE

class QtVsPlayer : public QMainWindow
{
    Q_OBJECT

    QStringList m_messages;
public:
    QtVsPlayer(QWidget *parent = nullptr);
    ~QtVsPlayer();
    void KeyPress(QWidget *parent = 0);
    //playm4interface *nPlaym4Interface;
    void FullScr();
    void HideCtrl();
    static VideoCtrls *WVideoCtrls;
    static FilesListe *filesLs;
    static void ParseArgs(QStringList args);
    void SetWindowTitle(QString Title);
    static int LastPlayIdx;
    static void PlayNextFile(bool FromFsList, int idx);
    static void WinIdWorkarround();
    static QStringList fileNames;


private slots:
    void on_actionOuvrir_triggered();

    void on_actionA_propos_triggered();

    void on_actionPlein_ecran_triggered();

    void on_actionMasquer_les_controles_triggered();

    void on_actionListe_de_lecture_triggered();

    void on_actionDossier_triggered();

    void on_actionSettings_triggered();

    void on_actionRtsp_Play_triggered();

    void on_actionBeer_and_Coffee_triggered();

    void on_actionInfos_triggered();

    void messageSlot(const QString &text);

    void actionSlot(const QString &text);
    void ShowHide();

public slots:

signals:
    void message(const QString &text);
    void action(const QString &text);

private:
    Ui::QtVsPlayer *ui;

    QStringList Hist;
    static QString Lastpath;
    static QString Lastfs;
    static QString GetmimeType( const QString &filePath );
    static void Play (QStringList Files);
    static QStringList Scandir( const QString &dir );

    //void DisplayError(unsigned int  ErrMess);

    static int centralWidgetwinId;
    static QVideoWidget *videoWidget;

    void test();

    static void DisplayFsName(QString Name);

    static QWidget  *GetWidgetByName(QString Name);
    //timer for play-process
    QTimer *ShowHideTimer;
/*signals:
static void  FsChanged();*/

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

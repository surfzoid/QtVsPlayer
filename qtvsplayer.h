#ifndef QTVSPLAYER_H
#define QTVSPLAYER_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include "playm4interface.h"
#include "videoctrls.h"

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

private slots:
    void on_actionOuvrir_triggered();

    void on_actionA_propos_triggered();

    void DisplayError(uint32_t ErrMess);

    void on_actionPlein_ecran_triggered();

    void on_actionMasquer_les_controles_triggered();

private:
    Ui::QtVsPlayer *ui;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
#endif // QTVSPLAYER_H

#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include "errormanager.h"
#include"playm4interface.h"
#include <QFileDialog>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMouseEvent>
#include "playm4interface.h"
#include "videoctrls.h"


QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{
    ui->setupUi(this);
    WVideoCtrls->setWindowOpacity(0);
    HideCtrl();
}

QtVsPlayer::~QtVsPlayer()
{
    delete ui;
}

void QtVsPlayer::on_actionOuvrir_triggered()
{
    HideCtrl();
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open video"), "/media/pi/HddFbx", tr("video Files (*.mp4 *.avi *.mkv)"));
    DisplayError(nPlaym4Interface->VideoFs(fileName,this->centralWidget()->winId()));
}

void QtVsPlayer::on_actionA_propos_triggered()
{
    QMessageBox::about(this, "QvSPlayer for Hikvision local records", "QvSPlayer can read local video files of Hikvision and display blue, green an red vector");
}


void QtVsPlayer::DisplayError(uint32_t ErrMess)
{

    QString QerrMess=ErrorManager::error_codes(ErrMess);
    statusBar()->showMessage(QerrMess);
}

void QtVsPlayer::on_actionPlein_ecran_triggered()
{
    FullScr();
}

void QtVsPlayer::keyPressEvent(QKeyEvent *event)
{
    /*if(event->key() == Qt::Key_F11)
    {
       FullScr();
    }*/
    switch (event->key()) {
    case Qt::Key_F11:
        QtVsPlayer().FullScr();
        break;
    case Qt::Key_F10:
        QtVsPlayer().HideCtrl();
        break;
    }
}

void QtVsPlayer::FullScr()
{

    //unsigned int nRegion=0; //this->ui->VideoRead->screen()->size()
    if (isFullScreen()) {
        showMaximized();
        menuBar()->setVisible(true);
        statusBar()->setVisible(true);
    } else {
        showFullScreen();
        menuBar()->setVisible(false);
        statusBar()->setVisible(false);
    }
}

void QtVsPlayer::resizeEvent(QResizeEvent *event)
{

    nPlaym4Interface->SetVideoWin(0,this->winId());
    nPlaym4Interface->RefreshPlay();
    nPlaym4Interface->SetVideoWin(0,this->centralWidget()->winId());
    WVideoCtrls->move(0,this->height() -
                          WVideoCtrls->height() -
                      this->statusBar()->height());
}


void QtVsPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    FullScr();
}

void QtVsPlayer::on_actionMasquer_les_controles_triggered()
{

    HideCtrl();
}

void QtVsPlayer::HideCtrl()
{
    if (this->ui->actionMasquer_les_controles->isChecked()) {
        WVideoCtrls->hide();

    } else {
        WVideoCtrls->show();
        WVideoCtrls->activateWindow();
        WVideoCtrls->raise();
        this->centralWidget()->lower();
        this->centralWidget()->stackUnder(WVideoCtrls);
        //WVideoCtrls->setParent(centralWidget());
    }
}


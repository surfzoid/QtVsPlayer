#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include "errormanager.h"
#include"playm4interface.h"
#include <QFileDialog>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include <QSize>
#include "videoctrls.h"
#include <QMimeDatabase>

QStringList QtVsPlayer::fileNames;
int QtVsPlayer::LastPlayIdx = 0;

playm4interface *QtVsPlayer::nPlaym4Interface = new playm4interface ();
/*
VideoCtrls *QtVsPlayer::WVideoCtrls;
QSlider * QtVsPlayer::VTimeSlider;
FilesListe *QtVsPlayer::filesLs;
QTabWidget* QtVsPlayer::tableWidget_localfilist;
*/

QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{
    ui->setupUi(this);


    QtVsPlayer::WVideoCtrls = new VideoCtrls (this);
    QtVsPlayer::VTimeSlider = WVideoCtrls->findChild<QSlider *>("TimeSlider");
    QtVsPlayer::filesLs = new FilesListe (this);
    QtVsPlayer::tableWidget_localfilist = filesLs->findChild<QTabWidget*>("tableWidget_2_localfilist");

    qApp->installEventFilter(this);
    WVideoCtrls->setWindowOpacity(0);
    //QStringList args = QApplication::arguments();
    //ParseArgs(args);
}

QtVsPlayer::~QtVsPlayer()
{
    delete ui;
}

bool QtVsPlayer::eventFilter(QObject *obj, QEvent *event)
{

    //const QObjectList& list = children(); // or centralwidget->children();
    if(event->type() == QEvent::WinIdChange)
    {
        if(obj->objectName() == "centralwidget")
        {
            qDebug() << "Debug---QEvent::WinIdChange";
            printf("pyd---WinIdChange :%s\n\r",obj->objectName().toUtf8().data());
            nPlaym4Interface->hwnd = this->centralWidget()->winId();

            nPlaym4Interface->RefreshPlay();
        }
    }

    return QObject::eventFilter(obj, event);
}

void QtVsPlayer::ParseArgs(QStringList args)
{
    HideCtrl();

    if (args.length() == 1) {
        QFileInfo fi(args[0].toUtf8());
        if (fi.isDir()) {

            Scandir(fi.filePath().toUtf8());
            if (fileNames.length() > 0) {
                Play (fileNames);
            }

        }
        if (fi.isFile()) {
            fileNames.append(args[0].toUtf8());
            Play (fileNames);
        }
    }

    if (args.length() > 1) {
        Play (args);
    }
    if (nPlaym4Interface->hwnd == 0) {
        nPlaym4Interface->hwnd = this->centralWidget()->winId();
        nPlaym4Interface->SetVideoWin(0);
        nPlaym4Interface->RefreshPlay();
    }
}


QString QtVsPlayer::GetmimeType( const QString &filePath )
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QMimeType Mime = QMimeDatabase().mimeTypeForFile(filePath );
    qDebug() << "Debug---File:" << filePath.toUtf8().data();
    qDebug() << "Debug---Mime type:" << Mime.name().toUtf8().data();
    //printf("pyd---File :%s\n\r",filePath.toUtf8().data());
    //printf("pyd---mime type:%s\n\r",Mime.name().toUtf8().data());
    return Mime.name();
#endif
}

QStringList QtVsPlayer::Scandir( const QString &dir2scan )
{
    QDir dir{dir2scan};
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::DirsFirst);
    if (dir.isEmpty() == false) {

        QFileInfoList list = dir.entryInfoList();
        QString MimeIs;

        for (int i=0; i<list.length(); i++)
        {
            MimeIs = GetmimeType(list[i].absoluteFilePath());
            if (MimeIs.contains("directory")) {
                Scandir(list[i].absoluteFilePath());
            }
            if (MimeIs.contains("video")) {
                if (fileNames.contains(list[i].absoluteFilePath()) == false) {
                    fileNames.append(list[i].absoluteFilePath());
                }

            }
        }
        qDebug("Debug---Found :%d files",list.length());
    }
    return fileNames;
}

void QtVsPlayer::on_actionOuvrir_triggered()
{
    HideCtrl();

    Hist = FsDialog->history();
    fileNames = FsDialog->getOpenFileNames(this,
                                           tr("Open video"), Lastpath,
                                           tr("video Files (*.mp4 *.avi *.mkv)"));
    if (fileNames.length() > 0) {

        Lastpath = fileNames[0].toUtf8();
        Play (fileNames);
    }
}

void QtVsPlayer::Play (QStringList Files)
{

    filesLs->Populate(Files);
    WVideoCtrls->PLast = false;
    PlayNextFile(false,0);

}

void QtVsPlayer::PlayNextFile(bool FromFsList, int idx)
{
    WVideoCtrls->InitTimeSlide();
    if (LastPlayIdx > fileNames.length()) {
        WVideoCtrls->PLast = true;
    }
    if (FromFsList) {
        LastPlayIdx = idx;
    }
    if (LastPlayIdx < fileNames.length()) {
        // this->ui->frame->setEnabled(0);

        DisplayError(nPlaym4Interface->VideoFs(
                         fileNames[LastPlayIdx]));
        LastPlayIdx += 1;
    }

    if (LastPlayIdx == fileNames.length()) {
        LastPlayIdx += 1;
    }
    //window()->resize(WinSize);
}

void QtVsPlayer::on_actionA_propos_triggered()
{
    QMessageBox::about(this, "QvSPlayer for Hikvision local records", "QvSPlayer can read local video files of Hikvision and display blue, green an red vector");
}


void QtVsPlayer::DisplayError(unsigned int  ErrMess)
{

    QString QerrMess=ErrorManager::error_codes(ErrMess);
    this->ui->statusbar->showMessage(QerrMess);
    //this->ui->statusbar->update();
    //this->ui->statusbar->repaint();
}

void QtVsPlayer::DisplayStatus(QString  StatuTxt)
{

    //this->ui->statusbar->clearMessage();
    this->ui->statusbar->showMessage(tr(StatuTxt.toUtf8()), 2000);
    //this->ui->statusbar->update();
    //this->ui->statusbar->repaint();

}

void QtVsPlayer::on_actionPlein_ecran_triggered()
{
    FullScr();
}

void QtVsPlayer::keyPressEvent(QKeyEvent *event)
{
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
        //this->ui->frame->showMaximized();
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
    WinIdWorkarround();
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


void QtVsPlayer::on_actionListe_de_lecture_toggled(bool checked)
{

    if (checked) {
        filesLs->show();
    } else {
        filesLs->hide();
    }
}

void QtVsPlayer::SetWindowTitle(QString Title)
{
    this->setWindowTitle(Title.toUtf8());
    this->update();
    //this->repaint();
}

void QtVsPlayer::WinIdWorkarround()
{
    if (this->isEnabled()) {

        if (this->centralWidget())
        {

            nPlaym4Interface->hwnd = this->winId();

            nPlaym4Interface->SetVideoWin(0);
            nPlaym4Interface->RefreshPlay();
            nPlaym4Interface->hwnd = this->centralWidget()->winId();
            nPlaym4Interface->SetVideoWin(0);
            nPlaym4Interface->RefreshPlay();
        }
    }
}

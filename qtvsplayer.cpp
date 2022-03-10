#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include "errormanager.h"
#include"playm4interface.h"
#include <QFileDialog>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include "videoctrls.h"
#include <QMimeDatabase>


QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{
    ui->setupUi(this);
    WVideoCtrls->setWindowOpacity(0);
    //QStringList args = QApplication::arguments();
    //ParseArgs(args);
}

QtVsPlayer::~QtVsPlayer()
{
    delete ui;
}
void QtVsPlayer::ParseArgs(QStringList args)
{
    HideCtrl();
    //QStringList args = QCoreApplication::arguments();
    // Toujours tester si le paramètre existe, avant de l'utiliser
    /*QString nom_fichier = (args.length() > 1 ? args[1] : "");
    qDebug("pyd---File name arg:%s",nom_fichier.data());*/

    args.append("/home/eric/Vidéos/Hikvision/");
    if (args.length() == 2) {
        QFileInfo fi(args[1].toUtf8());
        if (fi.isDir()) {

            fileNames.clear();
            QStringList ConstructFsList = Scandir(fi.filePath());
            if (ConstructFsList.length() > 0) {
                Play (ConstructFsList);
            }

        }
        if (fi.isFile()) {
            fileNames.append(args[1]);
            Play (fileNames);
        }
    }

    if (args.length() > 2) {
        args.removeAt(0);
        Play (args);
    }
}


QString QtVsPlayer::GetmimeType( const QString &filePath )
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QMimeType Mime = QMimeDatabase().mimeTypeForFile(filePath );
    qDebug() << "File:" << filePath.toUtf8().data();
    qDebug() << "Mime type:" << Mime.name().toUtf8().data();
    printf("pyd---File :%s\n\r",filePath.toUtf8().data());
    printf("pyd---mime type:%s\n\r",Mime.name().toUtf8().data());
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
                fileNames.append(Scandir(list[i].absoluteFilePath()));
            }
            if (MimeIs.contains("video")) {
                fileNames.append(list[i].absoluteFilePath());
            }
        }
        qDebug("pyd---Found :%d files",list.length());
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

    filesListe->Populate(Files);
    nPlaym4Interface->m_pbframe = this->centralWidget();
    //WVideoCtrls->t
    DisplayError(nPlaym4Interface->VideoFs(Files,this->centralWidget()->winId()));
}
void QtVsPlayer::on_actionA_propos_triggered()
{
    QMessageBox::about(this, "QvSPlayer for Hikvision local records", "QvSPlayer can read local video files of Hikvision and display blue, green an red vector");
}


void QtVsPlayer::DisplayError(unsigned int  ErrMess)
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


void QtVsPlayer::on_actionListe_de_lecture_toggled(bool checked)
{

    if (checked) {
        filesListe->show();
    } else {
        filesListe->hide();
    }
}

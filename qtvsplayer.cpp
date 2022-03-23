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

QString QtVsPlayer::Lastpath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
QString QtVsPlayer::Lastfs = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

static int eventEnumIndex = QEvent::staticMetaObject
      .indexOfEnumerator("Type");

QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{
    ui->setupUi(this);



    //if (QtVsPlayer::filesLs == nullptr) {
        QtVsPlayer::filesLs = new FilesListe (this);
    //}

    //if (QtVsPlayer::nPlaym4Interface == nullptr) {
        //QtVsPlayer::nPlaym4Interface = new playm4interface ();
    //}


    //if (QtVsPlayer::WVideoCtrls == nullptr) {
        QtVsPlayer::WVideoCtrls = new VideoCtrls (this);
        qApp->installEventFilter(this);
        //WVideoCtrls->setWindowOpacity(0);
        ui->statusbar->addPermanentWidget(ui->SatusLbl,1);
    //}
    //if (ArgLS.length() > 0) {ParseArgs(ArgLS);}

}

QtVsPlayer::~QtVsPlayer()
{
    /*playm4interface::Stop();
    playm4interface::FreePort();
    playm4interface::~playm4interface();*/
    if (filesLs != NULL)
    {
        delete filesLs;
        filesLs = NULL;
    }
    delete ui;
}

bool QtVsPlayer::eventFilter(QObject *obj, QEvent *event)
{

    if(event->type() == QEvent::WinIdChange)
    {
        printf("pyd---WinIdChange :%s\n\r",obj->objectName().toUtf8().data());
        if(obj->objectName() == "centralwidget")
        {

            printf("pyd---WinIdChange :%s\n\r",obj->objectName().toUtf8().data());
            playm4interface::hwnd = this->centralWidget()->winId();

            playm4interface::RefreshPlay();
        }
    }

    if(event->type() == 68)
    {
        if (Zoomed == false) {

            DisplayFsName(Lastfs);
        }
    }

/*printf("pyd---Event type %i :%s\n\r", event->type(), QEvent::staticMetaObject
       .enumerator(eventEnumIndex).valueToKey(event->type()));*/

    return QObject::eventFilter(obj, event);
}

void QtVsPlayer::ParseArgs(QStringList args)
{
    //HideCtrl();
    playm4interface::SetPort();
    if (playm4interface::hwnd == 0) {
        playm4interface::hwnd = this->centralWidget()->winId();
        playm4interface::SetVideoWin(0);
        playm4interface::RefreshPlay();
    }

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
        //fi.~QFileInfo();
    }

    if (args.length() > 1) {
        Play (args);
    }


    return;
}


QString QtVsPlayer::GetmimeType( const QString &filePath )
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QMimeType Mime = QMimeDatabase().mimeTypeForFile(filePath );
    printf("Debug---File:%s/n/r" , filePath.toUtf8().data());
    printf("Debug---Mime type:%s/n/r",Mime.name().toUtf8().data());
    return Mime.name();
#endif
    return "";
}

QStringList QtVsPlayer::Scandir( const QString &dir2scan )
{
    QDir dir(dir2scan);
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
        printf("Debug---Found :%d files",list.length());

    }
    return fileNames;
}

void QtVsPlayer::on_actionOuvrir_triggered()
{
    HideCtrl();

    QFileDialog *FsDialog = new QFileDialog();
    Hist = FsDialog->history();
    fileNames = FsDialog->getOpenFileNames(this,
                                           tr("Open video"), Lastpath,
                                           tr("video Files (*.mp4 *.avi *.mkv)"));
    if (fileNames.length() > 0) {

        playm4interface::SetPort();
        Lastpath = fileNames[0].toUtf8();
        Play (fileNames);
    }
    return;
}

void QtVsPlayer::Play (QStringList Files)
{
    filesLs->Populate(Files);
    WVideoCtrls->PLast = false;
    LastPlayIdx = 0 ;
    PlayNextFile(false,0);
    return;
}

void QtVsPlayer::PlayNextFile(bool FromFsList, int idx)
{
    if (fileNames.length() < LastPlayIdx) {
        LastPlayIdx = 0;
    }

    if (LastPlayIdx < 0 and fileNames.length() > 0) {
        LastPlayIdx = fileNames.length() - 1;
    }

    WVideoCtrls->InitTimeSlide();
    if (LastPlayIdx > fileNames.length()) {
        WVideoCtrls->PLast = true;
    }
    if (FromFsList) {
        LastPlayIdx = idx;
    }
    if (LastPlayIdx >= 0 and LastPlayIdx < fileNames.length()) {

        playm4interface::VideoFs(
                         fileNames[LastPlayIdx]);
        WVideoCtrls->RestoreSeek();

        if (fileNames.length() > 0) {
            QStringList Colom = fileNames[LastPlayIdx].split("-");

            if (Colom.length() < 4) {
                Lastfs = fileNames[LastPlayIdx];

            }
            else
            {
                QString tt = Colom[1];
                QString starttime = tt.left(2) + ":" + tt.mid(2,2) + ":" + tt.right(2);

                QString st = Colom[3];
                QString stoptime = st.left(2) + ":" + st.mid(2,2) + ":" + st.right(2);

                QString dd = Colom[2];
                QString Day = dd.left(4) + "/" + dd.mid(4,2) + "/" + dd.right(2);

                Lastfs = Colom[4] + " from " + starttime + " to " + stoptime + " at " + Day;

            }
        }

        LastPlayIdx += 1;
    }

    if (LastPlayIdx == fileNames.length()) {
        LastPlayIdx += 1;
    }
    return;
}

void QtVsPlayer::DisplayFsName(QString Name)
{
    ui->SatusLbl->setStyleSheet("font-weight: bold; color: red");
    this->ui->statusbar->setToolTip(Name.toUtf8());
    /*this->ui->statusbar->clearMessage();
    this->ui->statusbar-> showMessage(Name.toUtf8());*/
    ui->SatusLbl->setText(Name.toUtf8());
    //printf("pyd---currentMessage :%s\n\r",ui->SatusLbl->text().toUtf8().data());

    return;

}

void QtVsPlayer::on_actionA_propos_triggered()
{
    QMessageBox::about(this, tr("QvSPlayer for Hikvision local records"), tr("QvSPlayer can read local video files of Hikvision and display blue, green an red vector"));
    return;
}


/*void QtVsPlayer::DisplayError(unsigned int  ErrMess)
{

    QString QerrMess=ErrorManager::error_codes(ErrMess);
    //this->ui->statusbar->showMessage(QerrMess);
    printf("pyd---Hik Sdk error response :%s\n\r",QerrMess.toUtf8().data());
    return;

}*/

void QtVsPlayer::DisplayStatus(QString  StatuTxt)
{

    //this->ui->statusbar->clearMessage();
    this->ui->statusbar->showMessage(tr(StatuTxt.toUtf8()), 2000);
    //this->ui->statusbar->update();
    //this->ui->statusbar->repaint();
    return;

}

void QtVsPlayer::on_actionPlein_ecran_triggered()
{
    FullScr();
    return;
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
    return;
}

void QtVsPlayer::FullScr()
{
    if (isFullScreen()) {
        showMaximized();
        menuBar()->setVisible(true);
        statusBar()->setVisible(true);
    } else {
        showFullScreen();
        menuBar()->setVisible(false);
        statusBar()->setVisible(false);
    }
    return;
}

void QtVsPlayer::resizeEvent(QResizeEvent *event)
{
    WinIdWorkarround();
    WVideoCtrls->move(0,this->height() -
                      WVideoCtrls->height() -
                      this->statusBar()->height());

    //this->ui->FsDisplay->setVisible(false);
    originH = ui->centralwidget->rect();
    Zoomed = false;
    return;
}


void QtVsPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    FullScr();
    return;
}

void QtVsPlayer::mousePressEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::RightButton)) {
        this->ui->actionMasquer_les_controles->setChecked(!this->ui->actionMasquer_les_controles->isChecked());
        HideCtrl();

    }

    if ((event->buttons() == Qt::LeftButton)) {

    }
    return;
}


void QtVsPlayer::wheelEvent(QWheelEvent *event)
{
    QPoint Scroll = event->angleDelta();

    if (Scroll.y() > 0)
    {
        ui->centralwidget->resize(ui->centralwidget->width() + 50,
                                  ui->centralwidget->height() + 50);
        Zoomed = true;

    }

    if (Scroll.y() < 0 and originH.height() < ui->centralwidget->height())
    {
        ui->centralwidget->resize(ui->centralwidget->width() - 50,
                                  ui->centralwidget->height() - 50);
    }

    if (Scroll.y() < 0 and originH.height() == ui->centralwidget->height())
    {
        Zoomed = false;
        ui->centralwidget->move(originH.x(), originH.y());
    }

    WinIdWorkarround();
    return;
}

void QtVsPlayer::mouseMoveEvent(QMouseEvent *event)
{
    if (Zoomed == true and event->buttons() == Qt::LeftButton) {
        ui->centralwidget->move(event->pos().x() - ui->centralwidget->width()/2,
                                event->pos().y() - ui->centralwidget->height()/2);

        WinIdWorkarround();
    }
    return;
}

void QtVsPlayer::on_actionMasquer_les_controles_triggered()
{

    HideCtrl();
    return;
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
    return;
}


void QtVsPlayer::on_actionListe_de_lecture_toggled(bool checked)
{

    if (checked) {
        filesLs->show();
    } else {
        filesLs->hide();
    }
    return;

}

void QtVsPlayer::SetWindowTitle(QString Title)
{
    this->setWindowTitle(Title.toUtf8());
    this->update();
    //this->repaint();
    return;
}

void QtVsPlayer::WinIdWorkarround()
{
    if (this->isEnabled()) {

        if (this->centralWidget())
        {

            playm4interface::hwnd = this->winId();

            playm4interface::SetVideoWin(0);
            playm4interface::RefreshPlay();
            playm4interface::hwnd = this->centralWidget()->winId();
            playm4interface::SetVideoWin(0);
            playm4interface::RefreshPlay();
        }
    }
    return;
}

void QtVsPlayer::on_actionListe_de_lecture_triggered()
{
    filesLs->show();
    return;
}

void QtVsPlayer::on_actionDossier_triggered()
{
    QFileDialog *FsDialog = new QFileDialog();

    QString _IntputFolder = FsDialog->getExistingDirectory(this,
                                                           (tr("Select Folder to read")), Lastpath);
    Hist = FsDialog->history();

    if (_IntputFolder.isEmpty() == false) {
        fileNames.clear();
        fileNames = Scandir(_IntputFolder.toUtf8());
    }

    if (fileNames.length() > 0) {
        playm4interface::SetPort();
        Play (fileNames);
        Lastpath = _IntputFolder;
    }
    return;
}

void QtVsPlayer::InitPort(int port)
{
    VideoCtrls::HikNumPort = port;
    return;
}

void QtVsPlayer::on_actionConsole_triggered()
{
    ui->SatusLbl->setText("display" + Lastpath);
    return;
}

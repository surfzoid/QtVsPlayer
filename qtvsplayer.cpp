#include "qtvsplayer.h"
#include "ui_qtvsplayer.h"
#include "errormanager.h"
#include "playm4interface.h"
#include "settingsform.h"
#include "rtspwindow.h"
#include <QFileDialog>
#include <QtWidgets>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include <QSize>
#include "videoctrls.h"
#include <QMimeDatabase>
#include <QFileSystemWatcher>
#include "qtvsplayer_adaptor.h"
#include "qtvsplayer_interface.h"

QStringList QtVsPlayer::fileNames;
int QtVsPlayer::LastPlayIdx = 0;
int QtVsPlayer::centralWidgetwinId = 0;

QString QtVsPlayer::Lastpath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
QString QtVsPlayer::Lastfs = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

FilesListe *QtVsPlayer::filesLs;
VideoCtrls *QtVsPlayer::WVideoCtrls;
QVideoWidget *QtVsPlayer::videoWidget;

static int eventEnumIndex = QEvent::staticMetaObject
        .indexOfEnumerator("Type");

static QRect originH = *new QRect;
static bool Zoomed = false;


QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{

    QtVsPlayer::filesLs = new FilesListe (this);

    QtVsPlayer::WVideoCtrls = new VideoCtrls (this);
    QtVsPlayer::videoWidget = new QVideoWidget;

    ui->setupUi(this);

    //setCentralWidget(videoWidget);
    videoWidget->setContentsMargins(0,0,0,0);

    ui->statusbar->addPermanentWidget(ui->SatusLbl,1);

    QString CpuArch = QSysInfo::buildCpuArchitecture();
    printf("pyd---buildCpuArchitecture :%s\n\r",CpuArch.toUtf8().data());

    //settings//
    QCoreApplication::setOrganizationName("Surfzoid");
    QCoreApplication::setOrganizationDomain("https://github.com/surfzoid");
    QCoreApplication::setApplicationName("QtVsPlayer");
    //settings//

    qApp->installEventFilter(this);

    centralWidgetwinId = this->centralWidget()->winId();

    QStringList ArgLS = QApplication::arguments();
    if (ArgLS.length() > 0)
    {
        ArgLS.removeAt(0);
        ParseArgs(ArgLS);
    }

    WVideoCtrls->raise();

    ShowHideTimer = new QTimer(this);
    ShowHideTimer->setTimerType(Qt::PreciseTimer);
    //connect(m_pbqtimer, &QTimer::timeout, this, &VideoCtrls::updatelocalprocess);
    ShowHideTimer->start( 10000 );
    QtVsPlayer::connect(ShowHideTimer, SIGNAL(timeout()), this, SLOT(ShowHide()));

    new QtVsPlayerAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);

    local::QtVsPlayer *iface;
    iface = new local::QtVsPlayer(QString(), QString(), QDBusConnection::sessionBus(), this);
    //connect(iface, SIGNAL(message(QString)), this, SLOT(messageSlot(QString)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "local.QtVsPlayer", "message", this, SLOT(messageSlot(QString)));
    connect(iface, SIGNAL(action(QString)), this, SLOT(actionSlot(QString)));
}

QtVsPlayer::~QtVsPlayer()
{
    playm4interface::FreePort();
    /*playm4interface::~playm4interface();*/
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

            //DisplayFsName(Lastfs);
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
        playm4interface::hwnd = centralWidgetwinId;
        playm4interface::SetVideoWin(0);
        playm4interface::RefreshPlay();
    }

    for (int i=0; i<args.length(); i++)
    {
        QFileInfo fi(args[i].toUtf8());
        if (fi.isDir()) {

            Scandir(fi.filePath().toUtf8());
            if (fileNames.length() > 0) {
                Play (fileNames);
            }

        }
        if (fi.isFile()) {
            fileNames.append(args[i].toUtf8());
            Play (fileNames);
        }

    }

    /*if (args.length() > 1) {
        Play (args);
    }*/


    return;
}


QString QtVsPlayer::GetmimeType( const QString &filePath )
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QMimeType Mime = QMimeDatabase().mimeTypeForFile(filePath );
    printf("Debug---File:%s\n\r" , filePath.toUtf8().data());
    printf("Debug---Mime type:%s\n\r",Mime.name().toUtf8().data());
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

        QFileInfoList list(dir.entryInfoList());
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
        printf("Debug---Found :%d files\n\r",list.length());

    }

    return fileNames;
}

void QtVsPlayer::on_actionOuvrir_triggered()
{
    HideCtrl();

    fileNames = QFileDialog::getOpenFileNames(this,
                                              tr("Open video"), Lastpath,
                                              tr("video Files (*.mp4 *.avi *.mkv)"));
    if (fileNames.length() > 0) {

        //playm4interface::SetPort();
        Lastpath = fileNames[0].toUtf8();
        Play (fileNames);
    }
    return;
}

void QtVsPlayer::Play (QStringList Files)
{
    filesLs->Populate(Files,true);
    VideoCtrls::PLast = false;
    LastPlayIdx = 0 ;
    playm4interface::SetPort();
    PlayNextFile(false,0);

    return;
}

void QtVsPlayer::PlayNextFile(bool FromFsList, int idx)
{
    if (FromFsList) {
        LastPlayIdx = idx;
    }


    if (LastPlayIdx == fileNames.length()) {
        LastPlayIdx += 1;
    }

    if (fileNames.length() < LastPlayIdx) {
        LastPlayIdx = 0;
    }

    if (LastPlayIdx < 0 and fileNames.length() > 0) {
        LastPlayIdx = fileNames.length() - 1;
    }

    WVideoCtrls->InitTimeSlide();
    if (LastPlayIdx > fileNames.length()) {
        VideoCtrls::PLast = true;
    }

    if (LastPlayIdx >= 0 and LastPlayIdx < fileNames.length()) {

        playm4interface::VideoFs(
                    fileNames[LastPlayIdx]);

        if (fileNames.length() > 0) {
            QStringList Colom = fileNames[LastPlayIdx].split("-");

            if (Colom.length() < 5) {
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

                VideoCtrls::Duration = PlayM4_GetFileTime(playm4interface::m_pblocalportnum);
            }
        }

        filesLs->SelCurPlay(LastPlayIdx);
        LastPlayIdx += 1;
    }



    DisplayFsName(Lastfs);
    return;
}

void QtVsPlayer::DisplayFsName(QString Name)
{
    if (Name != QStandardPaths::writableLocation(QStandardPaths::MoviesLocation))

    {
        QWidget *pWin = GetWidgetByName("QtVsPlayer");
        QLabel *SatusLbl = pWin->findChild<QLabel *>("SatusLbl");
        QStatusBar *statusbar = pWin->findChild<QStatusBar *>("statusbar");
        SatusLbl->setStyleSheet("font-weight: bold; color: red");
        statusbar->setToolTip(Name.toUtf8());
        /*this->ui->statusbar->clearMessage();
    this->ui->statusbar-> showMessage(Name.toUtf8());*/
        SatusLbl->setText(Name.toUtf8());
        //printf("pyd---currentMessage :%s\n\r",ui->SatusLbl->text().toUtf8().data());

    }
    return;

}

QWidget  *QtVsPlayer::GetWidgetByName(QString Name)
{
    const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (widget->objectName() == Name)
            return widget;
    }
    return nullptr;
}

void QtVsPlayer::on_actionA_propos_triggered()
{
    QString Vers = APP_VERSION;
    QMessageBox::about(this, tr("QvSPlayer for Hikvision local records."), tr("QvSPlayer can read local video files of Hikvision and display blue, green an red vector.") +  "\n\r\n\rVersion " + Vers.toUtf8());
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
        QtVsPlayer::FullScr();
        break;
    case Qt::Key_F10:
        QtVsPlayer::HideCtrl();
        break;
    case Qt::Key_Space :
    case Qt::Key_Shift:
        VideoCtrls::pause();
        break;
    }
    int Cod = event->key();
    QString result = QString::number( Cod, 16 );
    qDebug() << "currentKeyPressed 0X" + result.toUpper();
    //    printf("pyd---currentKeyPressed :%s\n\r",result.toUpper().data());
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
    if (playm4interface::m_pblocalportnum)WinIdWorkarround();
        WVideoCtrls->move(0,this->height() -
                          WVideoCtrls->height() -
                          this->statusBar()->height());

        //this->ui->FsDisplay->setVisible(false);
        originH = ui->centralwidget->rect();
        Zoomed = false;

    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

    return ;
}


void QtVsPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    FullScr();

    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

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
    if (WVideoCtrls->underMouse()) {
        return;
    }

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


    if (!this->ui->actionMasquer_les_controles->isChecked() and
            WVideoCtrls->isHidden() and
            this->ui->actionAuto_hide_controls->isChecked()) {

        WVideoCtrls->show();
        WVideoCtrls->activateWindow();
        WVideoCtrls->raise();
        this->centralWidget()->lower();
        this->centralWidget()->stackUnder(WVideoCtrls);
        //WVideoCtrls->setParent(centralWidget());
    }

    if (QtVsPlayer::cursor() == Qt::BlankCursor) {
        QtVsPlayer::unsetCursor();
    }
    return;
}

void QtVsPlayer::ShowHide()
{

    if (!this->ui->actionMasquer_les_controles->isChecked() and
            !WVideoCtrls->isHidden() and
            this->ui->actionAuto_hide_controls->isChecked() and
            !WVideoCtrls->underMouse() ) {
        WVideoCtrls->hide();
    }

    if (QtVsPlayer::cursor() != Qt::BlankCursor and
            !WVideoCtrls->underMouse()) {
        QtVsPlayer::setCursor(Qt::BlankCursor);
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


void QtVsPlayer::SetWindowTitle(QString Title)
{
    this->setWindowTitle(Title.toUtf8());
    this->update();
    //this->repaint();
    return;
}

void QtVsPlayer::WinIdWorkarround()
{
    /*if (this->isEnabled()) {

        if (this->centralWidget())
        {*/

    playm4interface::hwnd = 0;

    playm4interface::SetVideoWin(0);
    playm4interface::RefreshPlay();
    playm4interface::hwnd = centralWidgetwinId;
    playm4interface::SetVideoWin(0);
    playm4interface::RefreshPlay();

#if (defined(__linux__))
    PlayM4_WndResolutionChange(playm4interface::m_pblocalportnum);
#endif
    //}
    //}
    return;
}

void QtVsPlayer::on_actionListe_de_lecture_triggered()
{
    QtVsPlayer::filesLs->show();
    QtVsPlayer::filesLs->activateWindow();
    QtVsPlayer::filesLs->raise();
    return;
}

void QtVsPlayer::on_actionDossier_triggered()
{

    QUrl _IntputFolder = QFileDialog::getExistingDirectory(this,
                                                           (tr("Select Folder to read")), Lastpath);
    //Hist = FsDialog->history();

    if (_IntputFolder.isEmpty() == false) {
        fileNames.clear();
        fileNames = Scandir(_IntputFolder.path().toUtf8());
    }

    if (fileNames.length() > 0) {
        //playm4interface::SetPort();
        Play (fileNames);
        Lastpath = _IntputFolder.path();
    }
    return;
}


void QtVsPlayer::on_actionSettings_triggered()
{
    SettingsForm *setTing = new SettingsForm();
    setTing->setWindowModality(Qt::WindowModality::WindowModal);
    setTing->show();

}

void QtVsPlayer::on_actionRtsp_Play_triggered()
{
    RtspWindow *RtspView = new RtspWindow (this);
    RtspView->showMaximized();
}

void QtVsPlayer::on_actionBeer_and_Coffee_triggered()
{
    QString Paypal = "<a href='https://paypal.me/EricPetit?'>Paypal</a>\n\r";
    QString Tip = "<a href='https://fr.tipeee.com/eric-utopiste'>Tip</a>\n\r";
    QMessageBox::about(this, tr("Enjoy!"),
                       Paypal +
                       " or \n\r" + Tip +
                       "\n\r" +
                       tr("If you like my job and be happy to buy me a beer or a coffee, you can do it here."));

    return;
}

void QtVsPlayer::test()
{
    foreach (const QAction *Act, ui->menuAide->actions()) {

        printf("menu name :%s\n\r",Act->text().toUtf8().data());
    }

}

void QtVsPlayer::on_actionInfos_triggered()
{
    playm4interface::GetMetadatas();
}

void QtVsPlayer::messageSlot(const QString &text)
{
    QString msg( QLatin1String("<%1> %2") );
    //msg = msg.arg(nickname, text);
    //m_messages.append(text);
    qDebug() << "New file catched  : " << text <<"\n";
    QFileInfo fi(text.toUtf8());
    if (fi.isDir()) {

        Scandir(fi.filePath().toUtf8());
        if (fileNames.length() > 0) {
            Play (fileNames);
        }

    }
    if (fi.isFile()) {
        fileNames.append(text.toUtf8());
        Play (fileNames);
    }

    fileNames.removeDuplicates();
    filesLs->Populate(fileNames,true);

}

void QtVsPlayer::actionSlot(const QString &text)
{
    QString msg( QLatin1String("<%1> %2") );
    //msg = msg.arg(nickname, text);
    m_messages.append(text);

}

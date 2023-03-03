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

#if (!defined(__ANDROID__))
#include "qtvsplayer_adaptor.h"
#include "qtvsplayer_interface.h"
#endif

QStringList QtVsPlayer::fileNames;
int QtVsPlayer::LastPlayIdx = 0;
int QtVsPlayer::centralWidgetwinId = 0;

QString QtVsPlayer::Lastpath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
QString QtVsPlayer::Lastfs = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

FilesListe *QtVsPlayer::filesLs;
VideoCtrls *QtVsPlayer::WVideoCtrls;
//QVideoWidget *QtVsPlayer::videoWidget;
QVideoWidget *QtVsPlayer::VideoView;

static int eventEnumIndex = QEvent::staticMetaObject
        .indexOfEnumerator("Type");

static QRect originH = *new QRect;
static bool Zoomed = false;
static bool Fullscr = false;//why isFullscreen is not always true in fullscreen?!

QtVsPlayer::QtVsPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtVsPlayer)
{

    QtVsPlayer::filesLs = new FilesListe(this);

    QtVsPlayer::WVideoCtrls = new VideoCtrls(this);
    ui->setupUi(this);

    //QtVsPlayer::videoWidget = new QVideoWidget(graphicsView);

    //ui->VideoView->setContentsMargins(0,0,0,0);
    //ui->VideoView->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->statusbar->addPermanentWidget(ui->SatusLbl,1);

    printf("---QtVsPlayer %s\n\r",APP_VERSION);

    QString CpuArch = QSysInfo::buildCpuArchitecture();
    printf("---buildCpuArchitecture :%s\n\r",CpuArch.toUtf8().data());

    //settings//
    QCoreApplication::setOrganizationName("Surfzoid");
    QCoreApplication::setOrganizationDomain("https://github.com/surfzoid");
    QCoreApplication::setApplicationName("QtVsPlayer");

    QSettings settings;
    settings.beginGroup("QtVsPlayer");
    QPoint NewPos;
    QSize NewSize;
    NewPos.setX(settings.value("X", x()).value<int>());
    NewPos.setY(settings.value("Y", y()).value<int>());
    NewSize.setWidth(settings.value("Width", width()).value<int>());
    NewSize.setHeight(settings.value("Height", height()).value<int>());
    setWindowState(windowState() ^ settings.value("State", Qt::WindowNoState).value<Qt::WindowStates>());

    settings.endGroup();
    this->move(NewPos);
    this->resize(NewSize);


    settings.beginGroup("VideoCtrls");
    //int ThisY = this->height() - WVideoCtrls->height() - this->statusBar()->height();
    NewPos.setX(settings.value("X", width()/2).value<int>());
    if(NewPos.x()<= 0)NewPos.setX(width()/2);
    NewPos.setY(settings.value("Y", height()/2).value<int>());
    if(NewPos.y()<= 0)NewPos.setY(height()/2);
    NewSize.setWidth(settings.value("Width", WVideoCtrls->width()).value<int>());
    NewSize.setHeight(settings.value("Height", WVideoCtrls->height()).value<int>());

    settings.endGroup();
    WVideoCtrls->move(NewPos);
    //settings//

    installEventFilter(this);


#if (!defined(__ANDROID__))
    new QtVsPlayerAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);

    local::QtVsPlayer *iface;
    iface = new local::QtVsPlayer(QString(), QString(), QDBusConnection::sessionBus(), this);
    //connect(iface, SIGNAL(message(QString)), this, SLOT(messageSlot(QString)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "local.QtVsPlayer", "message", this, SLOT(messageSlot(QString)));
    connect(iface, SIGNAL(action(QString)), this, SLOT(actionSlot(QString)));
#endif

    setCentralWidget(ui->scrollArea);
}

void QtVsPlayer::showEvent(QShowEvent *event)
{
    ui->actionQuit->setShortcut(Qt::ALT + Qt::Key_F4);
    //VideoView = new QDialog(this);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::black);
    VideoView = new QVideoWidget(this);
    VideoView->setPalette(pal);
    centralWidgetwinId = VideoView->winId();
    VideoView->setMouseTracking(true);
    VideoView->setObjectName("VideoView");
    m_scale = 1.0;
    ui->scrollArea->setWidget(VideoView);
    ui->scrollArea->setWidgetResizable(false);
    //ui->scrollArea->setMouseTracking(true);
    VideoView->resize(ui->scrollArea->size());
    originH = VideoView->rect();
    this->ui->scrollArea->setStyleSheet("background-color: black;");

    QStringList ArgLS = QApplication::arguments();
    //ArgLS.append("/mnt/cams/cam4/NVR/20230117");
    if (event and ArgLS.length() > 0)
    {
        ArgLS.removeAt(0);
        ParseArgs(ArgLS);
    }

    /*QList<QWidget *> widgets = QtVsPlayer::findChildren<QWidget *>();
    foreach (QWidget *var, widgets) {
        var->setMouseTracking(true);
        var->setAutoFillBackground(true);
    }*/

    GetMenuItemState(ui->menuAffichage);

}

QtVsPlayer::~QtVsPlayer()
{
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
        if(obj->objectName() == "VideoView")
        {
            printf("---WinIdChange :%s\n\r",obj->objectName().toUtf8().data());
            playm4interface::hwnd = VideoView->winId();
            centralWidgetwinId = playm4interface::hwnd;
        }
    }

    /*printf("---Event type %i :%s\n\r", event->type(), QEvent::staticMetaObject
       .enumerator(eventEnumIndex).valueToKey(event->type()));*/

    return QObject::eventFilter(obj, event);
}

void QtVsPlayer::ParseArgs(QStringList args)
{
    if (playm4interface::hwnd == 0) {
        playm4interface::hwnd = centralWidgetwinId;
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
    //playm4interface::SetPort();
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
    WinIdWorkarround();
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
        //printf("---currentMessage :%s\n\r",ui->SatusLbl->text().toUtf8().data());

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
    /*case 0X1000012:
        VideoCtrls::on_previousButton_released();
        break;
    case 0X1000012:
        VideoCtrls::on_previousButton_released();
        break;
    case 0X1000012:
        VideoCtrls::on_previousButton_released();
        break;
    case 0X1000012:
        VideoCtrls::on_previousButton_released();
        break;*/
    case 0X1000004:
    case Qt::Key_F11:
        QtVsPlayer::FullScr();
        break;
    case Qt::Key_F10:
        QtVsPlayer::HideCtrl();
        break;
    case 16777248:
    case Qt::Key_Space:
        VideoCtrls::pause();
        break;
    default:
        int Cod = event->key();
        QString result = QString::number( Cod, 16 );
        qDebug() << "currentKeyPressed 0X" + result.toUpper();
        break;
    }
    return;
}

void QtVsPlayer::FullScr()
{
    if (QtVsPlayer::isFullScreen()) {
        Fullscr = false;
        QtVsPlayer::showNormal();
        this->ui->menubar->setVisible(true);
        if (!Zoomed)
            this->ui->statusbar->setVisible(true);
    } else {
        QtVsPlayer::showFullScreen();
        Fullscr = true;
        this->ui->menubar->setVisible(false);
        this->ui->statusbar->setVisible(false);
    }
    return;
}

void QtVsPlayer::resizeEvent(QResizeEvent *event)
{
    if (!isVisible()) return;
    WinIdWorkarround();

    /*WVideoCtrls->move(WVideoCtrls->x(),this->height() -
                      WVideoCtrls->height() -
                      this->statusBar()->height());*/

    //this->ui->FsDisplay->setVisible(false);

    VideoView->resize(ui->scrollArea->size());
    //VideoView->resize(VideoView->width() - 2, VideoView->height());
    originH = VideoView->rect();
    Zoomed = false;

    //surpress warning!
    if (event == nullptr) {
        event = nullptr;
    }
    //surpress warning!

    QSettings settings;
    settings.beginGroup("QtVsPlayer");

    settings.setValue( "X", this->pos().x());
    settings.setValue("Y", this->pos().y());
    settings.setValue("Width", this->width());
    settings.setValue("Height", this->height());
    settings.setValue("State", GetWinState());

    settings.endGroup();
    settings.sync();
    //VideoView->resize(ui->graphicsView->size());
    return ;
}

QString QtVsPlayer::GetWinState()
{
    switch (this->windowState()) {

    default:
    case Qt::WindowNoState://0x00000000 The window has no state set (in normal state).
        return "0x00000000";
        break;
    case Qt::WindowMinimized://0x00000001 The window is minimized (i.e. iconified).
        return "0x00000001";
        break;
    case Qt::WindowMaximized://0x00000002 The window is maximized with a frame around it.
        return "0x00000002";
        break;
    case Qt::WindowFullScreen://0x00000004 The window fills the entire screen without any frame around it.
        return "0x00000004";
        break;
    case Qt::WindowActive://0x00000008 The window is the active window, i.e. it has keyboard focus.
        return "0x00000008";
        break;

    }
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

/*void QtVsPlayer::paintEvent(QPaintEvent* event)
{
    qDebug() << event->Paint;
}*/

void QtVsPlayer::wheelEvent(QWheelEvent *event)
{
    if (WVideoCtrls->underMouse()) {
        return;
    }

    QPoint Scroll = event->angleDelta();
    WinIdWorkarround();


    if (Scroll.y() > 0)
    {
        VideoView->resize(VideoView->width() + 50,
                          VideoView->height() + 50);
        Zoomed = true;

    }

    if (Scroll.y() < 0 and originH.height() < VideoView->height())
    {
        VideoView->resize(VideoView->width() - 50,
                          VideoView->height() - 50);
        Zoomed = true;
    }

    if (Scroll.y() <= 0)
    {
        Zoomed = false;
        VideoView->move(originH.x(), originH.y());
    }
    ScrollBarsView(Zoomed);
    WinIdWorkarround();
    return;
}

void QtVsPlayer::mouseMoveEvent(QMouseEvent *event)
{
    if (event and
            !this->ui->actionMasquer_les_controles->isChecked() and
            this->ui->actionAuto_hide_controls->isChecked()) {
        QRect WVideoCtrlsRect(WVideoCtrls->mapToGlobal(WVideoCtrls->rect().topLeft()),WVideoCtrls->size());
        if (WVideoCtrlsRect.contains(event->pos())) {

            if(!Zoomed and !Fullscr )
                ui->statusbar->setVisible(true);
            WVideoCtrls->setVisible(true);
            WVideoCtrls->activateWindow();
            WVideoCtrls->raise();
        }else
        {
            WVideoCtrls->setVisible(false);
            QtVsPlayer::cursor() = Qt::BlankCursor;
        }
    }

    if (QtVsPlayer::cursor() == Qt::BlankCursor) {
        QtVsPlayer::unsetCursor();
    }

    if (event->buttons() == Qt::LeftButton and Zoomed) {
        QPoint p = event->pos();
        QRect R = rect();
//        qDebug() << "max scroll" << ui->scrollArea->horizontalScrollBar()->maximum();
//        qDebug() << "scroll val" << ui->scrollArea->horizontalScrollBar()->value();
//        qDebug() << "mouse x" << p.x();
        int Xvalue = p.x() * 100 / R.width();
        int Yvalue = p.y() * 100 / R.height();
//        qDebug() << "mouse x%" << Xvalue;
//        qDebug() << "scroll value want" << ui->scrollArea->horizontalScrollBar()->maximum() / 100 * Xvalue;

        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->maximum() / 100 * Xvalue);
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum() / 100 * Yvalue);
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
        this->ui->statusbar->setVisible(false);

        if (!Zoomed and !QtVsPlayer::isFullScreen())
            resize(width(),height() + this->ui->statusbar->height());
    }

    if (!this->ui->actionMasquer_les_controles->isChecked() and
            QtVsPlayer::cursor() != Qt::BlankCursor and
            !WVideoCtrls->underMouse() and
            this->ui->actionAuto_hide_controls->isChecked()) {
        QtVsPlayer::setCursor(Qt::BlankCursor);
    }
    return;
}

void QtVsPlayer::on_actionMasquer_les_controles_triggered()
{

    HideCtrl();
    SaveMenuItemState(ui->menuAffichage);
    return;
}

void QtVsPlayer::HideCtrl()
{
    if (this->ui->actionMasquer_les_controles->isChecked()) {
        VideoCtrls::AutoHide = true;
        WVideoCtrls->hide();

    } else {
        WVideoCtrls->show();
        WVideoCtrls->activateWindow();
        WVideoCtrls->raise();
        this->centralWidget()->lower();
        this->centralWidget()->stackUnder(WVideoCtrls);
        if (!Zoomed and !QtVsPlayer::isFullScreen())
            this->ui->statusbar->setVisible(true);
        VideoCtrls::AutoHide = false;
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
    playm4interface::hwnd = 0;

    playm4interface::hwnd = VideoView->winId();

    playm4interface::RefreshPlay();
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
    RtspView->show();
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
    if (text != "Raiseit") {
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

    raise();
}

void QtVsPlayer::actionSlot(const QString &text)
{
    QString msg( QLatin1String("<%1> %2") );
    //msg = msg.arg(nickname, text);
    m_messages.append(text);

}


void QtVsPlayer::on_actionVCA_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_ANA_INTEL_DATA", PLAYM4_RENDER_ANA_INTEL_DATA, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionMotion_detection_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_MD", PLAYM4_RENDER_MD, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionPOS_Text_Overlay_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_ADD_POS", PLAYM4_RENDER_ADD_POS, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionPicture_Overlay_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_ADD_PIC", PLAYM4_RENDER_ADD_PIC, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionFire_Source_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_FIRE_DETCET", PLAYM4_RENDER_FIRE_DETCET, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionTemperature_toggled(bool arg1)
{
    VCASwitch("PLAYM4_RENDER_TEM", PLAYM4_RENDER_TEM, arg1);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::VCASwitch(QString Name, int Info, bool IsActive)
{
    if (!PlayM4_RenderPrivateData(playm4interface::m_pblocalportnum, Info, IsActive))
    {
        playm4interface::DisplayError(Name,PlayM4_GetLastError(playm4interface::m_pblocalportnum));
        SaveMenuItemState(ui->menuAffichage);
    }
}

void QtVsPlayer::on_actionAll_triggered()
{
    enumerateMenu(ui->menuVCA_Info_Overlay, true);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::on_actionNone_triggered()
{
    enumerateMenu(ui->menuVCA_Info_Overlay, false);
    SaveMenuItemState(ui->menuAffichage);
}

void QtVsPlayer::enumerateMenu(QMenu *menu, bool ChkState)
{
    foreach (QAction *action, menu->actions()) {
        if (action->isSeparator()) {
            //qDebug("this action is a separator");
        } else if (action->menu()) {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            //qDebug(">>> this action is associated with a submenu, iterating it recursively...");
            enumerateMenu(action->menu(), ChkState);
            //qDebug("<<< finished iterating the submenu");
        } else {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            action->setChecked(ChkState);
        }
    }
}

void QtVsPlayer::on_actionAuto_hide_controls_triggered(bool checked)
{
    VideoCtrls::AutoHide = checked;
    SaveMenuItemState(ui->menuAffichage);
    if (!checked and !ui->actionMasquer_les_controles->isChecked()) {
        WVideoCtrls->show();
        WVideoCtrls->activateWindow();
        WVideoCtrls->raise();
        this->centralWidget()->lower();
        this->centralWidget()->stackUnder(WVideoCtrls);
        if (!Zoomed and !QtVsPlayer::isFullScreen())
            this->ui->statusbar->setVisible(true);
    }
}

void QtVsPlayer::on_actionOnline_documentation_triggered()
{
    QString link = "https://github.com/surfzoid/QtVsPlayer/wiki/Doc";
    QDesktopServices::openUrl(QUrl(link));
}

void QtVsPlayer::SaveMenuItemState(QMenu *menu)
{
    QSettings settings;
    settings.beginGroup("MenuItemState");
    foreach (QAction *action, menu->actions()) {
        if (action->isSeparator()) {
            //qDebug("this action is a separator");
        } else if (action->menu()) {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            //qDebug(">>> this action is associated with a submenu, iterating it recursively...");
            SaveMenuItemState(action->menu());
            //qDebug("<<< finished iterating the submenu");
        } else {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            if (action->isCheckable()) {
                settings.setValue( action->objectName(), action->isChecked());
            }
        }
    }

    settings.endGroup();
    settings.sync();
}

void QtVsPlayer::GetMenuItemState(QMenu *menu)
{
    QSettings settings;
    settings.beginGroup("MenuItemState");
    foreach (QAction *action, menu->actions()) {
        if (action->isSeparator()) {
            //qDebug("this action is a separator");
        } else if (action->menu()) {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            //qDebug(">>> this action is associated with a submenu, iterating it recursively...");
            SaveMenuItemState(action->menu());
            //qDebug("<<< finished iterating the submenu");
        } else {
            //qDebug("action: %s", qUtf8Printable(action->text()));
            if (action->isCheckable()) {
                action->setChecked(settings.value(action->objectName(), action->isChecked()).value<bool>());
            }
        }
    }

    settings.endGroup();
    settings.sync();

    HideCtrl();
    WVideoCtrls->AutoHide = ui->actionAuto_hide_controls->isChecked();

}

void QtVsPlayer::ScrollBarsView(bool VState)
{
    if (VState) {
        ui->scrollArea->horizontalScrollBar()->show();
        ui->scrollArea->verticalScrollBar()->show();
        //ui->scrollArea->horizontalScrollBar()->resize(20, 2);
        //ui->scrollArea->verticalScrollBar()->resize(2, 2);
        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        if(!QtVsPlayer::isFullScreen())
            ui->statusbar->setVisible(false);
    } else {
        ui->scrollArea->horizontalScrollBar()->hide();
        ui->scrollArea->verticalScrollBar()->hide();
        //ui->scrollArea->horizontalScrollBar()->resize(0, 0);
        //ui->scrollArea->verticalScrollBar()->resize(0, 0);
        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        if(!Zoomed and !QtVsPlayer::isFullScreen())
            ui->statusbar->setVisible(true);
        VideoView->resize(ui->scrollArea->size());
    }
}

void QtVsPlayer::LoadCursor(bool Loading)
{
    if (Loading) {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    } else {
        QGuiApplication::restoreOverrideCursor();
    }
}

void QtVsPlayer::on_actionQuit_triggered()
{
    qApp->quit();
}

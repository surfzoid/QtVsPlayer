#include "videoctrls.h"
#include "ui_videoctrls.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "qtvsplayer.h"

bool VideoCtrls::EndRead = true;
bool VideoCtrls::PLast = false;
int VideoCtrls::HikNumPort = -1;
int VideoCtrls::seekSpeed = 0;
int VideoCtrls::Duration = 0;
//QString VideoCtrls::picturepathname = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/PlayBackPictureDir/";
static QString PicPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/";
static bool IsSnapShoot = false;
static int SnapShootCtl = 0;

VideoCtrls::VideoCtrls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCtrls)
{
    ui->setupUi(this);
    //setFocusPolicy( Qt::StrongFocus );
    //setFocus( Qt::PopupFocusReason );
    setEnabled( true );
    //Multimedia keys shortcut
    this->ui->previousButton->setShortcut(Qt::Key_MediaPrevious);
    this->ui->nextButton->setShortcut(Qt::Key_MediaNext);
    this->ui->playButton->setShortcut(Qt::Key_MediaPlay);
    this->ui->pauseButton->setShortcut(0x1000020);
    this->ui->stopButton->setShortcut(Qt::Key_MediaStop);
    this->ui->SeekLessButton->setShortcut(Qt::Key_AudioRewind);
    this->ui->SeekMoreButton->setShortcut(Qt::Key_BackForward);


    //set init value
    //m_pbqtimer = NULL ;
    m_pbqtimer = new QTimer(this);
    m_pbqtimer->setTimerType(Qt::PreciseTimer);
    //connect(m_pbqtimer, &QTimer::timeout, this, &VideoCtrls::updatelocalprocess);
    m_pbqtimer->start( 1000 );
    connect(m_pbqtimer, SIGNAL(timeout()), this, SLOT(updatelocalprocess()));

    this->ui->CurSpeed->setText("0X");
}

VideoCtrls::~VideoCtrls()
{
    if (m_pbqtimer != NULL)
    {
        delete m_pbqtimer;
        m_pbqtimer = NULL ;
    }

    //on_stopButton_released();

    delete ui;
}

void VideoCtrls::InitTimeSlide()
{
    this->ui->TimeSlider->setRange(0,100);
    this->ui->TimeSlider->setValue(0);
    //ui.horizontalSlider_playback_volume_2->setRange(0,10);
    return;

}

void VideoCtrls::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MiddleButton) {
        move(event->pos() + pos());
    }
    return;
}


bool PauseFlag= true;
void VideoCtrls::on_pauseButton_released()
{
    pause();
    return;
}

void VideoCtrls::pause()
{
    playm4interface::Pause(PauseFlag);
    EndRead = PauseFlag;
    PauseFlag = !PauseFlag;
    return;

}

void VideoCtrls::on_playButton_released()
{
    seekSpeed = 0;
    this->ui->CurSpeed->setText("0X");
    playm4interface::Play();
    EndRead =false;
    return;
}

void VideoCtrls::on_stopButton_released()
{
    seekSpeed = 0;
    this->ui->CurSpeed->setText("0X");
    this->ui->TimeSlider->setValue(0);
    playm4interface::Stop();
    return;

}

void VideoCtrls::on_SeekMoreButton_released()
{
    if (seekSpeed < 6) {
        playm4interface::Fast();
        seekSpeed += 1;
        this->ui->CurSpeed->setText(QString::number(seekSpeed) +  "X");
    }
    return;
}

void VideoCtrls::on_SeekLessButton_released()
{
    if (seekSpeed > -6 ) {
        playm4interface::Slow();
        seekSpeed -= 1;
        this->ui->CurSpeed->setText(QString::number(seekSpeed) +  "X");
    }
    return;

}

void VideoCtrls::RestoreSeek()
{
    int Speed = seekSpeed;
    seekSpeed = 0;

    if (Speed > 0) {
        for (int i = 0;i < Speed ;i++) {
            on_SeekMoreButton_released();
        }
    }

    if (Speed < 0) {
        for (int i = 0;i < Speed ;i--) {
            on_SeekMoreButton_released();
        }
    }
    return;
}

void VideoCtrls::on_OneByOneButton_released()
{
    seekSpeed = 0;
    playm4interface::OneByOne();
    return;
}

void VideoCtrls::on_OneByOneBackButton_released()
{
    seekSpeed = 0;
    this->ui->CurSpeed->setText("0X");
    playm4interface::OneByOneBack();
    EndRead =false;
    return;
}


/************************************************************************
 *	Function  		:	updatelocalprocess
 *  Description     :  	update play process
 *  Input           :
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void VideoCtrls::updatelocalprocess()
{
    if (EndRead == false) {
        if (HikNumPort >= 0)
        {
            try {

                unsigned int currentpos;
                float pos;
                pos = PlayM4_GetPlayPos(HikNumPort);
                currentpos =(unsigned int)(pos*100);
                ui->TimeSlider->setValue(currentpos);

                unsigned int PlayedTime = PlayM4_GetPlayedTime(HikNumPort);
                QString CurTime = QDateTime::fromSecsSinceEpoch((int)(PlayedTime)).toUTC().toString("mm:ss");
                QString TotTime = QDateTime::fromSecsSinceEpoch((int)(Duration)).toUTC().toString("mm:ss");
                ui->LblDuration->setText(CurTime +  "\n\r" + TotTime );

                //ui->LblDuration->setText(QString::number((Duration/60)*pos, 'f', 2) +  " / " + QString::number(Duration/60, 'f', 2) );
                //ui->TimeSlider->statusTip(currentpos).fromUtf8();
                if (currentpos >= 100) {

                    //PlayM4_CloseFile(playm4interface::m_pblocalportnum);
                    QThreadPool::globalInstance()->releaseThread();
                    QThreadPool::globalInstance()->clear();
                    printf("pyd---activeThreadCount:%d\n\r",QThreadPool::globalInstance()->activeThreadCount());

                    QtVsPlayer::PlayNextFile(false,0);

                    if (PLast == true) {
                        EndRead = true;
                        //on_stopButton_released();
                    }
                }
            }  catch (QException e ) {
                printf("pyd---Hik SDK Exception:%s\n\r",e.what());
            }


        }

    }
    return;
}

void VideoCtrls::on_nextButton_released()
{
    QtVsPlayer::PlayNextFile(false,0);
    return;

}



void VideoCtrls::on_previousButton_released()
{
    QtVsPlayer::LastPlayIdx -= 2;
    //int idx = QtVsPlayer::LastPlayIdx;
    QtVsPlayer::PlayNextFile(false, 0);
    return;
}

/************************************************************************
 *	Function  		:	PlayM4DisplayCallBack
 *  Description     :  	capture picture
 *  Input           :
 *						int nPort,
                        char *pBuf,
                        long size,
                        long width,
                        long height,
                        long stamp,
                        long type,
                        long reserved
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void CALLBACK PlayM4DisplayCallBack(int nPort, char *pBuf, int size, int width, int height,
                                    int stamp, int type, int reserved)
{
    if (IsSnapShoot == true) {
        QString picturepathname(CAPTURE_PICTURE_PATH);
        QString today=QDateTime::currentDateTime().toString("yyyy-MM-dd");
        QDir directory(PicPath);
        picturepathname = PicPath + picturepathname + today + "/";
        directory.mkpath(picturepathname);
        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd__hh_mm_ss");
        picturepathname.append(time.toUtf8().data());
        picturepathname.append(".bmp");

        //JPEG ERROR NEED FOLLOW!!!!
        PlayM4_ConvertToBmpFile(pBuf, size, width, height, type, picturepathname.toUtf8().data());
        //PlayM4_ConvertToJpegFile(pBuf, size, width, height, type, picturepathname.toUtf8().data());

        SnapShootCtl += 1;
    }

    if (SnapShootCtl == 45) {
        SnapShootCtl = 0;
        IsSnapShoot = false;
    }

    //surpress warning!
    if (nPort == 0 and stamp == 0 and reserved == 0 ) {
        //I'm happy for yu
    }
    //surpress warning!

    return;
}


/************************************************************************
 *	Function  		:	on_pushButton_playback_picture_2_clicked
 *  Description     :  	capture picture
 *  Input           :
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void VideoCtrls::on_SnapshotButton_released()
{
    PlayM4_SetPicQuality(HikNumPort,1);
    int *width = new int(1024);
    int *height = new int(768);

    PlayM4_GetPictureSize(HikNumPort,width,height);

    IsSnapShoot = true;

#if (defined(_WIN32))
    PlayM4_SetDisplayCallBack(HikNumPort,void (CALLBACK*)(int,char,long,long,long,long,long,void*)PlayM4DisplayCallBack);
#elif defined(__linux__)
    PlayM4_SetDisplayCallBack(HikNumPort, (void (CALLBACK *)(int,char *,int,int,int,int,int,int))PlayM4DisplayCallBack);
#endif

    QString picturepath(CAPTURE_PICTURE_PATH);
    QString today=QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir directory(PicPath);
    picturepath = PicPath + picturepath + today + "/";
    //test
    /*bool suc = false;
    QString test = picturepath + "test.jpg";
    QByteArray ba = test.toLocal8Bit();
    unsigned char *res = (unsigned char *)strdup(ba.constData());
    unsigned int  *buffSize = (unsigned int *)4608000;
    suc = PlayM4_GetBMP(HikNumPort,res, 4608000,buffSize);*/
    //end test

    QString picturepathname(tr("Capture Picture succes to "));
    picturepathname.append(picturepath);

    QMessageBox::information(this,tr("Capture Picture succes "),
                             picturepath.toUtf8().data());
    return;

}

void VideoCtrls::on_SoudVolume_valueChanged(int value)
{
    playm4interface::SetVolume((unsigned short)value);
}

void VideoCtrls::on_TimeSlider_actionTriggered(int action)
{
    if (QApplication::mouseButtons() == Qt::MiddleButton) {
        return;
    }

    pause();
    int value =ui->TimeSlider->value();
    PlayM4_SetPlayPos(HikNumPort, ((float)value)*0.01);

    return;
}

void VideoCtrls::on_TimeSlider_sliderReleased()
{

    pause();

    return;
}
void VideoCtrls::focusOutEvent(QFocusEvent *)
{
    hide();
}

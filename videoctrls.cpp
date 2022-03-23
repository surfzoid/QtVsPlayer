#include "videoctrls.h"
#include "ui_videoctrls.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "qtvsplayer.h"

bool VideoCtrls::EndRead = true;
bool VideoCtrls::PLast = false;
int VideoCtrls::HikNumPort = -1;
int VideoCtrls::seekSpeed = 0;

VideoCtrls::VideoCtrls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCtrls)
{
    ui->setupUi(this);

    //set init value
    //m_pbqtimer = NULL ;
    m_pbqtimer = new QTimer(this);
    m_pbqtimer->setTimerType(Qt::PreciseTimer);
    //connect(m_pbqtimer, &QTimer::timeout, this, &VideoCtrls::updatelocalprocess);
    m_pbqtimer->start( 1000 );
    connect(m_pbqtimer, SIGNAL(timeout()), this, SLOT(updatelocalprocess()));
    this->ui->lineEdit_2_pbprocess->setReadOnly(true);

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


bool PauseFlag= false;
void VideoCtrls::on_pauseButton_released()
{
    playm4interface::Pause(!PauseFlag);
    PauseFlag = !PauseFlag;
    EndRead = PauseFlag;
    return;
}

void VideoCtrls::on_TimeSlider_valueChanged(int value)
{
    /*updatelocalprocess();
    unsigned int currentpos;
    float pos;
    pos = PlayM4_GetPlayPos(HikNumPort);
    currentpos =(unsigned int)(pos*100);
    this->ui->TimeSlider->setValue(currentpos);*/
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
    if (seekSpeed < 6 and seekSpeed >= 0) {
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
    //while (true) {
    playm4interface::OneByOneBack();
    //}
    //m_pbqtimer->stop();
    //m_pbqtimer->connect(this,SIGNAL(this), this, SLOT(updatelocalprocess()));
    //m_pbqtimer->start(1000);
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
                //ui->TimeSlider->statusTip(currentpos).fromUtf8();
                if (currentpos >= 100) {

                    //PlayM4_CloseFile(playm4interface::m_pblocalportnum);
                    QThreadPool::globalInstance()->releaseThread();
                    QThreadPool::globalInstance()->clear();
                    printf("pyd---activeThreadCount:%d\n\r",QThreadPool::globalInstance()->activeThreadCount());

                    QtVsPlayer().PlayNextFile(false,0);

                    if (PLast == true) {
                        EndRead = true;
                        on_stopButton_released();
                    }
                }
            }  catch (QException e) {
                printf("pyd---Hik SDK Exception:%s\n\r",e.what());
            }


        }

    }
    return;
}

void VideoCtrls::on_lineEdit_2_pbprocess_textChanged(const QString &arg1)
{
    QtVsPlayer().DisplayStatus(arg1);
    return;
}

void VideoCtrls::on_TimeSlider_sliderMoved(int position)
{

    //int value =ui->TimeSlider->value();
    PlayM4_SetPlayPos(HikNumPort, ((float)position)*0.01);
    return;
}

void VideoCtrls::on_TimeSlider_sliderPressed()
{
    //EndRead = true;
    //on_pauseButton_released();
    return;

}

void VideoCtrls::on_TimeSlider_sliderReleased()
{
    //EndRead = false;
   // on_pauseButton_released();
    return;

}

void VideoCtrls::on_nextButton_released()
{
    QtVsPlayer().PlayNextFile(false,0);
    return;

}



void VideoCtrls::on_previousButton_released()
{
    QtVsPlayer::LastPlayIdx -= 2;
    QtVsPlayer().PlayNextFile(false,0);
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
void __stdcall PlayM4DisplayCallBack(long nPort, char *pBuf, long size, long width, long height,
                                     long stamp, long type, long reserved)
{
    QString picturepathname(CAPTURE_PICTURE_PATH);
    QDir directory("./");
    directory.mkpath(picturepathname);
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd__hh_mm_ss");
    picturepathname.append(time.toLatin1().data());
    picturepathname.append(".bmp");

    //width = (long)1024;
    //height  = (long)768;

    //JPEG ERROR NEED FOLLOW!!!!
    PlayM4_ConvertToBmpFile(pBuf, size, width, height, type, picturepathname.toUtf8().data());
    //PlayM4_ConvertToJpegFile(pBuf, size, width, height, type, picturepathname.toUtf8().data());
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
#if (defined(_WIN32))
    PlayM4_SetDisplayCallBack(HikNumPort, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    PlayM4_SetDisplayCallBack(HikNumPort, (void (__stdcall *)(int,char *,int,int,int,int,int,int))PlayM4DisplayCallBack);
#endif

    QString picturepathname(tr("Capture Picture succes to "));
    picturepathname.append(CAPTURE_PICTURE_PATH);

    QMessageBox::information(this,tr("Capture Picture succes "),
                             picturepathname.toUtf8().data());
    return;

}

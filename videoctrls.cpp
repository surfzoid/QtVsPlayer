#include "videoctrls.h"
#include "ui_videoctrls.h"
#include <QMouseEvent>
#include "qtvsplayer.h"

bool VideoCtrls::EndRead = true;
bool VideoCtrls::PLast = false;
int VideoCtrls::HikNumPort = -1;

VideoCtrls::VideoCtrls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCtrls)
{
    ui->setupUi(this);

    //set init value
    m_pbqtimer = NULL ;
    m_pbqtimer = new QTimer(this);
    m_pbqtimer->setTimerType(Qt::PreciseTimer);
    connect(m_pbqtimer, &QTimer::timeout, this, &VideoCtrls::updatelocalprocess);
    m_pbqtimer->start( 1000 );
}

VideoCtrls::~VideoCtrls()
{
    delete ui;
}

void VideoCtrls::InitTimeSlide()
{
    this->ui->TimeSlider->setRange(0,100);
    this->ui->TimeSlider->setValue(0);
    //ui.horizontalSlider_playback_volume_2->setRange(0,10);

}

void update()
{
    QtVsPlayer().nPlaym4Interface->OneByOneBack();
}

void VideoCtrls::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MiddleButton) {
        move(event->pos()+pos());
    }
}


bool PauseFlag= false;
void VideoCtrls::on_pauseButton_released()
{
    QtVsPlayer().nPlaym4Interface->Pause(!PauseFlag);
    PauseFlag = !PauseFlag;
    EndRead = PauseFlag;
}

void VideoCtrls::on_TimeSlider_valueChanged(int value)
{
    /*updatelocalprocess();
    unsigned int currentpos;
    float pos;
    pos = PlayM4_GetPlayPos(HikNumPort);
    currentpos =(unsigned int)(pos*100);
    this->ui->TimeSlider->setValue(currentpos);*/
}


void VideoCtrls::on_playButton_released()
{
    QtVsPlayer().nPlaym4Interface->Play();
    EndRead =false;
}

void VideoCtrls::on_stopButton_released()
{
    QtVsPlayer().nPlaym4Interface->Stop();
}

void VideoCtrls::on_SeekMoreButton_released()
{
    QtVsPlayer().nPlaym4Interface->Fast();
}

void VideoCtrls::on_SeekLessButton_released()
{
    QtVsPlayer().nPlaym4Interface->Slow();
}

void VideoCtrls::on_OneByOneButton_released()
{
    QtVsPlayer().nPlaym4Interface->OneByOne();
}

void VideoCtrls::on_OneByOneBackButton_released()
{
    //while (true) {
    QtVsPlayer().nPlaym4Interface->OneByOneBack();
    //}
    //m_pbqtimer->stop();
    //m_pbqtimer->connect(this,SIGNAL(this), this, SLOT(updatelocalprocess()));
    //m_pbqtimer->start(1000);
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
                unsigned int totaltime;
                //totaltime = PlayM4_GetPlayedTime(HikNumPort);
                //CurTime = totaltime;
                QString qtotaltime;
                QString timevalue;
                if (totaltime/3600 <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number(totaltime/3600);
                qtotaltime.append(timevalue);
                qtotaltime.append(":");
                if (((totaltime%3600)/60) <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number((totaltime%3600)/60);
                qtotaltime.append(timevalue);
                qtotaltime.append(":");
                if ((totaltime%60) <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number(totaltime%60);
                qtotaltime.append(timevalue);

                //unsigned int timeback = totaltime;
                qtotaltime.append("/");
                //totaltime =PlayM4_GetFileTime(HikNumPort);
                //EndTime = totaltime;
                if (totaltime/3600 <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number(totaltime/3600);
                qtotaltime.append(timevalue);
                qtotaltime.append(":");
                if (((totaltime%3600)/60) <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number((totaltime%3600)/60);
                qtotaltime.append(timevalue);
                qtotaltime.append(":");
                if ((totaltime%60) <= 9)
                {
                    qtotaltime.append("0");
                }
                timevalue = QString::number(totaltime%60);
                qtotaltime.append(timevalue);
                //qtotaltime.append("  ");
                this->ui->lineEdit_2_pbprocess->setText(qtotaltime);
                //QtVsPlayer().DisplayStatus(qtotaltime);

                unsigned int currentpos;
                float pos;
                pos = PlayM4_GetPlayPos(HikNumPort);
                currentpos =(unsigned int)(pos*100);
                ui->TimeSlider->setValue(currentpos);
                //ui->TimeSlider->statusTip(currentpos).fromUtf8();
                if (currentpos >= 100) {

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
    //return;
}

void VideoCtrls::on_lineEdit_2_pbprocess_textChanged(const QString &arg1)
{
    QtVsPlayer().DisplayStatus(arg1);
}

void VideoCtrls::on_TimeSlider_sliderMoved(int position)
{
    PlayM4_SetPlayPos(HikNumPort,(float)(position/100));
}

void VideoCtrls::on_TimeSlider_sliderPressed()
{
    //EndRead = true;
    on_pauseButton_released();

}

void VideoCtrls::on_TimeSlider_sliderReleased()
{
    //EndRead = false;
    on_pauseButton_released();

}

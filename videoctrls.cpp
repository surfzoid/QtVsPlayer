#include "videoctrls.h"
#include "ui_videoctrls.h"
#include <QMouseEvent>
#include "playm4interface.h"
#include "qtvsplayer.h"


VideoCtrls::VideoCtrls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCtrls)
{
    ui->setupUi(this);
}

VideoCtrls::~VideoCtrls()
{
    delete ui;
}


void VideoCtrls::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
            move(event->pos()+pos());
        }
}


bool PauseFlag= false;
void VideoCtrls::on_pauseButton_released()
{
    if (PauseFlag) {
        QtVsPlayer().nPlaym4Interface->Pause(0);
    } else {
        QtVsPlayer().nPlaym4Interface->Pause(1);
    }
    PauseFlag = !PauseFlag;
}

void VideoCtrls::on_TimeSlider_valueChanged(int value)
{

}


void VideoCtrls::on_playButton_released()
{
    QtVsPlayer().nPlaym4Interface->Play();
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

}

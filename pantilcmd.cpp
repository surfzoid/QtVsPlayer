#include "pantilcmd.h"
#include "ui_pantilcmd.h"
#include <QDebug>
#include<QMouseEvent>
#include "rtspwindow.h"

PanTilCmd::PanTilCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanTilCmd)
{
    ui->setupUi(this);

    resize(120,120);
    ui->PanTilLayout->setGeometry(rect());
    ui->gridLayoutWidget->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding);
    ui->gridLayoutWidget->setGeometry(rect());
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint);

    ui->BtnTopLeft->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnUp->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnTopRight->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnRight->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtBottomRight->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnDown->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnBottomLeft->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnLeft->setStyleSheet("QPushButton{background: transparent;}");
    ui->BtnMove->setStyleSheet("QPushButton{background: transparent;}");

    //there is a ghost widget!!!!!!
    QList<QWidget *> widgets = PanTilCmd::findChildren<QWidget *>();
    foreach (QWidget *var, widgets) {
        var->setMouseTracking(true);
    }

    QSettings settings;
    QPoint NewPos;

    settings.beginGroup("PanTilCmd");
    NewPos.setX(settings.value("X", 0).value<int>());
    if(NewPos.x()<= 0)NewPos.setX(0);
    NewPos.setY(settings.value("Y", height()).value<int>());
    if(NewPos.y()<= 0)NewPos.setY(height());

    settings.endGroup();
    move(NewPos);
}

PanTilCmd::~PanTilCmd()
{
    delete ui;
}

void PanTilCmd::on_BtnTopLeft_pressed()
{
    RtspWindow::PanTilTopLeft_pressed();
}

void PanTilCmd::on_BtnUp_pressed()
{
    RtspWindow::PanTilUp_pressed();
}

void PanTilCmd::on_BtnTopRight_pressed()
{
    RtspWindow::PanTilTopRight_pressed();
}

void PanTilCmd::on_BtnRight_pressed()
{
    RtspWindow::PanTilRight_pressed();
}

void PanTilCmd::on_BtBottomRight_pressed()
{
    RtspWindow::PanTilBottomRight_pressed();
}

void PanTilCmd::on_BtnDown_pressed()
{
    RtspWindow::PanTilDown_pressed();
}

void PanTilCmd::on_BtnBottomLeft_pressed()
{
    RtspWindow::PanTilBottomLeft_pressed();
}

void PanTilCmd::on_BtnLeft_pressed()
{
    RtspWindow::PanTilLeft_pressed();
}

void PanTilCmd::on_BtnMove_pressed()
{
    MoveWind = !MoveWind;
    if (!MoveWind) {

        QSettings settings;
        settings.beginGroup("PanTilCmd");

        settings.setValue( "X", x());
        settings.setValue("Y", y());
        settings.setValue("Width", width());
        settings.setValue("Height", height());

        settings.endGroup();
        settings.sync();
    }
}

void PanTilCmd::mouseMoveEvent(QMouseEvent *event)
{
    setFocus();
    raise();
    if (MoveWind && event) {

        QPoint p = QCursor::pos();
        //if (ui->BtnMove->underMouse())
            this->move(p.x()-60,p.y()-60);
        repaint();
    }
}

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
    /*setStyleSheet("background-color: rgba(0,0,0,0)");
//setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setPalette(Qt::transparent);
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute( Qt::WA_OpaquePaintEvent, true );
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowOpacity(0.1);*/
    ui->BtnTopLeft->setWindowOpacity(0.1);
    ui->BtnTopLeft->setAttribute(Qt::WA_TranslucentBackground, true);
    resize(ui->PanTilLayout->sizeHint());
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

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
    if (MoveWind && event) {

        QPoint p = parentWidget()->mapFromGlobal(QCursor::pos());

        this->move(p.x()-40,p.y()-40);
        repaint();
    }
}

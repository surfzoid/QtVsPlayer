#include "multimedia_shortcuts.h"
#include "ui_multimedia_shortcuts.h"
#include <QDebug>

Multimedia_shortcuts::Multimedia_shortcuts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Multimedia_shortcuts)
{
    installEventFilter(this);
    ui->setupUi(this);
    IsShown = false;
}

Multimedia_shortcuts::~Multimedia_shortcuts()
{
    delete ui;
}

void Multimedia_shortcuts::showEvent(QShowEvent *event)
{
    //Multimedia keys shortcut
    settings.beginGroup("Multimedia_shortcuts");
    this->ui->LEplay->setText(settings.value("play", "0X1000080").value<QString>());
    this->ui->LEpause->setText(settings.value("pause", "0X1000085").value<QString>());
    this->ui->LEstop->setText(settings.value("stop", "0X1000081").value<QString>());
    this->ui->LEprevious->setText(settings.value("previous", "0X1000082").value<QString>());
    this->ui->LEnext->setText(settings.value("next", "0X1000083").value<QString>());
    this->ui->LESeekLess->setText(settings.value("SeekLess", "0x01000062").value<QString>());
    this->ui->LESeekMore->setText(settings.value("SeekMore", "0x01000061").value<QString>());
    this->ui->LEForward->setText(settings.value("Forward", "0x01000063").value<QString>());
    this->ui->LEBackward->setText(settings.value("Backward", "0x01000064").value<QString>());
    settings.endGroup();

    IsShown = true;

    if(event)
        return;
}

void Multimedia_shortcuts::on_buttonBox_accepted()
{

    settings.beginGroup("Multimedia_shortcuts");

    settings.setValue( "play", ui->LEplay->text());
    settings.setValue("pause", ui->LEpause->text());
    settings.setValue("stop", ui->LEstop->text());
    settings.setValue("previous", ui->LEprevious->text());
    settings.setValue("next", ui->LEnext->text());
    settings.setValue("SeekLess", ui->LESeekLess->text());
    settings.setValue("SeekMore", ui->LESeekMore->text());
    settings.setValue("Forward", ui->LEForward->text());
    settings.setValue("Backward", ui->LEBackward->text());

    settings.endGroup();
    settings.sync();
    close();

}

bool Multimedia_shortcuts::eventFilter(QObject *obj, QEvent *event)
{
//    qDebug() << event->type();
    if (event->type() == QEvent::KeyRelease || event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        qDebug() << "pressed"<< key->key();
        qDebug() << "pressed"<< key->text();
        qDebug() << "pressed"<< key->nativeScanCode();
            QString TheKey = QString::number(key->key());
            if (this->ui->LEplay->hasFocus())
                this->ui->LEplay->setText(TheKey);
            if (this->ui->LEpause->hasFocus())
                this->ui->LEpause->setText(TheKey);
            if (this->ui->LEstop->hasFocus())
                this->ui->LEstop->setText(TheKey);
            if (this->ui->LEprevious->hasFocus())
                this->ui->LEprevious->setText(TheKey);
            if (this->ui->LEnext->hasFocus())
                this->ui->LEnext->setText(TheKey);
            if (this->ui->LESeekLess->hasFocus())
                this->ui->LESeekLess->setText(TheKey);
            if (this->ui->LESeekMore->hasFocus())
                this->ui->LESeekMore->setText(TheKey);
            if (this->ui->LEForward->hasFocus())
                this->ui->LEForward->setText(TheKey);
            if (this->ui->LEBackward->hasFocus())
                this->ui->LEBackward->setText(TheKey);
    }
    return QObject::eventFilter(obj, event);
}

#include "filesliste.h"
#include "ui_filesliste.h"
#include "playm4interface.h"
#include <QMessageBox>
#include <QDir>
#include <unistd.h>
#include <qtvsplayer.h>


FilesListe::FilesListe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilesListe)
{
    ui->setupUi(this);

    //table item been ticked emit signals and slots
    /*connect(ui->tableWidget_2_localfilist, SIGNAL(cellClicked(int, int)), this,
            SLOT(localtablebeenclicked(int , int)));*/

    //set begin and end time value
    QDate startdate = QDateTime::currentDateTime().date();
    QDate stopdate = QDateTime::currentDateTime().date();
    QTime starttime(0, 0, 0);
    QTime stoptime(23, 59, 59);
   /* FilesListe::starttime() = starttime;
    FilesListe::stoptime() = stoptime;*/
    ui->dateTimeEdit_2_start->setDate(startdate);
    ui->dateTimeEdit_2_start->setTime(starttime);
    ui->dateTimeEdit_2_stop->setDate(stopdate);
    ui->dateTimeEdit_2_stop->setTime(stoptime);
}

FilesListe::~FilesListe()
{
    delete ui;
}

void FilesListe::clearitems()
{
    //clear the table items of file list
    for (int i=0; i<ui->tableWidget_2_localfilist->rowCount(); i++)
    {
        for (int j=0; j<ui->tableWidget_2_localfilist->columnCount(); j++ )
        {
            delete ui->tableWidget_2_localfilist->takeItem(i, j);
        }
        ui->tableWidget_2_localfilist->removeRow(i);
    }
    ui->tableWidget_2_localfilist->setRowCount(0);
}


/*void FilesListe::localtablebeenclicked(int row,int column)
{
    if (0 != column)
    {
        return;
    }
    //playm4interface::m_pblocalfilepath = ui->tableWidget_2_localfilist->item(row,column)->text();
    return;
}*/

void FilesListe::Populate(QStringList fileNames)
{
    clearitems();
    foreach (QString fileName, fileNames) {

        QFile file(fileName);
        //��ʾ��table����  �ļ���
        QString filepath;
        filepath =file.fileName();
        QTableWidgetItem *FilePathItem = new QTableWidgetItem(filepath);
        FilePathItem->setFlags(Qt::ItemIsEnabled);
        //�ļ���С
        QString filesize;
        QFileInfo  filecurr(file.fileName());
        filesize = QString::number(filecurr.size());
        QTableWidgetItem *FileSizeItem = new QTableWidgetItem(filesize);
        FileSizeItem->setFlags(Qt::ItemIsEnabled);
        //����
        QString fileattr("Download");
        QTableWidgetItem *FileAttrItem = new QTableWidgetItem(fileattr);
        FileAttrItem->setFlags(Qt::ItemIsEnabled);

        //��ʼʱ��
       /* QTableWidgetItem *FileStartTimeItem = new QTableWidgetItem(starttime());
        FileStartTimeItem->setFlags(Qt::ItemIsEnabled);

        //����ʱ��
        QTableWidgetItem *FileStopTimeItem = new QTableWidgetItem(FilesListe::stoptime);
        FileStopTimeItem->setFlags(Qt::ItemIsEnabled);*/

        int row = ui->tableWidget_2_localfilist->rowCount();
        ui->tableWidget_2_localfilist->insertRow(row);
        ui->tableWidget_2_localfilist->setItem(row, 0, FilePathItem);
        ui->tableWidget_2_localfilist->setItem(row, 1, FileSizeItem);
        ui->tableWidget_2_localfilist->setItem(row, 2, FileAttrItem);
        /*ui->tableWidget_2_localfilist->setItem(row, 3, FileStartTimeItem);
        ui->tableWidget_2_localfilist->setItem(row, 4, FileStopTimeItem);*/


    }
    return;

}

void FilesListe::on_tableWidget_2_localfilist_cellClicked(int row, int column)
{
    if (0 != column)
    {
        return;
    }
    QtVsPlayer().PlayNextFile(true,row);
}

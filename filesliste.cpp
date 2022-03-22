#include "filesliste.h"
#include "ui_filesliste.h"
#include "playm4interface.h"
#include <QMessageBox>
#include <QDir>
//#include <unistd.h>
#include <qtvsplayer.h>


FilesListe::FilesListe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilesListe)
{
    ui->setupUi(this);

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
    for (int i=0; i< ui->tableWidget_2_localfilist->rowCount(); i++)
    {
        for (int j=0; j< ui->tableWidget_2_localfilist->columnCount(); j++ )
        {
            delete ui->tableWidget_2_localfilist->takeItem(i, j);
        }
        ui->tableWidget_2_localfilist->removeRow(i);
    }
    ui->tableWidget_2_localfilist->setRowCount(0);
    return;
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
    ui->tableWidget_2_localfilist->horizontalHeader()->sortIndicatorOrder();
    foreach (QString fileName, fileNames) {

        QStringList Colom = fileName.split("-");

        if (Colom.length() < 4) {
            QTableWidgetItem *FilePathItem = new QTableWidgetItem(fileName);
            FilePathItem->setFlags(Qt::ItemIsEnabled);
            break;
        }

        QFile file(fileName);

        QString filepath;
        filepath = Colom[4];
        QTableWidgetItem *FilePathItem = new QTableWidgetItem(filepath);
        FilePathItem->setFlags(Qt::ItemIsEnabled);

        QString filesize;
        QFileInfo  *filecurr = new QFileInfo(file.fileName());
        filesize = QString::number(filecurr->size() / 1000000);

        QTableWidgetItem *FileSizeItem = new QTableWidgetItem(filesize);
        FileSizeItem->setFlags(Qt::ItemIsEnabled);

        QString path(Colom[0]);
        path = path.left(path.length() - 8);
        QTableWidgetItem *pathItem = new QTableWidgetItem(path);
        //pathItem->setFlags(Qt::ItemIsEnabled);


        QString tt = Colom[1];
        QString starttime = tt.left(2) + ":" + tt.mid(2,2) + ":" + tt.right(2);
        QTableWidgetItem *FileStartTimeItem = new QTableWidgetItem(starttime);
        //FileStartTimeItem->setFlags(Qt::ItemIsEnabled);


        QString st = Colom[3];
        QString stoptime = st.left(2) + ":" + st.mid(2,2) + ":" + st.right(2);
         QTableWidgetItem *FileStopTimeItem = new QTableWidgetItem(stoptime);
        //FileStopTimeItem->setFlags(Qt::ItemIsEnabled);


        QString dd = Colom[2];
        QString Day = dd.left(4) + "/" + dd.mid(4,2) + "/" + dd.right(2);

        QTableWidgetItem *DayItem = new QTableWidgetItem(Day);

        FileStopTimeItem->setFlags(Qt::ItemIsEnabled);
        int row = ui->tableWidget_2_localfilist->rowCount();
        ui->tableWidget_2_localfilist->insertRow(row);
        ui->tableWidget_2_localfilist->setItem(row, 0, FilePathItem);
        ui->tableWidget_2_localfilist->setItem(row, 1, FileStartTimeItem);
        ui->tableWidget_2_localfilist->setItem(row, 2, FileStopTimeItem);
        ui->tableWidget_2_localfilist->setItem(row, 3, DayItem);
        ui->tableWidget_2_localfilist->setItem(row, 4, FileSizeItem);
        ui->tableWidget_2_localfilist->setItem(row, 5, pathItem);


    }
    QHeaderView *header = ui->tableWidget_2_localfilist->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    return;

}

void FilesListe::on_tableWidget_2_localfilist_cellClicked(int row, int column)
{
    if (0 != column)
    {
        return;
    }
    QtVsPlayer().PlayNextFile(true,row);
    return;
}

void FilesListe::on_FilesListe_destroyed()
{

    return;
}

#include "filesliste.h"
#include "ui_filesliste.h"
#include "playm4interface.h"
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

static QString MemDay("");// = QDate::currentDate().toString("yyyy-mm-dd");
static bool NextDay = false;
void FilesListe::Populate(QStringList fileNames, bool ClearBefore)
{
    if (ClearBefore) clearitems();
    fileNames.removeDuplicates();

    ui->tableWidget_2_localfilist->horizontalHeader()->sortIndicatorOrder();
    foreach (QString fileName, fileNames) {

        QStringList Colom = fileName.split("-");

        if (Colom.length() < 5) {
            QFile file(fileName);
            QString filesize;
            QFileInfo  filecurr(file.fileName());

            QTableWidgetItem *FilePathItem = new QTableWidgetItem(filecurr.fileName());
            FilePathItem->setFlags(Qt::ItemIsEnabled);

            filesize = QString::number(filecurr.size() / 1000000);
            QTableWidgetItem *FileSizeItem = new QTableWidgetItem(filesize);

            FileSizeItem->setFlags(Qt::ItemIsEnabled);
            FileSizeItem->setTextAlignment(Qt::AlignHCenter);

            QString path(filecurr.path());
            path = path.left(path.length() - 8);
            QTableWidgetItem *pathItem = new QTableWidgetItem(path);

            QString Day = filecurr.lastModified().date().toString("dd.MM.yyyy");

            QTableWidgetItem *DayItem = new QTableWidgetItem(Day);
            DayItem->setTextAlignment(Qt::AlignHCenter);

            QString stoptime = filecurr.lastModified().time().toString("hh:mm:ss");
            QTableWidgetItem *FileStopTimeItem = new QTableWidgetItem(stoptime);
            //FileStopTimeItem->setFlags(Qt::ItemIsEnabled);
            //FileStopTimeItem->setBackground(Qt::darkRed);
            FileStopTimeItem->setForeground(Qt::darkBlue);
            FileStopTimeItem->setTextAlignment(Qt::AlignHCenter);

            DayItem->setBackground(Qt::darkRed);
            DayItem->setForeground(Qt::white);
            FilePathItem->setBackground(Qt::cyan);

            int row = ui->tableWidget_2_localfilist->rowCount();
            ui->tableWidget_2_localfilist->insertRow(row);
            ui->tableWidget_2_localfilist->setItem(row, 2, FileStopTimeItem);
            ui->tableWidget_2_localfilist->setItem(row, 0, FilePathItem);
            ui->tableWidget_2_localfilist->setItem(row, 4, FileSizeItem);
            ui->tableWidget_2_localfilist->setItem(row, 5, pathItem);
            ui->tableWidget_2_localfilist->setItem(row, 3, DayItem);
        }
        else {


            QFile file(fileName);

            QString filepath;
            filepath = Colom[4];
            QTableWidgetItem *FilePathItem = new QTableWidgetItem(filepath);
            FilePathItem->setFlags(Qt::ItemIsEnabled);

            QString filesize;
            QFileInfo  filecurr(file.fileName());
            filesize = QString::number(filecurr.size() / 1000000);

            QTableWidgetItem *FileSizeItem = new QTableWidgetItem(filesize);
            FileSizeItem->setFlags(Qt::ItemIsEnabled);
            FileSizeItem->setTextAlignment(Qt::AlignHCenter);

            QString path(Colom[0]);
            path = path.left(path.length() - 8);
            QTableWidgetItem *pathItem = new QTableWidgetItem(path);
            //pathItem->setFlags(Qt::ItemIsEnabled);


            QString tt = Colom[1];
            QString starttime = tt.left(2) + ":" + tt.mid(2,2) + ":" + tt.right(2);
            QTableWidgetItem *FileStartTimeItem = new QTableWidgetItem(starttime);
            //FileStartTimeItem->setFlags(Qt::ItemIsEnabled);
            FileStartTimeItem->setTextAlignment(Qt::AlignHCenter);


            QString st = Colom[3];
            QString stoptime = st.left(2) + ":" + st.mid(2,2) + ":" + st.right(2);
            QTableWidgetItem *FileStopTimeItem = new QTableWidgetItem(stoptime);
            //FileStopTimeItem->setFlags(Qt::ItemIsEnabled);
            //FileStopTimeItem->setBackground(Qt::darkRed);
            FileStopTimeItem->setForeground(Qt::darkBlue);
            FileStopTimeItem->setTextAlignment(Qt::AlignHCenter);


            QString dd = Colom[2];
            QString Day = dd.left(4) + "/" + dd.mid(4,2) + "/" + dd.right(2);

            QTableWidgetItem *DayItem = new QTableWidgetItem(Day);
            DayItem->setTextAlignment(Qt::AlignHCenter);

            if (Day != MemDay) {
                NextDay = !NextDay;
                MemDay = Day;
            }

            if (NextDay == true) {
                DayItem->setBackground(Qt::darkRed);
                DayItem->setForeground(Qt::white);
                FilePathItem->setBackground(Qt::cyan);
                FileStartTimeItem->setBackground(Qt::red);
                FileStartTimeItem->setForeground(Qt::white);
            }

            if (NextDay == false) {
                DayItem->setBackground(Qt::white);
                DayItem->setForeground(Qt::darkRed);
                FilePathItem->setBackground(Qt::white);
                FileStartTimeItem->setBackground(Qt::white);
                FileStartTimeItem->setForeground(Qt::red);
            }

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

    }
    QHeaderView *header = ui->tableWidget_2_localfilist->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    if (!ClearBefore) QtVsPlayer::PlayNextFile(true,ui->tableWidget_2_localfilist->currentRow());;

    if (!FromDTPicker) {
        QString DayStr = ui->tableWidget_2_localfilist->item(0,3)->text();
        QString TimeStr = ui->tableWidget_2_localfilist->item(0,1)->text();
        QDateTime DateFilter = QDateTime::fromString(DayStr + TimeStr, "yyyy/MM/ddhh:mm:ss");
        ui->dateTimeEdit_2_start->setDateTime(DateFilter);

        int RwCnt = ui->tableWidget_2_localfilist->rowCount() -1;
        DayStr = ui->tableWidget_2_localfilist->item(RwCnt,3)->text();
        TimeStr = ui->tableWidget_2_localfilist->item(RwCnt,1)->text();
        DateFilter = QDateTime::fromString(DayStr + TimeStr, "yyyy/MM/ddhh:mm:ss");
        ui->dateTimeEdit_2_stop->setDateTime(DateFilter);
    }

    FromDTPicker = false;
    return;

}

void FilesListe::on_tableWidget_2_localfilist_cellClicked(int row, int column)
{
    if (0 != column)
    {
        return;
    }
    QtVsPlayer::PlayNextFile(true,row);
    return;
}

void FilesListe::on_FilesListe_destroyed()
{

    return;
}

void FilesListe::SelCurPlay(unsigned int FsNumber)
{
    ui->tableWidget_2_localfilist->selectRow(FsNumber);
    return;
}


void FilesListe::on_tableWidget_2_localfilist_itemSelectionChanged()
{
    int CurRow = ui->tableWidget_2_localfilist->currentRow();
    QTableWidgetItem *item=new QTableWidgetItem(windowIcon(),QString::number(CurRow + 1),0);//set Icon a and string 1

    ui->tableWidget_2_localfilist->setVerticalHeaderItem(CurRow,item);
}

void FilesListe::on_dateTimeEdit_2_start_dateTimeChanged(const QDateTime &dateTime)
{
    FilterList(dateTime);
}

void FilesListe::on_dateTimeEdit_2_stop_dateTimeChanged(const QDateTime &dateTime)
{
    FilterList(dateTime);
}

void FilesListe::FilterList(QDateTime dateTime)
{
    FromDTPicker = true;
    if (dateTime.isValid() and ui->centralwidget->isVisible()) {
        ShowAll();
        QDateTime DateTimeStart = ui->dateTimeEdit_2_start->dateTime();

        QDateTime DateTimeEnd = ui->dateTimeEdit_2_stop->dateTime();

        //remove row change index, need to use a foreach, or make a new list to add after a clean, or change remove by hide.
        for (int i=0; i< ui->tableWidget_2_localfilist->rowCount(); i++) {

            QString DayCurentStr = ui->tableWidget_2_localfilist->item(i,3)->text();
            QString TimeCurentStr = ui->tableWidget_2_localfilist->item(i,1)->text();
            QDateTime DateTimeCurent = QDateTime::fromString(DayCurentStr + TimeCurentStr, "yyyy/MM/ddhh:mm:ss");
            if (DateTimeCurent<DateTimeStart or DateTimeCurent>DateTimeEnd) {
                for (int j=0; j< ui->tableWidget_2_localfilist->columnCount(); j++ )
                {
                    delete ui->tableWidget_2_localfilist->takeItem(i, j);
                }
                ui->tableWidget_2_localfilist->hideRow(i);
                //ui->tableWidget_2_localfilist->setRowCount(0);
            }
        }
    }
}

void FilesListe::ShowAll()
{
    Populate(QtVsPlayer::fileNames,true);
}

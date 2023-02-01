#ifndef FILESLISTE_H
#define FILESLISTE_H

#include <QMainWindow>
#include <QFileInfo>

namespace Ui {
class FilesListe;
}

class FilesListe : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilesListe(QWidget *parent = nullptr);
    ~FilesListe();

    void clearitems();
    void Populate(QStringList fileName, bool ClearBefore);
    void SelCurPlay(unsigned int FsNumber);

private:
    Ui::FilesListe *ui;
    void FilterList(QDateTime dateTime);
    void ShowAll();
    bool FromDTPicker = false;

private slots:

    void on_tableWidget_2_localfilist_cellClicked(int row, int column);
    void on_FilesListe_destroyed();
    void on_tableWidget_2_localfilist_itemSelectionChanged();
    void on_dateTimeEdit_2_start_dateTimeChanged(const QDateTime &dateTime);
    void on_dateTimeEdit_2_stop_dateTimeChanged(const QDateTime &dateTime);
};

#endif // FILESLISTE_H

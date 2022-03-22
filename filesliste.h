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
    void Populate(QStringList fileName);

    static QTime starttime();
    static QTime stoptime();
private:
    Ui::FilesListe *ui;

private slots:

    void on_tableWidget_2_localfilist_cellClicked(int row, int column);
    void on_FilesListe_destroyed();
};

#endif // FILESLISTE_H

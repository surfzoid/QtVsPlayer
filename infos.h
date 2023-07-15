#ifndef INFOS_H
#define INFOS_H

#include <QDialog>

namespace Ui {
class Infos;
}

class Infos : public QDialog
{
    Q_OBJECT

public:
    explicit Infos(QWidget *parent = nullptr);
    ~Infos();
    static QString InfoData;

private slots:

public slots:

private:
    Ui::Infos *ui;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // INFOS_H

#ifndef CFGDIALOG_H
#define CFGDIALOG_H

#include <QDialog>

namespace Ui {
class CfgDialog;
}

class CfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CfgDialog(QWidget *parent = nullptr);
    ~CfgDialog();
    QString ChannelName;
    bool ShowChannel=false;
    bool SaveParam=false;
    int ChannelX = 0;
    int ChannelY = 0;
    int wOSDTopLeftX = 0;
    int wOSDTopLeftY = 0;


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CfgDialog *ui;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // CFGDIALOG_H

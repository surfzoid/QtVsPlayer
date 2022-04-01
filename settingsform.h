#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();

    QString CamName = "CamName";
    QString CamIp = "192.168.";
    QString CamPort = "554";
    QString CamPortHttp = "800";
    QString CamUser = "admin";
    QString CamPass = "hik12345";

private slots:
    void on_BtnSave_released();

    void on_CamNameEd_currentIndexChanged(const QString &arg1);

    void on_BtnDel_released();

    void on_BtPlay_released();

    void on_BtnCancel_released();

private:
    Ui::SettingsForm *ui;
};

#endif // SETTINGSFORM_H

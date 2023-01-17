#ifndef PANTILCMD_H
#define PANTILCMD_H

#include <QWidget>

namespace Ui {
class PanTilCmd;
}

class PanTilCmd : public QWidget
{
    Q_OBJECT

public:
    explicit PanTilCmd(QWidget *parent = nullptr);
    ~PanTilCmd();

private:
    Ui::PanTilCmd *ui;
    bool MoveWind = false;

private slots:
    void on_BtnTopLeft_pressed();
    void on_BtnUp_pressed();
    void on_BtnTopRight_pressed();
    void on_BtnRight_pressed();
    void on_BtBottomRight_pressed();
    void on_BtnDown_pressed();
    void on_BtnBottomLeft_pressed();
    void on_BtnLeft_pressed();
    void on_BtnMove_pressed();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // PANTILCMD_H

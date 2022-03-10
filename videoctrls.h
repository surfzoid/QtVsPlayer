#ifndef VIDEOCTRLS_H
#define VIDEOCTRLS_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>

namespace Ui {
class VideoCtrls;
}

class VideoCtrls : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCtrls(QWidget *parent = nullptr);
    ~VideoCtrls();

    void InitTimeSlide();
private:
    Ui::VideoCtrls *ui;
    //timer for play-process
    QTimer *m_pbqtimer;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void on_pauseButton_released();
    void on_TimeSlider_valueChanged(int value);
    void on_playButton_released();
    void on_stopButton_released();
    void on_SeekMoreButton_released();
    void on_SeekLessButton_released();
    void on_OneByOneButton_released();
    void on_OneByOneBackButton_released();
    void updatelocalprocess();
//public slots:


};

#endif // VIDEOCTRLS_H

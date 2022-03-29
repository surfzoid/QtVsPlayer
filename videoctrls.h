#ifndef VIDEOCTRLS_H
#define VIDEOCTRLS_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>

#define CAPTURE_PICTURE_PATH "/QtVsPlayerPlayBackPicture/"

namespace Ui {
class VideoCtrls;
}

class VideoCtrls : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCtrls(QWidget *parent = nullptr);
    ~VideoCtrls();

    static int HikNumPort;
    void InitTimeSlide();
    static bool EndRead;
    static bool PLast;
    static int seekSpeed;
    void RestoreSeek();

private:
    Ui::VideoCtrls *ui;
    //timer for play-process
    QTimer *m_pbqtimer;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void on_pauseButton_released();
    void on_playButton_released();
    void on_stopButton_released();
    void on_SeekMoreButton_released();
    void on_SeekLessButton_released();
    void on_OneByOneButton_released();
    void on_OneByOneBackButton_released();
    void updatelocalprocess();

    //action slots:
    void on_TimeSlider_sliderMoved(int position);
    void on_TimeSlider_sliderPressed();
    void on_TimeSlider_sliderReleased();
    void on_nextButton_released();
    void on_previousButton_released();
    void on_SnapshotButton_released();
};

#endif // VIDEOCTRLS_H

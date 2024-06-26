#ifndef VIDEOCTRLS_H
#define VIDEOCTRLS_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QSettings>

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
    static void pause();
    static int Duration;
    static bool AutoHide;
    void Forward();
    void Backward();

private:
    Ui::VideoCtrls *ui;
    //timer for play-process
    QTimer *m_pbqtimer;
    QSettings settings;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *) override;
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

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
    void on_nextButton_released();
    void on_previousButton_released();
    void on_SnapshotButton_released();
    void on_SoudVolume_valueChanged(int value);
    void on_TimeSlider_actionTriggered(int action);
    void on_TimeSlider_sliderReleased();
    void on_Audio_released();
};

#endif // VIDEOCTRLS_H

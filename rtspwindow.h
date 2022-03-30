#ifndef RTSPWINDOW_H
#define RTSPWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class RtspWindow;
}

class RtspWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RtspWindow(QWidget *parent = nullptr);
    ~RtspWindow();

private slots:
    void on_ComboBxCam_currentIndexChanged(const QString &arg1);
    void positionChanged(qint64 pos);


    void on_actionIgnoreAspectRatio_triggered();

    void on_actionDefault_triggered();

    void on_actionKeepAspectRatio_triggered();

    void on_actionKeepAspectRatioByExpanding_triggered();

private:
    Ui::RtspWindow *ui;
    void PlayRtsp(QString Camuri);
    static QMediaPlayer *player;
    static QVideoWidget *videoWidget;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

};

#endif // RTSPWINDOW_H

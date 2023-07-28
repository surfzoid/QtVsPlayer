#ifndef MULTIMEDIA_SHORTCUTS_H
#define MULTIMEDIA_SHORTCUTS_H

#include <QDialog>
#include <QSettings>
#include <QKeyEvent>

namespace Ui {
class Multimedia_shortcuts;
}

class Multimedia_shortcuts : public QDialog
{
    Q_OBJECT

public:
    explicit Multimedia_shortcuts(QWidget *parent = nullptr);
    ~Multimedia_shortcuts();

private:
    Ui::Multimedia_shortcuts *ui;
    QSettings settings;
    bool IsShown = false;

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_buttonBox_accepted();
};

#endif // MULTIMEDIA_SHORTCUTS_H

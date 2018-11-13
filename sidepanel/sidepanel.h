#ifndef SidePanel_H
#define SidePanel_H

#include "../outputwindow/outputwindow.h"
#include <QWidget>

namespace Ui {
class SidePanel;
}

class SidePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SidePanel(QWidget *parent = nullptr,OutputWindow *mOutputWindow = nullptr);
    ~SidePanel();
signals:
    void change_color_of_ball(QColor);
    void toggle_ball_animation(QString vx,QString vy,QString vz,QString gx,QString gy,QString gz);

public slots:
    void on_colorButton_clicked();
    void on_printData_clicked();
private:
    Ui::SidePanel *ui;
};

#endif

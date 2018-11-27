#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <osg/PositionAttitudeTransform>
#include "block.h"

class OutputWindow;
class OSGWidget;
class OSGWidgetTop;
class OSGWidgetFront;
class OSGWidgetSide;
class SidePanel;


namespace Ui
{
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void add_widgets();
    void add_output_window();
    void add_osg_window();
    osg::PositionAttitudeTransform *create_wireframe_tetrahedron();
    osg::PositionAttitudeTransform *create_wireframe_knife();
    osg::Vec3Array* to_osgArray(std::vector<osg::Vec3d> x);
    void redraw_wireframe_tetrahedron();
    void update_all_views();
    void change_kife(int o);
    void flip_view(int v);
    void undo_slice();
    void undo_paint();
    Block* block;
    QString fileName{""};
public slots:
    void on_actionExit_triggered();
    void on_actionPrint_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_3_triggered();
    void on_actionSave_3_triggered();
private:
    Ui::MainWindowForm *mainWindowUI;
    QDockWidget* outputWindowWidget;
    QDockWidget* panelWidget;
    OutputWindow* outputWindow;
    OSGWidget* osgWidget{nullptr};
    osg::Vec3Array* knifeV;
    osg::Vec4 color;
    osg::Vec4 colorOfLine;
    osg::Vec4Array* cOfLine;
    osg::Geode* geode;
    OSGWidgetTop* osgWidgetTop{nullptr};
    OSGWidgetFront* osgWidgetFront{nullptr};
    OSGWidgetSide* osgWidgetSide{nullptr};
};

#endif

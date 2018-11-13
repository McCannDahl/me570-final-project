#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include <osg/PositionAttitudeTransform>
#include "osgWidgets/osgwidget.h"
#include "osgWidgets/osgwidgetTop.h"
#include "osgWidgets/osgwidgetfront.h"
#include "osgWidgets/osgwidgetside.h"
#include "../outputwindow/outputwindow.h"
#include "../sidepanel/sidepanel.h"
#include "block.h"

#include <QDockWidget>
#include <osg/LineWidth>
#include <osg/Material>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, mainWindowUI{new Ui::MainWindowForm}
{
    mainWindowUI->setupUi(this);
    add_widgets();
}

MainWindow::~MainWindow()
{
    delete mainWindowUI;
}

void MainWindow::add_widgets()
{
    add_output_window();
    add_osg_window();
}

void MainWindow::add_output_window()
{
    outputWindowWidget = new QDockWidget("Output Window",this);
    this->addDockWidget(Qt::RightDockWidgetArea,outputWindowWidget);
    outputWindow = new OutputWindow(this);
    outputWindowWidget->setWidget(outputWindow);
}

void MainWindow::add_osg_window()
{
    osgWidget = new OSGWidget{this,nullptr,outputWindow};
    osgWidgetTop = new OSGWidgetTop{this,nullptr,outputWindow};
    osgWidgetSide = new OSGWidgetSide{this,nullptr,outputWindow};
    osgWidgetFront = new OSGWidgetFront{this,nullptr,outputWindow};

    QWidget *centralWidget = new QWidget;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(osgWidget, 0, 0, 6, 6);
    mainLayout->addWidget(osgWidgetTop, 0, 6, 6, 2);
    mainLayout->addWidget(osgWidgetSide, 6, 0, 2, 6);
    mainLayout->addWidget(osgWidgetFront, 6, 6, 2, 2);
    centralWidget->setLayout(mainLayout);

    this->setCentralWidget(centralWidget);

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionPrint_triggered()
{
    outputWindow->print_string("Printing....");

}

void MainWindow::on_actionSave_3_triggered()
{
    outputWindow->print_string("Saving ");

}

void MainWindow::on_actionSave_as_triggered()
{

    outputWindow->print_string("Saving ");

}

void MainWindow::on_actionOpen_3_triggered()
{

    outputWindow->print_string("Opening ");

}

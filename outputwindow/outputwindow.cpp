#include "outputwindow.h"
#include "ui_outputwindow.h"

#include <QColorDialog>

void OutputWindow::print_string(QString s)
{
    ui->mOutputText->appendPlainText(s);
}

void OutputWindow::on_radioButton_clicked()
{
}

void OutputWindow::on_radioButton_2_clicked()
{
}

void OutputWindow::on_radioButton_3_clicked()
{
}

void OutputWindow::on_pushButton_clicked()
{
}

void OutputWindow::on_pushButton_2_clicked()
{
}

void OutputWindow::on_pushButton_3_clicked()
{
}

void OutputWindow::on_pushButton_4_clicked()
{
}

void OutputWindow::on_pushButton_5_clicked()
{
}

void OutputWindow::on_pushButton_6_clicked()
{
}

OutputWindow::OutputWindow(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::OutputWindow)
{
    ui->setupUi(this);
    mainWindow = parent;
}

OutputWindow::~OutputWindow()
{
    delete ui;
}

#include "outputwindow.h"
#include "ui_outputwindow.h"

#include <QColorDialog>

void OutputWindow::print_string(QString s)
{
    ui->mOutputText->appendPlainText(s);
}

void OutputWindow::clear_output()
{
    ui->mOutputText->clear();
}

void OutputWindow::on_radioButton_clicked()
{
    mainWindow->change_kife(1);
}

void OutputWindow::on_radioButton_2_clicked()
{
    mainWindow->change_kife(0);
}

void OutputWindow::on_radioButton_3_clicked()
{
    mainWindow->change_kife(2);
}

void OutputWindow::on_pushButton_clicked()
{
    mainWindow->flip_view(0);
}

void OutputWindow::on_pushButton_2_clicked()
{
    mainWindow->flip_view(1);
}

void OutputWindow::on_pushButton_3_clicked()
{
    mainWindow->flip_view(2);
}

void OutputWindow::on_pushButton_4_clicked()
{
    mainWindow->undo_slice();
}

void OutputWindow::on_pushButton_5_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->label->setText(color.name());
        ui->label->setPalette(QPalette(color));
        ui->label->setAutoFillBackground(true);
    }
    mainWindow->block->paintBrushColor = osg::Vec4(color.redF(),color.greenF(),color.blueF(),color.alphaF());
}

void OutputWindow::on_pushButton_6_clicked()
{
    mainWindow->undo_paint();
}

void OutputWindow::on_pushButton_7_clicked()
{
    clear_output();
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

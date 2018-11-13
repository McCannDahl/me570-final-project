#include "sidepanel.h"
#include "ui_sidepanel.h"
#include "../outputwindow/outputwindow.h"
#include <QColorDialog>
#include <QColor>
#include <QPalette>
#include <QDoubleValidator>

SidePanel::SidePanel(QWidget *parent,OutputWindow *mOutputWindow) :
    QWidget(parent),
    ui(new Ui::SidePanel)
{
    ui->setupUi(this);

    QDoubleValidator* doubleValidator = new QDoubleValidator;
    ui->initialVelocity->setValidator(doubleValidator);
    ui->lineEdit->setValidator(doubleValidator);
    ui->lineEdit_2->setValidator(doubleValidator);
    ui->lineEdit_3->setValidator(doubleValidator);
    ui->lineEdit_4->setValidator(doubleValidator);
    ui->lineEdit_5->setValidator(doubleValidator);

    //This is how to connect a text edit
    //connect(ui->initialVelocity, SIGNAL(textChanged(const QString &)), mOutputWindow, SLOT(print_string(const QString &)));
}

SidePanel::~SidePanel()
{
    delete ui;
}
void SidePanel::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor();
    ui->selectedColor->setText(color.name());
    ui->selectedColor->setPalette(QPalette(color));
    ui->selectedColor->setAutoFillBackground(true);
    emit(change_color_of_ball(color));
}

void SidePanel::on_printData_clicked()
{
    emit(toggle_ball_animation(ui->initialVelocity->text(),ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text()));
}

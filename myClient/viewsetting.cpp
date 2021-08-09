#include "viewsetting.h"
#include "ui_viewsetting.h"
#include <QDebug>

viewSetting::viewSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewSetting)
{
    ui->setupUi(this);
    ui->dark_radioButton->setChecked(true);
    ui->whiteBackgroundRadioButton->setChecked(true);
    ui->antialiasing_acheckBox->setChecked(true);
    ui->viewPoints_checkBox->setChecked(true);
    ui->darkPoints_radioButton->setChecked(true);
    ui->size_spinBox->setRange(1,5);
    ui->size_spinBox->setValue(1);
    ui->pointsSize_spinBox->setRange(1,10);
    ui->pointsSize_spinBox->setValue(4);
}

viewSetting::~viewSetting()
{
    delete ui;
}

void viewSetting::on_cancel_clicked()
{
    this->close();
}


void viewSetting::on_apply_clicked()
{
    if(ui->dark_radioButton->isChecked()){
        red=0;
        green=0;
        blue=0;
    }
    else if(ui->red_radioButton->isChecked()){
        red=255;
        green=0;
        blue=0;
    }
    else if(ui->green_radioButton->isChecked()){
        red=0;
        green=255;
        blue=0;
    }
    else{
        red=0;
        green=200;
        blue=255;
    }

    if(ui->darkPoints_radioButton->isChecked()){
        redPoints=0;
        greenPoints=0;
        bluePoints=0;
    }
    else if(ui->whitePoints_radioButton->isChecked()){
        redPoints=255;
        greenPoints=255;
        bluePoints=255;
    }
    else if(ui->bluePoints_radioButton->isChecked()){
        redPoints=0;
        greenPoints=200;
        bluePoints=255;
    }
    else {
        redPoints=255;
        greenPoints=0;
        bluePoints=0;
    }


    if(ui->whiteBackgroundRadioButton->isChecked()){
        background_color=1;
    }
    else if(ui->greyBackgroundRadioButton->isChecked()){
        background_color=2;
    }
    else if(ui->redBackgroundRadioButton->isChecked()){
        background_color=3;
    }
    else background_color=4;

    size_line=ui->size_spinBox->value();
    size_points=ui->pointsSize_spinBox->value();

    if(!ui->antialiasing_acheckBox->isChecked())
        antialiasing = 0;
    else antialiasing = 1;

    if(!ui->viewPoints_checkBox->isChecked()){
        viewPoints=0;
    }
    else viewPoints=1;
    this->close();
}


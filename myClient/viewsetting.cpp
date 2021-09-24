#include "viewsetting.h"
#include "ui_viewsetting.h"
#include <QDebug>
#include <QFile>
viewSetting::viewSetting(Settings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewSetting),
    settings(settings)
{
    ui->setupUi(this);

}


viewSetting::~viewSetting()
{
    delete ui;
}

void viewSetting::updateValue(Settings &settings)
{
    if(settings.getRed()==settings.getGreen()==settings.getBlue()==0)
        ui->dark_radioButton->setChecked(true);
    else if(settings.getRed()==255 && settings.getGreen() == 0 && settings.getBlue() == 0)
        ui->red_radioButton->setChecked(true);
    else if(settings.getRed()==0 && settings.getGreen() == 255 && settings.getBlue() == 0)
        ui->green_radioButton->setChecked(true);
    else ui->blue_radioButton->setChecked(true);

    if(settings.getBackgroundColor() == 1){
        ui->whiteBackgroundRadioButton->setChecked(true);
    }
    else if(settings.getBackgroundColor() == 2){
        ui->greyBackgroundRadioButton->setChecked(true);
    }
    else if(settings.getBackgroundColor() == 3){
        ui->redBackgroundRadioButton->setChecked(true);
    }
    else ui->blueBackgroundRadioButton->setChecked(true);

    if(settings.getAntialiasing())
        ui->antialiasing_acheckBox->setChecked(true);
    if(settings.getViewPoints())
        ui->viewPoints_checkBox->setChecked(true);


    if(settings.getRedPoints()==settings.getGreenPoints()==settings.getBluePoints()==0)
        ui->darkPoints_radioButton->setChecked(true);
    else if(settings.getRedPoints()==255 && settings.getGreenPoints() == 0 && settings.getBluePoints() == 0)
        ui->redPoints_radioButton->setChecked(true);
    else if(settings.getRedPoints()==0 && settings.getGreenPoints() == 200 && settings.getBluePoints() == 255)
        ui->bluePoints_radioButton->setChecked(true);
    else ui->whitePoints_radioButton->setChecked(true);

    ui->size_spinBox->setRange(1,5);
    ui->size_spinBox->setValue(settings.getSizeLine());
    ui->pointsSize_spinBox->setRange(1,10);
    ui->pointsSize_spinBox->setValue(settings.getSizePoints());
}

void viewSetting::on_cancel_clicked()
{
    this->close();
}

void viewSetting::on_apply_clicked()
{
    if(ui->dark_radioButton->isChecked()){
        settings.setLineColor(0,0,0);
    }
    else if(ui->red_radioButton->isChecked()){
        settings.setLineColor(255,0,0);
    }
    else if(ui->green_radioButton->isChecked()){
        settings.setLineColor(0,255,0);
    }
    else{
        settings.setLineColor(0,200,255);
    }

    if(ui->darkPoints_radioButton->isChecked()){
        settings.setPointColor(0,0,0);
    }
    else if(ui->whitePoints_radioButton->isChecked()){
        settings.setPointColor(255,255,255);
    }
    else if(ui->bluePoints_radioButton->isChecked()){
        settings.setPointColor(0,200,255);
    }
    else {
        settings.setPointColor(255,0,0);
    }


    if(ui->whiteBackgroundRadioButton->isChecked()){
        settings.setBackgroundColor(1);
    }
    else if(ui->greyBackgroundRadioButton->isChecked()){
        settings.setBackgroundColor(2);
    }
    else if(ui->redBackgroundRadioButton->isChecked()){
        settings.setBackgroundColor(3);
    }
    else settings.setBackgroundColor(4);

    settings.setSizeLine(ui->size_spinBox->value());
    settings.setSizePoints(ui->pointsSize_spinBox->value());

    if(!ui->antialiasing_acheckBox->isChecked())
        settings.setAntialiasing(0);
    else settings.setAntialiasing(1);

    if(!ui->viewPoints_checkBox->isChecked()){
        settings.setViewPoints(0);
    }
    else settings.setViewPoints(1);

    this->close();
}

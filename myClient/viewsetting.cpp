#include "viewsetting.h"
#include "ui_viewsetting.h"
#include <QDebug>
#include <QFile>
viewSetting::viewSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewSetting)
{
    ui->setupUi(this);

    settings.clear();
    QFile file(QCoreApplication::applicationDirPath() + "settings.bin");
    if(file.open(QIODevice::ReadOnly)){
        QDataStream stream(&file);
        stream >> settings;
        if(stream.status() != QDataStream::Ok){
            qDebug() << "Ошибка чтения файлы с настройками";
        }
    }
    file.close();
    red=settings[0];
    green=settings[1];
    blue = settings[2];
    size_line = settings[3];
    redPoints = settings[4];
    greenPoints = settings[5];
    bluePoints = settings[6];
    size_points = settings[7];
    background_color = settings[8];
    antialiasing = settings[9];
    viewPoints = settings[10];
    settings.clear();

    if(red==green==blue==0)
        ui->dark_radioButton->setChecked(true);
    else if(red==255 && green == 0 && blue == 0)
        ui->red_radioButton->setChecked(true);
    else if(red==0 && green == 255 && blue == 0)
        ui->green_radioButton->setChecked(true);
    else ui->blue_radioButton->setChecked(true);

    if(background_color == 1){
        ui->whiteBackgroundRadioButton->setChecked(true);
    }
    else if(background_color == 2){
        ui->greyBackgroundRadioButton->setChecked(true);
    }
    else if(background_color == 3){
        ui->redBackgroundRadioButton->setChecked(true);
    }
    else ui->blueBackgroundRadioButton->setChecked(true);

    if(antialiasing)
        ui->antialiasing_acheckBox->setChecked(true);
    if(viewPoints)
        ui->viewPoints_checkBox->setChecked(true);


    if(redPoints==greenPoints==bluePoints==0)
        ui->darkPoints_radioButton->setChecked(true);
    else if(redPoints==255 && greenPoints == 0 && bluePoints == 0)
        ui->redPoints_radioButton->setChecked(true);
    else if(redPoints==0 && greenPoints == 200 && bluePoints == 255)
        ui->bluePoints_radioButton->setChecked(true);
    else ui->whitePoints_radioButton->setChecked(true);

    ui->size_spinBox->setRange(1,5);
    ui->size_spinBox->setValue(size_line);
    ui->pointsSize_spinBox->setRange(1,10);
    ui->pointsSize_spinBox->setValue(size_points);

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


    settings.clear();
    settings.push_back(red);
    settings.push_back(green);
    settings.push_back(blue);
    settings.push_back(size_line);
    settings.push_back(redPoints);
    settings.push_back(greenPoints);
    settings.push_back(bluePoints);
    settings.push_back(size_points);
    settings.push_back(background_color);
    settings.push_back(antialiasing);
    settings.push_back(viewPoints);
    QFile file(QCoreApplication::applicationDirPath() + "settings.bin");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream stream(&file);
        stream<<settings;
        if(stream.status() != QDataStream::Ok){
            qDebug() << "Ошибка записи";
        }
    }
    file.close();
    for(int i=0; i<settings.size();i++){
        qDebug() << settings[i];
    }
    settings.clear();
    this->close();
}


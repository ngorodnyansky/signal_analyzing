#include "settings.h"

void MainWindow::readSettings(){
    settings.beginGroup("/Settings");

    setting_window.red = settings.value("/redColorLine",0).toInt();
    setting_window.green = settings.value("/greenColorLine",0).toInt();
    setting_window.blue = settings.value("/blueColorLine",0).toInt();
    setting_window.size_line = settings.value("/sizeLine",1).toInt();
    setting_window.antialiasing = settings.value("/antialiasing",1).toBool();
    setting_window.redPoints = settings.value("/redPointsColorLine",0).toInt();
    setting_window.greenPoints = settings.value("/greenPointsColorLine",0).toInt();
    setting_window.bluePoints = settings.value("/bluePointsColorLine",0).toInt();
    setting_window.size_points = settings.value("/sizePoints",4).toInt();
    setting_window.viewPoints = settings.value("/pointsView",1).toBool();
    setting_window.background_color = settings.value("/backgroundColor",1).toInt();

    settings.endGroup();
}

void MainWindow::writeSettings(){
    settings.beginGroup("/Settings");

    settings.setValue("/redColorLine",setting_window.red);
    settings.setValue("/greenColorLine",setting_window.green);
    settings.setValue("/blueColorLine",setting_window.blue);
    settings.setValue("/sizeLine",setting_window.size_line);
    settings.setValue("/antialiasing",setting_window.antialiasing);
    settings.setValue("/redPointsColorLine",setting_window.redPoints);
    settings.setValue("/greenPointsColorLine",setting_window.greenPoints);
    settings.setValue("/bluePointsColorLine",setting_window.bluePoints);
    settings.setValue("/sizePoints",setting_window.size_points);
    settings.setValue("/pointsView",setting_window.viewPoints);
    settings.setValue("/antialiasing",setting_window.antialiasing);
    settings.setValue("/backgroundColor",setting_window.background_color);

    settings.endGroup();
}

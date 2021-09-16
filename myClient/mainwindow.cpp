#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
     settings("settings.ini", QSettings::IniFormat), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
        readSettings();
        ui->amplitude_Slider->setRange(1,50);
        ui->amplitude_Slider->setValue(amplitude);
        ui->frequency_Slider->setRange(5,40);
        ui->frequency_Slider->setValue(frequency);
        xBegin = 0;
        xEnd = 5;

        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5,5);

        if(setting_window.background_color==1){
            ui->widget->setBackground(Qt::white);
        }
        else if(setting_window.background_color==2){
            ui->widget->setBackground(Qt::gray);
        }
        else if(setting_window.background_color==3){
            ui->widget->setBackground(Qt::red);
        }
        else ui->widget->setBackground(Qt::blue);

        ui->disconnectButton->setEnabled(false);
        ui->disconnectButton->repaint();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::sockDisc()
{
    QMessageBox *disc = new QMessageBox(QMessageBox::Warning,"MessageBox",
                                        "Произошло отключение от сервера",
                                        QMessageBox::Ok);
    int n = disc->exec();

    if(n==QMessageBox::Ok){
        delete disc;
    }
}

void MainWindow::sockReady()
{
    QColor lineColor(setting_window.red,setting_window.green,setting_window.blue);
    QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
    ui->widget->setInteraction(QCP::iRangeDrag, false);
    ui->widget->setInteraction(QCP::iRangeZoom, false);
    Data = socket->readAll();
    double ordinate = QVariant(Data).toDouble();
    allData.add(time, ordinate);
    time+=h;
    amplitude = ui->amplitude_Slider->value();
    frequency = ui->frequency_Slider->value();

    dataToServer.push_back(time);
    dataToServer.push_back(amplitude);
    dataToServer.push_back(frequency);

    QDataStream out(&Data,QIODevice::WriteOnly);
    out << dataToServer;

    socket->write(Data);
    socket->waitForBytesWritten();

    dataToServer.clear();

    ui->widget->clearGraphs();
    if(time<=5){
        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(lineColor,setting_window.size_line));
        ui->widget->graph(0)->addData(allData.xview,allData.yview);




        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);
        if(setting_window.viewPoints){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
            ui->widget->graph(1)->setPen(pointsColor);
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(allData.extremums_x,allData.extremums_y);
        }


        ui->widget->replot();
    }
    else{
        ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(lineColor,setting_window.size_line));
        ui->widget->graph(0)->addData(allData.xview,allData.yview);

        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);

        if(setting_window.viewPoints){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
            ui->widget->graph(1)->setPen(pointsColor);
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(allData.extremums_x,allData.extremums_y);
        }
        ui->widget->replot();
    }
}

void MainWindow::on_connectButton_clicked()
{
    time = 0;
    allData.clear();
    socket->connectToHost("127.0.0.1",6000);
    dataToServer.push_back(time);
    dataToServer.push_back(ui->amplitude_Slider->value());
    dataToServer.push_back(ui->frequency_Slider->value());

    QDataStream out(&Data,QIODevice::WriteOnly);
    out << dataToServer;

    socket->write(Data);
    socket->waitForBytesWritten();

    dataToServer.clear();

    ui->connectButton->setEnabled(false);
    ui->connectButton->repaint();

    ui->disconnectButton->setEnabled(true);
    ui->disconnectButton->repaint();
}

void MainWindow::on_disconnectButton_clicked()
{
    QColor color(setting_window.red,setting_window.green,setting_window.blue);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);

    ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
    ui->widget->yAxis->setRange(-5,5);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
    ui->widget->graph(0)->addData(allData.x,allData.y);

    if(!setting_window.antialiasing)
        ui->widget->graph(0)->setAntialiased(false);
    if(setting_window.viewPoints){
        QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
        ui->widget->addGraph();
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
        ui->widget->graph(1)->setPen(pointsColor);
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(1)->addData(allData.extremums_xview,allData.extremums_yview);
    }


    ui->widget->replot();
    socket->disconnectFromHost();

    ui->disconnectButton->setEnabled(false);
    ui->disconnectButton->repaint();

    ui->connectButton->setEnabled(true);
    ui->connectButton->repaint();
}

void MainWindow::on_SettingAction_triggered()
{
    setting_window.setModal(true);
    setting_window.updateValue();
    setting_window.exec();
    QColor color(setting_window.red,setting_window.green,setting_window.blue);

    ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
    ui->widget->yAxis->setRange(-5,5);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
    ui->widget->graph(0)->addData(allData.x,allData.y);

    if(!setting_window.antialiasing)
        ui->widget->graph(0)->setAntialiased(false);
    if(setting_window.viewPoints){
        QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
        ui->widget->addGraph();
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
        ui->widget->graph(1)->setPen(pointsColor);
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(1)->addData(allData.extremums_xview,allData.extremums_yview);
    }

    if(setting_window.background_color==1){
        ui->widget->setBackground(Qt::white);
    }
    else if(setting_window.background_color==2){
        ui->widget->setBackground(Qt::gray);
    }
    else if(setting_window.background_color==3){
        ui->widget->setBackground(Qt::red);
    }
    else ui->widget->setBackground(Qt::blue);

    ui->widget->replot();

}

void MainWindow::on_action_exit_triggered()
{
    this->close();
}

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
    amplitude = settings.value("/Amplitude", 20).toDouble();
    frequency = settings.value("/Frequency", 10).toDouble();

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
    settings.setValue("/Amplitude", amplitude);
    settings.setValue("/Frequency", frequency);

    settings.endGroup();
}

void MainWindow::on_action_save_triggered()
{
    QMessageBox *pmbx;
    if(time==0){
        pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                            "График не должен быть пустым для сохранения!\n",
                                            QMessageBox::Ok);
        int n = pmbx->exec();
        if(n==QMessageBox::Ok){
           delete pmbx;
        }
    }
    else if(socket->state()== QTcpSocket::ConnectedState){
        pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                            "Отключитесь от сервера для сохранения графика!\n",
                                            QMessageBox::Ok);
        int n = pmbx->exec();
        if(n==QMessageBox::Ok){
           delete pmbx;
        }
    }
    else {
        fileWork.save(allData);
    }
}

void MainWindow::on_action_open_triggered()
{
    QMessageBox *pmbx;
    if(socket->state()== QTcpSocket::ConnectedState){
        pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                            "Отключитесь от сервера для открытия графика!\n",
                                            QMessageBox::Ok);
        int n = pmbx->exec();
        if(n==QMessageBox::Ok){
           delete pmbx;
        }
    }
    else if(time!=0){
            pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                                "При открытии графика существующий будет стёрт. Продолжить?\n",
                                               QMessageBox::Ok | QMessageBox::Cancel);
               int n = pmbx->exec();
               if(n==QMessageBox::Ok){
                  delete pmbx;
                   fileWork.open(allData);


                       QColor color(setting_window.red,setting_window.green,setting_window.blue);
                       ui->widget->clearGraphs();
                       ui->widget->setInteraction(QCP::iRangeDrag, true);
                       ui->widget->setInteraction(QCP::iRangeZoom, true);

                       ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
                       ui->widget->yAxis->setRange(-5,5);

                       ui->widget->addGraph();
                       ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
                       ui->widget->graph(0)->addData(allData.x,allData.y);

                       if(!setting_window.antialiasing)
                           ui->widget->graph(0)->setAntialiased(false);

                       if(setting_window.viewPoints){
                           QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
                           ui->widget->addGraph();
                           ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
                           ui->widget->graph(1)->setPen(pointsColor);
                           ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
                           ui->widget->graph(1)->addData(allData.extremums_xview,allData.extremums_yview);
                       }

                       ui->amplitude_Slider->setValue(amplitude);
                       ui->frequency_Slider->setValue(frequency);

                       ui->widget->replot();


            }
            if(n==QMessageBox::Cancel) delete pmbx;
        }
        else {


            fileWork.open(allData);

            QColor color(setting_window.red,setting_window.green,setting_window.blue);
            ui->widget->clearGraphs();
            ui->widget->setInteraction(QCP::iRangeDrag, true);
            ui->widget->setInteraction(QCP::iRangeZoom, true);

            ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
            ui->widget->yAxis->setRange(-5,5);

            ui->widget->addGraph();
            ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
            ui->widget->graph(0)->addData(allData.x,allData.y);

            if(!setting_window.antialiasing)
                ui->widget->graph(0)->setAntialiased(false);

            if(setting_window.viewPoints){
                QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
                ui->widget->addGraph();
                ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
                ui->widget->graph(1)->setPen(pointsColor);
                ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
                ui->widget->graph(1)->addData(allData.extremums_xview,allData.extremums_yview);
            }

            ui->amplitude_Slider->setValue(amplitude);
            ui->frequency_Slider->setValue(frequency);

            ui->widget->replot();

    }

}


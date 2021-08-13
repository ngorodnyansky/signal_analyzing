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
    y.push_back(ordinate);
    x.push_back(time);
    if(time*100<=area_limit){
        xview.push_back(time);
        yview.push_back(ordinate);
    }
    else{
        for(int i=0;i<area_limit;++i){
            xview.swapItemsAt(i,i+1);
            yview.swapItemsAt(i,i+1);
        }
        int k=xview.size();
        xview[k-1]=time;
        yview[k-1]=ordinate;
    }
    int n = y.size();
    if(n>=3){
        if(((y[n-3]-y[n-2])/0.01>0 && (y[n-2]-y[n-1])/0.01<0) || ((y[n-3]-y[n-2])/0.01<0 && (y[n-2]-y[n-1])/0.01>0)){
            extremums_x.push_back(time-0.01);
            extremums_y.push_back(y[n-2]);
            extremums_xview.push_back(time-0.01);
            extremums_yview.push_back(y[n-2]);
            if(extremums_xview.size()==area_limit){
                for(int i=0;i<area_limit;++i){
                   extremums_xview.swapItemsAt(i,i+1);
                    extremums_yview.swapItemsAt(i,i+1);
                }
                int j = extremums_xview.size();
                extremums_xview[j-1]=time-0.01;
                extremums_yview[j-1]=y[n-2];
            }
        }
    }

    time+=h;
    amplitude = ui->amplitude_Slider->value();
    frequency = ui->frequency_Slider->value();

    massiv.push_back(time);
    massiv.push_back(amplitude);
    massiv.push_back(frequency);

    QDataStream out(&Data,QIODevice::WriteOnly);
    out << massiv;

    socket->write(Data);
    socket->waitForBytesWritten();

    massiv.clear();

    ui->widget->clearGraphs();
    if(time<=5){
        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(lineColor,setting_window.size_line));
        ui->widget->graph(0)->addData(xview,yview);




        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);
        if(setting_window.viewPoints){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
            ui->widget->graph(1)->setPen(pointsColor);
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(extremums_x,extremums_y);
        }


        ui->widget->replot();
    }
    else{
        ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(lineColor,setting_window.size_line));
        ui->widget->graph(0)->addData(xview,yview);

        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);

        if(setting_window.viewPoints){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
            ui->widget->graph(1)->setPen(pointsColor);
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(extremums_x,extremums_y);
        }
        ui->widget->replot();
    }




}

void MainWindow::on_connectButton_clicked()
{
    if(time==0){
        socket->connectToHost("127.0.0.1",6000);
        massiv.push_back(time);
        massiv.push_back(ui->amplitude_Slider->value());
        massiv.push_back(ui->frequency_Slider->value());

        QDataStream out(&Data,QIODevice::WriteOnly);
        out << massiv;

        socket->write(Data);
        socket->waitForBytesWritten();

        massiv.clear();

    }
    else{
        socket->connectToHost("127.0.0.1",6000);
        QMessageBox *pmbx = new QMessageBox(QMessageBox::Question,"MessageBox",
                                            "Продолжить рисовать график или начать сначала?\n"
                                            "Yes - Продолжить \nNo - Начать сначала\nCancel - Отмена ",
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int n = pmbx->exec();

        if(n==QMessageBox::Yes){
            massiv.push_back(time);
            massiv.push_back(ui->amplitude_Slider->value());
            massiv.push_back(ui->frequency_Slider->value());

            QDataStream out(&Data,QIODevice::WriteOnly);
            out << massiv;

            socket->write(Data);
            socket->waitForBytesWritten();

            massiv.clear();
            delete pmbx;
        }
        else if(n==QMessageBox::No){
            time=0;
            massiv.push_back(time);
            massiv.push_back(ui->amplitude_Slider->value());
            massiv.push_back(ui->frequency_Slider->value());

            QDataStream out(&Data,QIODevice::WriteOnly);
            out << massiv;

            socket->write(Data);
            socket->waitForBytesWritten();

            massiv.clear();
            delete pmbx;
            x.clear();
            y.clear();
            xview.clear();
            yview.clear();
            extremums_yview.clear();
            extremums_xview.clear();
            extremums_y.clear();
            extremums_x.clear();
        }
        else delete pmbx;

    }
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
    ui->widget->graph(0)->addData(x,y);

    if(!setting_window.antialiasing)
        ui->widget->graph(0)->setAntialiased(false);
    if(setting_window.viewPoints){
        QColor pointsColor(setting_window.redPoints,setting_window.greenPoints,setting_window.bluePoints);
        ui->widget->addGraph();
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, setting_window.size_points));
        ui->widget->graph(1)->setPen(pointsColor);
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(1)->addData(extremums_xview,extremums_yview);
    }


    ui->widget->replot();
    socket->disconnectFromHost();
}

void MainWindow::on_SettingAction_triggered()
{
    setting_window.setModal(true);
    setting_window.updateValue();
    setting_window.exec();
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

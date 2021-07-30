#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
        ui->amplitude_Slider->setRange(1,50);
        ui->amplitude_Slider->setValue(10);
        ui->speed_Slider->setRange(400000,1030000);
        ui->speed_Slider->setValue(800000);
        h = 0.1;
        xBegin = 0;
        xEnd = 5;

        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5,5);


}

MainWindow::~MainWindow()
{
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
    QColor color(setting_window.red,setting_window.green,setting_window.blue);
    ui->widget->setInteraction(QCP::iRangeDrag, false);
    ui->widget->setInteraction(QCP::iRangeZoom, false);
    Data = socket->readAll();
    double ordinate = QVariant(Data).toDouble();
    qDebug() << ordinate;
    y.push_back(ordinate);
    x.push_back(time);
    if(time*10<=50){
        xview.push_back(time);
        yview.push_back(ordinate);
    }
    else{
        for(int i=0;i<50;++i){
            xview.swapItemsAt(i,i+1);
            yview.swapItemsAt(i,i+1);
        }
        xview[49]=time;
        yview[49]=ordinate;
    }

    time+=h;
    massiv.push_back(time);
    massiv.push_back(ui->amplitude_Slider->value());
    massiv.push_back(ui->speed_Slider->value());

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
        ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
        ui->widget->graph(0)->addData(xview,yview);

        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);

        ui->widget->replot();
    }
    else{
        ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(color,setting_window.size_line));
        ui->widget->graph(0)->addData(xview,yview);

        if(!setting_window.antialiasing)
            ui->widget->graph(0)->setAntialiased(false);

        ui->widget->replot();
    }
}

void MainWindow::on_connectButton_clicked()
{
    if(time==0){
        socket->connectToHost("127.0.0.1",6000);
        massiv.push_back(time);
        massiv.push_back(ui->amplitude_Slider->value());
        massiv.push_back(ui->speed_Slider->value());

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
            massiv.push_back(ui->speed_Slider->value());

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
            massiv.push_back(ui->speed_Slider->value());

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

    ui->widget->replot();
    socket->disconnectFromHost();
}

void MainWindow::on_SettingAction_triggered()
{
    setting_window.setModal(true);
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








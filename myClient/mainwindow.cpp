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
    amplitude=ui->amplitude_Slider->value();
    ui->widget->setInteraction(QCP::iRangeDrag, false);
    ui->widget->setInteraction(QCP::iRangeZoom, false);
    Data = socket->readAll();
    double ordinate = QVariant(Data).toDouble();
    qDebug() << ordinate;
    y.push_back(ordinate*amplitude*0.1);
    x.push_back(time);
    if(time*10<=50){
        xview.push_back(time);
        yview.push_back(ordinate*amplitude*0.1);
    }
    else{
        for(int i=0;i<50;++i){
            xview.swapItemsAt(i,i+1);
            yview.swapItemsAt(i,i+1);
        }
        xview[49]=time;
        yview[49]=ordinate*amplitude*0.1;
    }

    time+=h;
    ui->widget->clearGraphs();
    if(time<=5){
        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->addData(xview,yview);
        ui->widget->replot();
    }
    else{
        ui->widget->xAxis->setRange(xBegin+time-5-5,xEnd+time-5+5);
        ui->widget->yAxis->setRange(-5,5);

        ui->widget->addGraph();
        ui->widget->graph(0)->addData(xview,yview);
        ui->widget->replot();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(time==-1){
        socket->connectToHost("127.0.0.1",6000);
        socket->write(QByteArray::fromStdString(QVariant(time).toString().toStdString()));
        socket->waitForBytesWritten();
        time = 0;
    }
    else{
        socket->connectToHost("127.0.0.1",6000);
        QMessageBox *pmbx = new QMessageBox(QMessageBox::Question,"MessageBox",
                                            "Продолжить рисовать график или начать сначала?\n"
                                            "Yes - Продолжить \nNo - Начать сначала\nCancel - Отмена ",
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int n = pmbx->exec();

        if(n==QMessageBox::Yes){
            socket->write(QByteArray::fromStdString(QVariant(time).toString().toStdString()));
            socket->waitForBytesWritten();
            delete pmbx;
        }
        else if(n==QMessageBox::No){
            time=-1;
            socket->connectToHost("127.0.0.1",6000);
            socket->write(QByteArray::fromStdString(QVariant(time).toString().toStdString()));
            socket->waitForBytesWritten();
            delete pmbx;
            x.clear();
            y.clear();
            xview.clear();
            yview.clear();
            time = 0;
        }
        else delete pmbx;

    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);

    ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
    ui->widget->yAxis->setRange(-5,5);

    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x,y);
    ui->widget->replot();
    socket->disconnectFromHost();
}


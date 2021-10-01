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
        settings.readSettings();
        ui->amplitude_Slider->setRange(1,50);
        ui->amplitude_Slider->setValue(amplitude);
        ui->frequency_Slider->setRange(5,40);
        ui->frequency_Slider->setValue(frequency);
        xBegin = 0;
        xEnd = 5;

        ui->widget->xAxis->setRange(xBegin,xEnd);
        ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

        if(settings.getBackgroundColor()==1){
            ui->widget->setBackground(Qt::white);
        }
        else if(settings.getBackgroundColor()==2){
            ui->widget->setBackground(Qt::gray);
        }
        else if(settings.getBackgroundColor()==3){
            ui->widget->setBackground(Qt::red);
        }
        else ui->widget->setBackground(Qt::blue);

        ui->disconnectButton->setEnabled(false);
        ui->disconnectButton->repaint();
}

MainWindow::~MainWindow()
{
    settings.writeSettings();
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
        ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
        ui->widget->graph(0)->addData(allData.getAbscisa(),allData.getOrdinate());




        if(!settings.getAntialiasing())
            ui->widget->graph(0)->setAntialiased(false);
        if(settings.getViewPoints()){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
            ui->widget->graph(1)->setPen(settings.getPointColor());
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(allData.getExtremumAbscissa(),allData.getExtremumOrdinate());
        }


        ui->widget->replot();
    }
    else{
        ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
        ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

        ui->widget->addGraph();
        ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
        ui->widget->graph(0)->addData(allData.getAbscissaView(),allData.getOrdinateView());

        if(!settings.getAntialiasing())
            ui->widget->graph(0)->setAntialiased(false);

        if(settings.getViewPoints()){
            ui->widget->addGraph();
            ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
            ui->widget->graph(1)->setPen(settings.getPointColor());
            ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
            ui->widget->graph(1)->addData(allData.getExtremumAbscissa(),allData.getExtremumOrdinate());
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
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);

    if(time>=5){
        ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
    }
    else ui->widget->xAxis->setRange(xBegin,xEnd);

    ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
    ui->widget->graph(0)->addData(allData.getAbscisa(),allData.getOrdinate());

    if(!settings.getAntialiasing())
        ui->widget->graph(0)->setAntialiased(false);
    if(settings.getViewPoints()){
        ui->widget->addGraph();
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
        ui->widget->graph(1)->setPen(settings.getPointColor());
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(1)->addData(allData.getExtremumAbscissa(),allData.getExtremumOrdinate());
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

    viewSetting setting_window(settings);
    setting_window.update();
    setting_window.setModal(true);
    setting_window.updateValue(settings);
    setting_window.exec();

    ui->widget->xAxis->setRange(xBegin+time-5,xEnd+time-5);
    ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
    ui->widget->graph(0)->addData(allData.getAbscisa(),allData.getOrdinate());

    if(!settings.getAntialiasing())
        ui->widget->graph(0)->setAntialiased(false);
    if(settings.getViewPoints()){
        ui->widget->addGraph();
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
        ui->widget->graph(1)->setPen(settings.getPointColor());
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(1)->addData(allData.getExtremumAbscissa(),allData.getExtremumOrdinate());
    }

    if(settings.getBackgroundColor()==1){
        ui->widget->setBackground(Qt::white);
    }
    else if(settings.getBackgroundColor()==2){
        ui->widget->setBackground(Qt::gray);
    }
    else if(settings.getBackgroundColor()==3){
        ui->widget->setBackground(Qt::red);
    }
    else ui->widget->setBackground(Qt::blue);

    ui->widget->replot();

}

void MainWindow::on_action_exit_triggered()
{
    this->close();
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

                       ui->widget->clearGraphs();
                       ui->widget->setInteraction(QCP::iRangeDrag, true);
                       ui->widget->setInteraction(QCP::iRangeZoom, true);

                       ui->widget->xAxis->setRange(xBegin,xEnd);
                       ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

                       ui->widget->addGraph();
                       ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
                       ui->widget->graph(0)->addData(allData.getAbscisa(),allData.getOrdinate());

                       if(!settings.getAntialiasing())
                           ui->widget->graph(0)->setAntialiased(false);

                       if(settings.getViewPoints()){
                           ui->widget->addGraph();
                           ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
                           ui->widget->graph(1)->setPen(settings.getPointColor());
                           ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
                           ui->widget->graph(1)->addData(allData.getExtremumAbscissaView(),allData.getExtremumOrdinateView());
                       }

                       ui->amplitude_Slider->setValue(amplitude);
                       ui->frequency_Slider->setValue(frequency);

                       ui->widget->replot();


            }
            if(n==QMessageBox::Cancel) delete pmbx;
        }
        else {

            fileWork.open(allData);

            ui->widget->clearGraphs();
            ui->widget->setInteraction(QCP::iRangeDrag, true);
            ui->widget->setInteraction(QCP::iRangeZoom, true);

            ui->widget->xAxis->setRange(xBegin,xEnd);
            ui->widget->yAxis->setRange(-5-verticalPlus,5+verticalPlus);

            ui->widget->addGraph();
            ui->widget->graph(0)->setPen(QPen(settings.getLineColor(),settings.getSizeLine()));
            ui->widget->graph(0)->addData(allData.getAbscisa(),allData.getOrdinate());

            if(!settings.getAntialiasing())
                ui->widget->graph(0)->setAntialiased(false);

            if(settings.getViewPoints()){
                ui->widget->addGraph();
                ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, settings.getSizePoints()));
                ui->widget->graph(1)->setPen(settings.getPointColor());
                ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
                ui->widget->graph(1)->addData(allData.getExtremumAbscissaView(),allData.getExtremumOrdinateView());
            }

            ui->amplitude_Slider->setValue(amplitude);
            ui->frequency_Slider->setValue(frequency);

            ui->widget->replot();

    }

}


void MainWindow::on_pushButton_2_clicked()
{
    if(verticalPlus!=0)
    {
        verticalPlus-=10;
    }
}


void MainWindow::on_plus_clicked()
{
    verticalPlus+=10;
}


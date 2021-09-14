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

        ui->connectButton->setEnabled(false);
        ui->connectButton->repaint();

        ui->disconnectButton->setEnabled(true);
        ui->disconnectButton->repaint();

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

            ui->connectButton->setEnabled(false);
            ui->connectButton->repaint();

            ui->disconnectButton->setEnabled(true);
            ui->disconnectButton->repaint();
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
            allData.x.clear();
            allData.y.clear();
            allData.xview.clear();
            allData.yview.clear();
            allData.extremums_yview.clear();
            allData.extremums_xview.clear();
            allData.extremums_y.clear();
            allData.extremums_x.clear();

            ui->connectButton->setEnabled(false);
            ui->connectButton->repaint();

            ui->disconnectButton->setEnabled(true);
            ui->disconnectButton->repaint();
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


struct graphData{
    QVector<double> ordinate, abscissa, viewOrdinate, viewAbscissa;
    QVector<double> pointOrdinate, pointAbscissa, viewPointOrdinate, viewPointAbscissa;
    double saveTime=0, saveAmplitude=1,saveFrequency=1;
};
QDataStream &operator<<(QDataStream &out, const graphData &saveGraph){
    out << saveGraph.ordinate << saveGraph.abscissa << saveGraph.viewOrdinate << saveGraph.viewAbscissa;
    out << saveGraph.pointOrdinate << saveGraph.pointAbscissa << saveGraph.viewPointOrdinate << saveGraph.viewPointAbscissa;
    out << saveGraph.saveTime << saveGraph.saveAmplitude << saveGraph.saveFrequency;
    return out;
}
QDataStream &operator>>(QDataStream &in, graphData &openGraph){
    in >> openGraph.ordinate >> openGraph.abscissa >> openGraph.viewOrdinate >> openGraph.viewAbscissa;
    in >> openGraph.pointOrdinate >> openGraph.pointAbscissa >> openGraph.viewPointOrdinate >> openGraph.viewPointAbscissa;
    in >> openGraph.saveTime >> openGraph.saveAmplitude >> openGraph.saveFrequency;
    return in;
};
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
        QString saveName = QFileDialog::getSaveFileName(this,tr("Выбор папки"),"/home/untitled.txt",tr("* .txt"));
        QFile saveFile(saveName + ".txt");

        if(!saveFile.open(QIODevice::WriteOnly)){
            pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                                "Ошибка записи!\n",
                                                QMessageBox::Ok);
            int n = pmbx->exec();
            if(n==QMessageBox::Ok){
               delete pmbx;
            }
        }
        else{
            graphData save;
            for(int i=0;i<allData.y.size();++i){
                save.ordinate.push_back(allData.y[i]);
                save.abscissa.push_back(allData.x[i]);
            }
            for(int i=0;i<allData.yview.size();++i){
                save.viewOrdinate.push_back(allData.yview[i]);
                save.viewAbscissa.push_back(allData.xview[i]);
            }
            for(int i=0;i<allData.extremums_y.size();++i){
                save.pointOrdinate.push_back(allData.extremums_y[i]);
                save.pointAbscissa.push_back(allData.extremums_x[i]);
            }
            for(int i=0;i<allData.extremums_yview.size();++i){
                save.viewPointOrdinate.push_back(allData.extremums_yview[i]);
                save.viewPointAbscissa.push_back(allData.extremums_xview[i]);
            }

            save.saveTime = time;
            save.saveAmplitude = amplitude;
            save.saveFrequency = frequency;

            QDataStream out(&saveFile);
               out << save;
            saveFile.close();
        }
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
                   QString fileName = QFileDialog::getOpenFileName(this,tr("Выбор файла"),"/untiled",tr("* .txt"));
                   if(!fileName.isEmpty()){
                       QFile openFile(fileName);

                       if(!openFile.open(QIODevice::ReadOnly)){
                       pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                                           "Ошибка чтения!\n",
                                                           QMessageBox::Ok);
                           int n = pmbx->exec();
                           if(n==QMessageBox::Ok){
                           delete pmbx;
                           }
                       }

                       allData.x.clear();
                       allData.y.clear();
                       allData.xview.clear();
                       allData.yview.clear();
                       allData.extremums_yview.clear();
                       allData.extremums_xview.clear();
                       allData.extremums_y.clear();
                       allData.extremums_x.clear();

                       graphData open;
                       QDataStream in(&openFile);
                           in >> open;
                           openFile.close();

                       for(int i=0;i<open.ordinate.size();++i){
                           allData.y.push_back(open.ordinate[i]);
                           allData.x.push_back(open.abscissa[i]);
                       }
                       for(int i=0;i<open.viewOrdinate.size();++i){
                           allData.yview.push_back(open.viewOrdinate[i]);
                           allData.xview.push_back(open.viewAbscissa[i]);
                       }
                       for(int i=0;i<open.pointOrdinate.size();++i){
                           allData.extremums_y.push_back(open.pointOrdinate[i]);
                           allData.extremums_x.push_back(open.pointAbscissa[i]);
                       }
                       for(int i=0;i<open.viewPointOrdinate.size();++i){
                           allData.extremums_yview.push_back(open.viewPointOrdinate[i]);
                           allData.extremums_xview.push_back(open.viewPointAbscissa[i]);
                       }

                       time = open.saveTime;
                       amplitude = open.saveAmplitude;
                       frequency = open.saveFrequency;

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
            if(n==QMessageBox::Cancel) delete pmbx;
        }
        else {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Выбор файла"),"/untiled",tr("* .txt"));
        if(!fileName.isEmpty()){
            QFile openFile(fileName);

            if(!openFile.open(QIODevice::ReadOnly)){
            pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                                "Ошибка чтения!\n",
                                                QMessageBox::Ok);
                int n = pmbx->exec();
                if(n==QMessageBox::Ok){
                delete pmbx;
                }
            }

            allData.x.clear();
            allData.y.clear();
            allData.xview.clear();
            allData.yview.clear();
            allData.extremums_yview.clear();
            allData.extremums_xview.clear();
            allData.extremums_y.clear();
            allData.extremums_x.clear();

            graphData open;
            QDataStream in(&openFile);
                in >> open;
                openFile.close();

            for(int i=0;i<open.ordinate.size();++i){
                allData.y.push_back(open.ordinate[i]);
                allData.x.push_back(open.abscissa[i]);
            }
            for(int i=0;i<open.viewOrdinate.size();++i){
                allData.yview.push_back(open.viewOrdinate[i]);
                allData.xview.push_back(open.viewAbscissa[i]);
            }
            for(int i=0;i<open.pointOrdinate.size();++i){
                allData.extremums_y.push_back(open.pointOrdinate[i]);
                allData.extremums_x.push_back(open.pointAbscissa[i]);
            }
            for(int i=0;i<open.viewPointOrdinate.size();++i){
                allData.extremums_yview.push_back(open.viewPointOrdinate[i]);
                allData.extremums_xview.push_back(open.viewPointAbscissa[i]);
            }

            time = open.saveTime;
            amplitude = open.saveAmplitude;
            frequency = open.saveFrequency;

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

}


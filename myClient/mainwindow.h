#pragma once
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>
#include "viewsetting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTcpSocket* socket;
    QByteArray Data;
    QVector<double> massiv;
    double xBegin=0, xEnd=0, h=0, X=0, time=0,amplitude=1;
    QVector<double> x,y,xview,yview;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();



    void on_SettingAction_triggered();

private:
    viewSetting setting_window;
    Ui::MainWindow *ui;
};

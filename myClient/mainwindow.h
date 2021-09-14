#pragma once
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>
#include "viewsetting.h"
#include "signaldata.h"

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
    double xBegin=0, xEnd=0, h=0.01, X=0, time=0,amplitude=20,frequency=10;
    int area_limit=5/h;
    QSettings settings;
    SignalData allData;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void readSettings();
    void writeSettings();
    void on_SettingAction_triggered();
    void on_action_exit_triggered();
    void on_action_save_triggered();

    void on_action_open_triggered();

private:
    viewSetting setting_window;
    Ui::MainWindow *ui;
};

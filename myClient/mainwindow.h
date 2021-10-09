#pragma once
#include <QMainWindow>
#include <QShortcut>
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>
#include <QShortcut>
#include "viewsetting.h"
#include "signaldata.h"
#include "dataio.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_SettingAction_triggered();
    void on_action_exit_triggered();
    void on_action_save_triggered();

    void on_action_open_triggered();

protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QByteArray Data;
    QVector<double> dataToServer;
    double xBegin=0, xEnd=0, h=0.01, X=0, time=0,amplitude=20,frequency=10,verticalPlus=0;
    int area_limit=5/h;
    Settings settings;
    SignalData allData;
    DataIO fileWork;
    QShortcut       *keyCtrlPlus,*keyCtrlMinus;
};

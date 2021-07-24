#pragma once
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>

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
    double xBegin=0, xEnd=0, h=0, X=0, time=-1,amplitude=1;
    QVector<double> x,y,xview,yview;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();


private:
    Ui::MainWindow *ui;
};

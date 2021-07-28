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
    QVector<double> massiv;
    double xBegin=0, xEnd=0, h=0, X=0, time=0,amplitude=1;
    int r=0,g=0,b=0,size=1;
    QVector<double> x,y,xview,yview;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_green_colout_triggered();

    void on_black_colour_triggered();

    void on_blue_colour_triggered();

    void on_red_colour_triggered();

    void on_size_1_triggered();

    void on_size_2_triggered();

    void on_size_3_triggered();

    void on_size_4_triggered();

    void on_size_5_triggered();

private:
    Ui::MainWindow *ui;
};

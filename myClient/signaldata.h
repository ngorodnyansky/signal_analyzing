#pragma once
#include <QVector>

class SignalData
{
private:
    double h=0.01;
    int area_limit=5/h;
    QVector <double> x,y,xview, yview, extremums_x, extremums_y, extremums_xview, extremums_yview;
public:
    void add(double abscissa, double ordiate);
    double getAbscissa(int n);
    double getOrdinate(int n);
    double getAbscissaView(int n);
    double getOrdinateView(int n);
    double getExtremumAbscissa(int n);
    double getExtremumOrdinate(int n);
    double getExtremumAbscissaView(int n);
    double getExtremumOrdinateView(int n);
    int getAbscissaSize();
    int getAbscissaViewSize();
    int getExtremumAbscissaSize();
    double getExtremumAbscissaViewSize();
    void copyAbscissa(QVector <double>& newCopy);
    void copyOrdinate(QVector <double>& newCopy);
    void copyAbscissaView(QVector <double>& newCopy);
    void copyOrdinateView(QVector <double>& newCopy);
    void copyExtremumAbscissa(QVector <double>& newCopy);
    void copyExtremumOrdinate(QVector <double>& newCopy);
    void copyExtremumAbscissaView(QVector <double>& newCopy);
    void copyExtremumOrdinateView(QVector <double>& newCopy);
    SignalData();
    void clear();
};


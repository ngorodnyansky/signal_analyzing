#pragma once
#include <QVector>


class SignalData
{
public:
    double xBegin=0, xEnd=0, h=0.01, X=0, time=0,amplitude=20,frequency=10;
    int area_limit=5/h;
    QVector <double> x,y,xview, yview, extremums_x, extremums_y, extremums_xview, extremums_yview;
    void add(double abscissa, double ordiate);
    SignalData();
};


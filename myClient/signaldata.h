#pragma once
#include <QVector>


class SignalData
{
public:
    double h=0.01;
    int area_limit=5/h;
    QVector <double> x,y,xview, yview, extremums_x, extremums_y, extremums_xview, extremums_yview;
    void add(double abscissa, double ordiate);
    SignalData();
};


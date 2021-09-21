#pragma once
#include <QVector>

class SignalData
{
public:
    void add(double abscissa, double ordiate);
    double getAbscissa(int n) const;
    double getOrdinate(int n) const;
    double getAbscissaView(int n) const;
    double getOrdinateView(int n) const;
    double getExtremumAbscissa(int n) const;
    double getExtremumOrdinate(int n) const;
    double getExtremumAbscissaView(int n) const;
    double getExtremumOrdinateView(int n) const;
    int getAbscissaSize() const;
    int getAbscissaViewSize() const;
    int getExtremumAbscissaSize() const;
    double getExtremumAbscissaViewSize() const;
    QVector <double> getAbscisa() const;
    QVector <double> getOrdinate() const;
    QVector <double> getAbscissaView() const;
    QVector <double> getOrdinateView() const;
    QVector <double> getExtremumAbscissa() const;
    QVector <double> getExtremumOrdinate() const;
    QVector <double> getExtremumAbscissaView() const;
    QVector <double> getExtremumOrdinateView() const;
    void clear();
private:
    double h=0.01;
    int area_limit=5/h;
    QVector <double> m_x,m_y,m_xview, m_yview, m_extremums_x, m_extremums_y, m_extremums_xview, m_extremums_yview;
};


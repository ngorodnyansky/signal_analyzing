#pragma once
#include <QVector>
#include <QDataStream>
#include <QMessageBox>
#include <QFileDialog>
#include "signaldata.h"


class DataIO
{
private:
    QVector<double> ordinate, abscissa, viewOrdinate, viewAbscissa;
    QVector<double> pointOrdinate, pointAbscissa, viewPointOrdinate, viewPointAbscissa;
public:
    friend QDataStream &operator>>(QDataStream &in, DataIO &openGraph);
    DataIO();
    void save(SignalData& data);
    void open(SignalData& data);
};



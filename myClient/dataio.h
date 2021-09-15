#pragma once
#include <QVector>
#include <QDataStream>
#include <QMessageBox>
#include <QFileDialog>
#include "signaldata.h"


class DataIO
{
public:
    QVector<double> ordinate, abscissa, viewOrdinate, viewAbscissa;
    QVector<double> pointOrdinate, pointAbscissa, viewPointOrdinate, viewPointAbscissa;
    DataIO();
    void save(const SignalData& data);
    void open(SignalData& data);
};



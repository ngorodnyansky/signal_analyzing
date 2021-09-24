#pragma once
#include <QVector>
#include <QDataStream>
#include <QMessageBox>
#include <QFileDialog>
#include "signaldata.h"


class DataIO
{
public:
    friend QDataStream &operator>>(QDataStream &in, DataIO &openGraph);
    void save(SignalData& data);
    void open(SignalData& data);

private:
    QVector<double> m_ordinate, m_abscissa, m_viewOrdinate, m_viewAbscissa;
    QVector<double> m_pointOrdinate, m_pointAbscissa, m_viewPointOrdinate, m_viewPointAbscissa;
};



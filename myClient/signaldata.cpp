#include "signaldata.h"

void SignalData::add(double abscissa, double ordiate){
    m_y.push_back(ordiate);
    m_x.push_back(abscissa);
    if(abscissa*100<=area_limit){
        m_xview.push_back(abscissa);
        m_yview.push_back(ordiate);
    }
    else{
        for(int i=0;i<area_limit;++i){
            m_xview.swapItemsAt(i,i+1);
            m_yview.swapItemsAt(i,i+1);
        }
        int k=m_xview.size();
        m_xview[k-1]=abscissa;
        m_yview[k-1]=ordiate;
    }
    int n = m_y.size();
    if(n>=3){
        if(m_y[n-2]!=m_y[n-1])
        {
            if(m_variabl_Sign==0)
            {
                if(((m_y[n-3]-m_y[n-2])>0 && (m_y[n-2]-m_y[n-1])<0) || ((m_y[n-3]-m_y[n-2])<0 && (m_y[n-2]-m_y[n-1])>0))
                {
                    m_extremums_x.push_back(abscissa-0.01);
                    m_extremums_y.push_back(m_y[n-2]);
                    m_extremums_xview.push_back(abscissa-0.01);
                    m_extremums_yview.push_back(m_y[n-2]);
                    if(m_extremums_xview.size()==area_limit)
                    {
                        for(int i=0;i<area_limit;++i)
                        {
                            m_extremums_xview.swapItemsAt(i,i+1);
                            m_extremums_yview.swapItemsAt(i,i+1);
                        }
                        int j = m_extremums_xview.size();
                        m_extremums_xview[j-1]=abscissa-0.01;
                        m_extremums_yview[j-1]=m_y[n-2];
                    }
                }
            }
            else
            {
                if((m_variabl_Sign>0 && (m_y[n-2]-m_y[n-1])<0) || (m_variabl_Sign<0 && (m_y[n-2]-m_y[n-1])>0))
                {
                    m_extremums_x.push_back(abscissa-0.01);
                    m_extremums_y.push_back(m_y[n-2]);
                    m_extremums_xview.push_back(abscissa-0.01);
                    m_extremums_yview.push_back(m_y[n-2]);
                    if(m_extremums_xview.size()==area_limit)
                    {
                        for(int i=0;i<area_limit;++i)
                        {
                            m_extremums_xview.swapItemsAt(i,i+1);
                            m_extremums_yview.swapItemsAt(i,i+1);
                        }
                        int j = m_extremums_xview.size();
                        m_extremums_xview[j-1]=abscissa-0.01;
                        m_extremums_yview[j-1]=m_y[n-2];
                    }
                }
            }
            m_variabl_Sign=0;
        }
        else
        {
            if(m_variabl_Sign==0)
            {
                m_variabl_Sign=m_y[n-3]-m_y[n-2];
            }
            /*
            if((m_y[n-2]-m_y[n-1])==0){
                m_extremums_x.push_back(abscissa-0.01);
                m_extremums_y.push_back(m_y[n-2]);
                m_extremums_xview.push_back(abscissa-0.01);
                m_extremums_yview.push_back(m_y[n-2]);
                if(m_extremums_xview.size()==area_limit){
                    for(int i=0;i<area_limit;++i){
                        m_extremums_xview.swapItemsAt(i,i+1);
                        m_extremums_yview.swapItemsAt(i,i+1);
                    }
                    int j = m_extremums_xview.size();
                    m_extremums_xview[j-1]=abscissa-0.01;
                    m_extremums_yview[j-1]=m_y[n-2];
                }
            }
            */
        }
    }
}

void SignalData::clear()
{
    m_x.clear();
    m_y.clear();
    m_xview.clear();
    m_yview.clear();
    m_extremums_yview.clear();
    m_extremums_xview.clear();
    m_extremums_y.clear();
    m_extremums_x.clear();
}

double SignalData::getAbscissa(int n) const
{
    return m_x[n];
}
double SignalData::getOrdinate(int n) const
{
    return m_y[n];
}
double SignalData::getAbscissaView(int n) const
{
    return m_xview[n];
}
double SignalData::getOrdinateView(int n) const
{
    return m_yview[n];
}
double SignalData::getExtremumAbscissa(int n) const
{
    return m_extremums_x[n];
}
double SignalData::getExtremumOrdinate(int n) const
{
    return m_extremums_y[n];
}
double SignalData::getExtremumAbscissaView(int n) const
{
    return m_extremums_xview[n];
}
double SignalData::getExtremumOrdinateView(int n) const
{
    return m_extremums_yview[n];
}

int SignalData::getAbscissaSize() const
{
    return m_x.size();
}
int SignalData::getAbscissaViewSize() const
{
    return m_xview.size();
}
int SignalData::getExtremumAbscissaSize() const
{
    return m_extremums_x.size();
}
double SignalData::getExtremumAbscissaViewSize() const
{
    return m_extremums_xview.size();
}

QVector<double> SignalData::getOrdinate() const
{
    return m_y;
}

QVector<double> SignalData::getAbscissaView() const
{
    return m_xview;
}

QVector<double> SignalData::getOrdinateView() const
{
    return m_yview;
}

QVector<double> SignalData::getExtremumAbscissa() const
{
    return m_extremums_x;
}

QVector<double> SignalData::getExtremumOrdinate() const
{
    return m_extremums_y;
}

QVector<double> SignalData::getExtremumAbscissaView() const
{
    return m_extremums_xview;
}

QVector<double> SignalData::getExtremumOrdinateView() const
{
    return m_extremums_yview;
}

QVector<double> SignalData::getAbscisa() const
{
    return m_x;
}

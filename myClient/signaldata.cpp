#include "signaldata.h"

SignalData::SignalData()
{

}

void SignalData::add(double abscissa, double ordiate){
    y.push_back(ordiate);
    x.push_back(abscissa);
    if(abscissa*100<=area_limit){
        xview.push_back(abscissa);
        yview.push_back(ordiate);
    }
    else{
        for(int i=0;i<area_limit;++i){
            xview.swapItemsAt(i,i+1);
            yview.swapItemsAt(i,i+1);
        }
        int k=xview.size();
        xview[k-1]=abscissa;
        yview[k-1]=ordiate;
    }
    int n = y.size();
    if(n>=3){
        if(((y[n-3]-y[n-2])/h>0 && (y[n-2]-y[n-1])/h<0) || ((y[n-3]-y[n-2])/h<0 && (y[n-2]-y[n-1])/h>0)){
            extremums_x.push_back(abscissa-0.01);
            extremums_y.push_back(y[n-2]);
            extremums_xview.push_back(abscissa-0.01);
            extremums_yview.push_back(y[n-2]);
            if(extremums_xview.size()==area_limit){
                for(int i=0;i<area_limit;++i){
                   extremums_xview.swapItemsAt(i,i+1);
                    extremums_yview.swapItemsAt(i,i+1);
                }
                int j = extremums_xview.size();
                extremums_xview[j-1]=abscissa-0.01;
                extremums_yview[j-1]=y[n-2];
            }
        }
    }
}

void SignalData::clear()
{
    x.clear();
    y.clear();
    xview.clear();
    yview.clear();
    extremums_yview.clear();
    extremums_xview.clear();
    extremums_y.clear();
    extremums_x.clear();
}

double SignalData::getAbscissa(int n)
{
    return x[n];
}
double SignalData::getOrdinate(int n)
{
    return y[n];
}
double SignalData::getAbscissaView(int n)
{
    return xview[n];
}
double SignalData::getOrdinateView(int n)
{
    return yview[n];
}
double SignalData::getExtremumAbscissa(int n)
{
    return extremums_x[n];
}
double SignalData::getExtremumOrdinate(int n)
{
    return extremums_y[n];
}
double SignalData::getExtremumAbscissaView(int n)
{
    return extremums_xview[n];
}
double SignalData::getExtremumOrdinateView(int n)
{
    return extremums_yview[n];
}

int SignalData::getAbscissaSize()
{
    return x.size();
}
int SignalData::getAbscissaViewSize()
{
    return xview.size();
}
int SignalData::getExtremumAbscissaSize()
{
    return extremums_x.size();
}
double SignalData::getExtremumAbscissaViewSize()
{
    return extremums_xview.size();
}

void SignalData::copyAbscissa(QVector <double>& newCopy){
    newCopy.clear();
    for(int i=0;i<x.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyOrdinate(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<y.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyAbscissaView(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<xview.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyOrdinateView(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<yview.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyExtremumAbscissa(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<extremums_x.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyExtremumOrdinate(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<extremums_y.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyExtremumAbscissaView(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<extremums_xview.size();i++)
        newCopy.push_back(x[i]);
}
void SignalData::copyExtremumOrdinateView(QVector <double>& newCopy)
{
    newCopy.clear();
    for(int i=0;i<extremums_yview.size();i++)
        newCopy.push_back(x[i]);
}

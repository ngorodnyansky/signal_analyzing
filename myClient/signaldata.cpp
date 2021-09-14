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

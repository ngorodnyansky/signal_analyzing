#include "dataio.h"

DataIO::DataIO()
{

}

void DataIO::save(const SignalData& data)
{   QMessageBox *pmbx;
    QString saveName = QFileDialog::getSaveFileName(0, "Выбор папки","/home/untitled.txt","* .txt");
    QFile saveFile(saveName + ".txt");

    if(!saveFile.open(QIODevice::WriteOnly)){
        pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                            "Ошибка записи!\n",
                                            QMessageBox::Ok);
        int n = pmbx->exec();
        if(n==QMessageBox::Ok){
           delete pmbx;
        }
    }
    else{
        for(int i=0;i<data.y.size();++i){
            this->ordinate.push_back(data.y[i]);
            this->abscissa.push_back(data.x[i]);
        }
        for(int i=0;i<data.yview.size();++i){
            this->viewOrdinate.push_back(data.yview[i]);
            this->viewAbscissa.push_back(data.xview[i]);
        }
        for(int i=0;i<data.extremums_y.size();++i){
            this->pointOrdinate.push_back(data.extremums_y[i]);
            this->pointAbscissa.push_back(data.extremums_x[i]);
        }
        for(int i=0;i<data.extremums_yview.size();++i){
            this->viewPointOrdinate.push_back(data.extremums_yview[i]);
            this->viewPointAbscissa.push_back(data.extremums_xview[i]);
        }

        QDataStream out(&saveFile);
           out << this->ordinate << this->abscissa << this->viewOrdinate << this->viewAbscissa;
           out << this->pointOrdinate << this->pointAbscissa << this->viewPointOrdinate << this->viewPointAbscissa;
        saveFile.close();
    }
}

QDataStream &operator>>(QDataStream &in, DataIO &openGraph){
    in >> openGraph.ordinate >> openGraph.abscissa >> openGraph.viewOrdinate >> openGraph.viewAbscissa;
    in >> openGraph.pointOrdinate >> openGraph.pointAbscissa >> openGraph.viewPointOrdinate >> openGraph.viewPointAbscissa;
    return in;
}

void DataIO::open(SignalData &data)
{
    QMessageBox *pmbx;
    QString fileName = QFileDialog::getOpenFileName(0,"Выбор файла","/untiled","* .txt");
    if(!fileName.isEmpty()){
        QFile openFile(fileName);

        if(!openFile.open(QIODevice::ReadOnly)){
        pmbx = new QMessageBox(QMessageBox::Question,"Ошибка",
                                            "Ошибка чтения!\n",
                                            QMessageBox::Ok);
            int n = pmbx->exec();
            if(n==QMessageBox::Ok){
            delete pmbx;
            }
        }

        data.x.clear();
        data.y.clear();
        data.xview.clear();
        data.yview.clear();
        data.extremums_yview.clear();
        data.extremums_xview.clear();
        data.extremums_y.clear();
        data.extremums_x.clear();
        DataIO open;
        QDataStream in(&openFile);
            in >> open;
            openFile.close();

        for(int i=0;i<open.ordinate.size();++i){
            data.y.push_back(open.ordinate[i]);
            data.x.push_back(open.abscissa[i]);
        }
        for(int i=0;i<open.viewOrdinate.size();++i){
            data.yview.push_back(open.viewOrdinate[i]);
            data.xview.push_back(open.viewAbscissa[i]);
        }
        for(int i=0;i<open.pointOrdinate.size();++i){
            data.extremums_y.push_back(open.pointOrdinate[i]);
            data.extremums_x.push_back(open.pointAbscissa[i]);
        }
        for(int i=0;i<open.viewPointOrdinate.size();++i){
            data.extremums_yview.push_back(open.viewPointOrdinate[i]);
            data.extremums_xview.push_back(open.viewPointAbscissa[i]);
        }
    }
}


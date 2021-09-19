#include "dataio.h"

DataIO::DataIO()
{

}

void DataIO::save(SignalData& data)
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
        for(int i=0;i<data.getAbscissaSize();++i){
            this->ordinate.push_back(data.getOrdinate(i));
            this->abscissa.push_back(data.getAbscissa(i));
        }
        for(int i=0;i<data.getAbscissaViewSize();++i){
            this->viewOrdinate.push_back(data.getOrdinateView(i));
            this->viewAbscissa.push_back(data.getAbscissaView(i));
        }
        for(int i=0;i<data.getExtremumAbscissaSize();++i){
            this->pointOrdinate.push_back(data.getExtremumOrdinate(i));
            this->pointAbscissa.push_back(data.getExtremumAbscissa(i));
        }
        for(int i=0;i<data.getExtremumAbscissaViewSize();++i){
            this->viewPointOrdinate.push_back(data.getExtremumOrdinateView(i));
            this->viewPointAbscissa.push_back(data.getExtremumAbscissaView(i));
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

        data.clear();
        DataIO open;
        QDataStream in(&openFile);
            in >> open;
            openFile.close();

        for(int i=0;i<open.ordinate.size();++i){
            data.add(open.abscissa[i],open.ordinate[i]);
        }
    }
}


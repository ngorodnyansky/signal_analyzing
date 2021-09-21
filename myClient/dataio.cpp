#include "dataio.h"

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
            m_ordinate.push_back(data.getOrdinate(i));
            m_abscissa.push_back(data.getAbscissa(i));
        }
        for(int i=0;i<data.getAbscissaViewSize();++i){
            m_viewOrdinate.push_back(data.getOrdinateView(i));
            m_viewAbscissa.push_back(data.getAbscissaView(i));
        }
        for(int i=0;i<data.getExtremumAbscissaSize();++i){
            m_pointOrdinate.push_back(data.getExtremumOrdinate(i));
            m_pointAbscissa.push_back(data.getExtremumAbscissa(i));
        }
        for(int i=0;i<data.getExtremumAbscissaViewSize();++i){
            m_viewPointOrdinate.push_back(data.getExtremumOrdinateView(i));
            m_viewPointAbscissa.push_back(data.getExtremumAbscissaView(i));
        }

        QDataStream out(&saveFile);
           out << m_ordinate << m_abscissa << m_viewOrdinate << m_viewAbscissa;
           out << m_pointOrdinate << m_pointAbscissa << m_viewPointOrdinate << m_viewPointAbscissa;
        saveFile.close();
    }
}

QDataStream &operator>>(QDataStream &in, DataIO &openGraph){
    in >> openGraph.m_ordinate >> openGraph.m_abscissa >> openGraph.m_viewOrdinate >> openGraph.m_viewAbscissa;
    in >> openGraph.m_pointOrdinate >> openGraph.m_pointAbscissa >> openGraph.m_viewPointOrdinate >> openGraph.m_viewPointAbscissa;
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

        for(int i=0;i<open.m_ordinate.size();++i){
            data.add(open.m_abscissa[i],open.m_ordinate[i]);
        }
    }
}


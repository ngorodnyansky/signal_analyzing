#include "translatorthread.h"
#include <QFile>

TranslatorThread::TranslatorThread(QString fileName,qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;

    QFile openFile(fileName);

    if(!openFile.open(QIODevice::ReadOnly))
        qDebug() << "Read error";

    translatorData.clear();
    QDataStream in(&openFile);
        in >> translatorData;
        openFile.close();
}

void TranslatorThread::run()
{
    qDebug() << " Thread started";

    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << translatorData.size();

    qDebug() << socketDescriptor << " Client connected";

    exec();
}

void TranslatorThread::readyRead()
{

    double data=translatorData[i];
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if(connected){
    socket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
    socket->waitForBytesWritten();
    QThread::currentThread()->usleep(1000);
    }
    i++;
    if(i==translatorData.size()-1)
        i=0;
}

void TranslatorThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    exit(0);
}


#include "thread.h"

Thread::Thread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void Thread::run()
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


    qDebug() << socketDescriptor << " Client connected";

    exec();
}

void Thread::readyRead()
{
    QVector<double> clientData;
    QDataStream in(socket);
    in >> clientData;
    amplitude=clientData[1]*0.1;
    frequency=clientData[2];
    double xBegin=clientData[0], X=xBegin, data=0;
    if(X==6.28){
        X=0;
    }
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if(connected){
    data=amplitude*sin(X*0.2*3.14159265*frequency);
    socket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
    socket->waitForBytesWritten();
    QThread::currentThread()->usleep(50000);
    }
}

void Thread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

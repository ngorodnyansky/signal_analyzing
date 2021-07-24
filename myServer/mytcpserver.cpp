#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}
void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

}

void MyTcpServer::slotServerRead()
{
    QByteArray array;
    array = mTcpSocket->readAll();
    double abscissa = QVariant(array).toDouble();
    //qDebug() << abscissa;

    if(abscissa==-1){
        double xBegin=0, h=0.1, X=xBegin, data=0;
        while(X<=6.3){
            bool connected = (mTcpSocket->state() == QTcpSocket::ConnectedState);
            if(connected){
                data=amplitude*sin(X);
                mTcpSocket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
                mTcpSocket->waitForBytesWritten();
                qDebug() << data;
                QThread::currentThread()->usleep(speed);
                if(X>=6.2){
                  X=0;
                }
                X+=h;
            }
            else
                break;
        }
    }
    else{

        double xBegin=abscissa, h=0.1, X=xBegin, data=0;
        while(X>6.3){
            X-=6.3;
        }
        while(X<=6.3){
            bool connected = (mTcpSocket->state() == QTcpSocket::ConnectedState);
            if(connected){
                data=amplitude*sin(X);
                mTcpSocket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
                mTcpSocket->waitForBytesWritten();
                qDebug() << data;
                QThread::currentThread()->usleep(speed);
                if(X>=6.2){
                  X=0;
                }
                X+=h;
            }
            else
                break;
        }
    }
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

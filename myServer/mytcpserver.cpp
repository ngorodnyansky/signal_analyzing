#include "mytcpserver.h"
#include <QDebug>
#include "thread.h"
#include <QFile>

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent)
{
}

MyTcpServer::MyTcpServer(QString fileName)
{
    qDebug()<<fileName;

}

void MyTcpServer::startServer()
{
    int port = 6000;

    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    Thread *thread = new Thread(socketDescriptor, this);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}










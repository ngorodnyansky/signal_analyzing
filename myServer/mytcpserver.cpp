#include "mytcpserver.h"
#include <QDebug>
#include "thread.h"

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent)
{
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

    // Every new connection will be run in a newly created thread
    Thread *thread = new Thread(socketDescriptor, this);

    // connect signal/slot
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}










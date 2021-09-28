#include "mytcpserver.h"
#include <QDebug>
#include "thread.h"
#include <QFile>


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





void GenerationServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    Thread *thread = new Thread(socketDescriptor, this);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

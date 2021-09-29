#include "mytcpserver.h"
#include <QDebug>
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

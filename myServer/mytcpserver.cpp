#include "mytcpserver.h"
#include <QDebug>
#include "generatorthread.h"
#include "translatorthread.h"


MyTcpServer::MyTcpServer(const QString& dataFileName)
    :m_dataFileName(dataFileName)
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

    QThread* thread = Q_NULLPTR;
    if(!m_dataFileName.isEmpty())
        thread = new TranslatorThread(m_dataFileName, socketDescriptor, this);
    else
        thread = new GeneratorThread(socketDescriptor, this);

    thread->start();

}







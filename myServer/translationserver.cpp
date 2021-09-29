#include "translationserver.h"

TranslationServer::TranslationServer(QString filename)
{
    qDebug()<<filename;
}

void TranslationServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << socketDescriptor;

}

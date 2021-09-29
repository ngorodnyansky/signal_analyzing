#include "generationserver.h"


void GenerationServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    emit generateGraph.generate(socketDescriptor);

    qDebug() << socketDescriptor << " Client connected";
}

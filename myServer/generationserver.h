#pragma once
#include "mytcpserver.h"

class GenerationServer : public MyTcpServer
{
private:
    void incomingConnection(qintptr socketDescriptor);
    Controller generateGraph;
};

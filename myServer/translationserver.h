#pragma once
#include "mytcpserver.h"


class TranslationServer: public MyTcpServer
{
public:
    TranslationServer(QString filename);
private:
    void incomingConnection(qintptr socketDescriptor);
};


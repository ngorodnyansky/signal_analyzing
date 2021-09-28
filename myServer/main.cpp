#include <QCoreApplication>
#include "mytcpserver.h"
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Sygnal analyzing");
    QCoreApplication::setApplicationVersion("0.8");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();


    parser.addPositionalArgument("filename", "The name of the file to be streamed");

    parser.parse(QCoreApplication::arguments());

    const QStringList args = parser.positionalArguments();
    const QString filename = args.isEmpty() ? QString() : args.first();
    parser.process(a);
    if (filename.isNull())
    {
        MyTcpServer server;
        server.startServer();
        return a.exec();
    }
    else
    {
        MyTcpServer server(filename);
        server.startServer();
        return a.exec();
    }
}

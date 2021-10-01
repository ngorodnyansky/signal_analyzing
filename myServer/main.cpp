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

    parser.process(a);

    const QStringList args = parser.positionalArguments();
    const QString filename = args.isEmpty() ? QString() : args.first();

    MyTcpServer server(filename);
    server.startServer();
    return a.exec();

}

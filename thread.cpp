//-----------------Thread : created when incomming connection of server is called-----------//

#include "thread.h"
#include <QRegularExpression>

Thread::Thread(int ID , QObject *parent) : QThread(parent)
{

    //Thread constructor.
    // Set the socket descriptor id
    this->socketdescriptor = ID;
}

void Thread::run()
{
    //Aim : We will run this to start our thread. All inside " " are the signals/slots
    //Step 1: Create a socket
    //Step 2: if success good else give error
    //Step 3: connect socket "readyread" to this class "readyread"
    //Step 4: connect "disconnected" of socket to "disconnected" of this object
    //exec for keeping the thread open
    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketdescriptor))
    {
        emit error(socket->error());
        emit status(socket->errorString());
        return;
    }

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyread()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketdescriptor << ":Client Connected";
    emit status("Client connected.");
    exec();
}

//On Client Disconnect - When the client disconnected emit status as : Disconnected

void Thread::disconnected()
{
    socket->deleteLater();
    emit status("Disconnected");
    exit(0);
}

void Thread::readyread()
{
    // Aim: Read data from socket and update the status bar
    // Step 1: Read all
    // Step 2: Send the data
    // Step 3: Send status to status bar

    QByteArray Data = socket->readAll();
    QRegularExpression re("[0-9]\\d{0,3},[0-9]\\d{0,3},\\S+,\\S+,-[0-9]\\d{0,3}");
    QRegularExpressionMatch match2 = re.match(Data);
    if(match2.hasMatch())
    {
    emit info(Data);
    emit status("Recieved data :" + Data);
    }
    else {
        emit status("Recieved wrong data format :" + Data);
    }
}

void Thread::writeconfig(QByteArray str)
{
    // Aim: Write to socket the updated configuration
    socket->write(str);
}

//This is server code that listens at port 1234//

#include "myserver.h"
#include "thread.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{

}

void MyServer::StartServer()
{
    // Change: 1234 to your required port
    if(!this->listen(QHostAddress::Any,1234))
    {

        emit statusinf("Server could not be started. Please relaunch the app.");
    }
    else {

        emit statusinf("Listening on port 1234.");
    }


}

void MyServer::info(QString str)
{
    emit sendinf(str);
}

//Thread is created on incomming connection//

void MyServer::incomingConnection(qintptr socketDescriptor)
{
   //Step 1: Create a new thread pointer and send the socket descriptor to it

   //Step 2:
   //connect 1: for deleting later
   //connect 2: for sending info
   //connect 3: for sending status info and updating it , connected etc
   //connect 4: for sending the updated configuration file

   //Step 3 : start the thread

                    Thread *thread = new Thread(socketDescriptor,this);
                    connect(thread,SIGNAL(finished()), thread, SLOT(deleteLater()));
                    connect(thread,&Thread::info, this, &MyServer::info);
                    connect(thread,&Thread::status, this, &MyServer::statusinf);
                    connect(this,&MyServer::sockwritesign,thread, &Thread::writeconfig);
                    thread->start();


}



#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>


class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void StartServer();

signals:
    void sendinf(QString str);
    void statusinf(QString str);
    void sockwritesign(QByteArray str);


public slots:
    void info(QString str);
    void status(QString str)
    {
        emit statusinf(str);
    }
    void sockwrite(QByteArray str)
    {
        emit sockwritesign(str);
    }


protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H

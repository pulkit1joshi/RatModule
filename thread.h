#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int ID, QObject *parent = nullptr);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void info(QString str);
    void status(QString str);

public slots:
    void disconnected();
    void readyread();
    void writeconfig(QByteArray str);

private:
    QTcpSocket *socket;
    int socketdescriptor;


};

#endif // THREAD_H

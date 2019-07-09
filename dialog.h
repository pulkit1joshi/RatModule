#ifndef DIALOG_H
#define DIALOG_H
#include <QDataStream>
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include "cmd.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QDialog>






namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int *num,QWidget *parent = nullptr);
    ~Dialog();

signals:
    void refnow(cmd obj);

private slots:
    void on_Save_accepted();

    void on_pushButton_clicked();

    void setstring(QString str);

    
private:
    Ui::Dialog *ui;
    int *numb;
};

#endif // DIALOG_H

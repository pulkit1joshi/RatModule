// THis class is mainly fo configuring the file //

#include "config.h"
#include "ui_config.h"

config::config(QString path , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);

    fpath = path;

    QFile inputFile(path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QString text = in.readAll();
       ui->plainTextEdit->setPlainText(text);
       inputFile.close();
    }
}

config::~config()
{
    delete ui;
}

// On clicking save this is executed
// Output to the file aswell as the socket

void config::on_buttonBox_accepted()
{
    QString path = fpath;

    QFile outputFile(path);
    if (outputFile.open(QIODevice::WriteOnly | QFile::Text))
    {
       QTextStream out(&outputFile);
       out << ui->plainTextEdit->toPlainText();
       outputFile.flush();
       outputFile.close();
    }

    QByteArray str =  ui->plainTextEdit->toPlainText().toUtf8();

    emit sockwrite(str);

}

// On clickiing the config file. The previous value must be shown

void config::created()
{
    QString path = fpath;
    QFile inputFile(path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QString text = in.readAll();
       ui->plainTextEdit->setPlainText(text);
       inputFile.close();
    }

}

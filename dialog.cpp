//This class deals with commands added for SDN Controller

#include "dialog.h"
#include "ui_dialog.h"
#include "cmd.h"
#include "fields.h"
#include <QMessageBox>

//----------Read defined--------------------//
//Reads on the bases of string name//

cmd read(QString name)
{

    cmd obj;
    QFile inputFile("/home/pulkit/Desktop/cmd.txt"); // Here the commands are saved

    if (inputFile.open(QIODevice::ReadOnly)) // Read the cmd.txt file
        {
           QDataStream in(&inputFile);
           while (!in.atEnd())
               {
                   in >> obj; // obj stores the read cmd.
                   int x = QString::compare(obj.name, name, Qt::CaseInsensitive);
                   if(!x) // If name of the obj read is same as that of the name we return the object
                   {
                       inputFile.close();
                       return obj;
                   }
               }
           inputFile.close();

        }

    cmd obj2;
    return obj2;
}


// Write the cmd object to the file

void write(cmd object)
{
    QFile inputFile("/home/pulkit/Desktop/cmd.txt");
    if (inputFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
       QDataStream out(&inputFile);
       out << object;
       inputFile.close();
    }
}

// Constructor of the dailog (present class)

Dialog::Dialog(int *num ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    numb = new int();
    numb = num;
}

Dialog::~Dialog()
{
    delete ui;
}

// Saving the command

void Dialog::on_Save_accepted()
{
    cmd obj=read(ui->lineEdit->text());
    int x = QString::compare(obj.name, ui->lineEdit->text(), Qt::CaseInsensitive);
    if(x)
    {
        *numb=*numb+1;
        obj.num = *numb;

        if(!ui->lineEdit->text().length())
            QMessageBox::information(this,"Error","Enter command name");
        else if(!ui->lineEdit_2->text().length())
            QMessageBox::information(this,"Error","Enter command string");
        else
        {
            obj.cmdstr = ui->lineEdit_2->text();
            obj.name = ui->lineEdit->text();
            if(ui->comboBox->currentText() == "Post")
            {obj.type = 1;}
            else if(ui->comboBox->currentText() == "Get")
            {obj.type = 2;}
            write(obj);
            emit refnow(obj);
        }

    }
    else
    {
        QMessageBox::information(this,"Error","This command already exists");
    }
}

// Display the fields dialog that opens when fields is clicked

void Dialog::on_pushButton_clicked()
{
    fields *f = new fields(this);
    connect(f,&fields::post,this,&Dialog::setstring);
    f->setModal(true);
    f->exec();
}

// Show the fields with added values in the string field of the cmd

void Dialog::setstring(QString str)
{
    ui->lineEdit_2->setText(str);
}











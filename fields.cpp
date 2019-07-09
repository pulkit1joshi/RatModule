//------------------------- FIELDS IN THE SDN CONTROLLER--------------------------//
//------------------THIS IS DISPLAYED WHEN USER CLICKES THE FIELDS---------------//
//----------------===============================================----------------//

#include "fields.h"
#include "ui_fields.h"
#include <stdio.h>

fields::fields(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fields)
{
    ui->setupUi(this);
}

fields::~fields()
{
    delete ui;
}

void fields::on_buttonBox_accepted()
{
    //Append all the fields into one string for making it  ready for POST//
    QString str;
    str.append('"' + ui->label->text() + '"' + ':' + '"' + ui->lineEdit->text() + '"' + ',');
    str.append('"' + ui->label_2->text() + '"' +':' + '"' + ui->lineEdit_2->text() + '"' + ',');
    str.append('"' + ui->label_3->text() + '"' + ':' + '"' + ui->lineEdit_3->text() + '"'+ ',');
    str.append('"' + ui->label_4->text() + '"' + ':' + '"' + ui->lineEdit_4->text() + '"'+ ',');
    str.append('"' + ui->label_5->text() + '"' + ':' + '"' + ui->lineEdit_5->text() + '"' +',');
    str.append('"' + ui->label_6->text() + '"' + ':' + '"' +  ui->lineEdit_6->currentText()+"="+ui->value->text() + '"' +',');
    //str.append('"' + ui->label_7->text() + '"' + ':' + '"' + ui->lineEdit_7->text() + '"' +',');
  //  str.append('"' + ui->label_8->text() + '"' + ':' + '"' + ui->lineEdit_8->text() + '"' +',');
    //str.append('"' + ui->label_9->text() + '"' + ':' + '"' + ui->lineEdit_9->text() + '"');

    qDebug() << "Check:" << str;

    emit post('{' + str +'}');
}

//-------------- Setting dialog box -------------//

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QString hostname , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    // This is contructor for the dialog box
    ui->setupUi(this);
    ui->snrpath->setText(snrp);
    ui->appath->setText(app);
    ui->configpath->setText(configp);
    ui->qpath->setText(qp);
    ui->hostnamebox->setText(hostname);
}

Settings::~Settings()
{
    delete ui;
}

// Emit the changed fields

void Settings::on_hostnamebox_textEdited(const QString &arg1)
{
    emit hostnamechanged(arg1);
}


// Emit the changed fields


/*void Settings::on_checkBox_toggled(bool checked)
{
    emit headerchanged(checked);
}


// Emit the changed fields

void Settings::on_checkBox_2_toggled(bool checked)
{
    emit bodychanged(checked);
}*/


// Emit the changed fields


void Settings::on_graphnamebox_textEdited(const QString &arg1)
{
    emit graphboxchanged(arg1);
}

// Emit the changed fields


void Settings::on_datanamebox_textEdited(const QString &arg1)
{
    emit datanameboxchanged(arg1);
}

// Emit the changed fields


void Settings::on_buttonBox_accepted()
{
    emit updatepaths(this->snrp,this->app,this->qp,this->configp);
}


// Set the paths


void Settings::on_snrpath_textEdited(const QString &arg1)
{
    this->snrp = arg1;
}

void Settings::on_configpath_textEdited(const QString &arg1)
{
    this->configp = arg1;
}

void Settings::on_qpath_textEdited(const QString &arg1)
{
    this->qp = arg1;
}

void Settings::on_appath_textEdited(const QString &arg1)
{
    this->app = arg1;
}

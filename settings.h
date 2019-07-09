#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QString hostname,QWidget *parent = nullptr);
    void path(QString snr,QString ap,QString q,QString config)
    {
//        snrp.clear();
//        //snrp.append(snr);

//        //snrp = snr;

//        app.clear();
//        //app.append(ap);

//        //app = ap;

//        qp.clear();
//        //qp.append(qp);

//        //qp = q;

//        configp.clear();
//        //configp.append(qp);
        //configp = config;
    }
    ~Settings();

private slots:
    void on_hostnamebox_textEdited(const QString &arg1);
    /*void on_checkBox_toggled(bool checked);
    void on_checkBox_2_toggled(bool checked);*/
    void on_graphnamebox_textEdited(const QString &arg1);
    void on_datanamebox_textEdited(const QString &arg1);

    void on_snrpath_textEdited(const QString &arg1);

    void on_buttonBox_accepted();

    void on_configpath_textEdited(const QString &arg1);

    void on_qpath_textEdited(const QString &arg1);

    void on_appath_textEdited(const QString &arg1);

signals:
    void hostnamechanged(QString arg1);
    void headerchanged(bool checked);
    void bodychanged(bool checked);
    void graphboxchanged(QString arg1);
    void datanameboxchanged(QString arg1);
    void updatepaths(QString snrp,QString app,QString qp,QString configp);

private:
    Ui::Settings *ui;
    QString qp;
    QString app;
    QString configp;
    QString snrp;
};

#endif // SETTINGS_H

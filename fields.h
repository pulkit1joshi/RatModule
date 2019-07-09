#ifndef FIELDS_H
#define FIELDS_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class fields;
}

class fields : public QDialog
{
    Q_OBJECT

public:
    explicit fields(QWidget *parent = nullptr);
    ~fields();

signals:
    void post(QString str);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::fields *ui;
};

#endif // FIELDS_H

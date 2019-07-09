#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui {
class config;
}

class config : public QDialog
{
    Q_OBJECT

public:
    explicit config(QString path, QWidget *parent = nullptr);
    ~config();

private slots:
    void on_buttonBox_accepted();

signals:
    void sockwrite(QByteArray str);

public slots:
    void created();

private:
    Ui::config *ui;
    QString fpath;
};

#endif // CONFIG_H

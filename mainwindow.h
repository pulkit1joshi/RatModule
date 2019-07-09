#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include "dialog.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include "cmd.h"
#include "settings.h"
#include "config.h"
#include "fields.h"
#include "ue.h"
#include <QHash>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void list(int req);
    void count(int req);
    void postf();
    void getf();
    void update();
    void initializegraphs()
    {

        float y;/*
        QFile inputFile(snrpath);
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
                 QString line = in.readLine();
                 QStringList row = line.split(splitter);
                 y = row.value(0).toDouble();
                 time = row.value(1).toDouble();
                 series->append(y,time);
                 deadseries->append(y,0);
                 qDebug() << line;
                 QTimer *t = new QTimer();
                 t->setSingleShot(true);
                 t->start(1000);
                 connect(t,SIGNAL(timeout()),this,SLOT(scroll()));
           }

           inputFile.close();
        }*/

        QFile inputFile2(qpath);
        if (inputFile2.open(QIODevice::ReadOnly))
        {
           QTextStream inq(&inputFile2);
           while (!inq.atEnd())
           {
                 QString line = inq.readLine();
                 QStringList row = line.split(splitter);
                 y = row.value(0).toDouble();
                 int time2 = row.value(1).toDouble();
                 qseries->append(y,time2);
                 qDebug() << "y: " << y << "Time:" << time2;
                 qxaxis->setMax(y);
                 int X = int(time2);
                 int Y = int(qxaxis->max());
                 if(X>Y)
                 {
                     qchart->scroll((qchart->plotArea().width() / 2), 0);
                     if(flag==1)
                     {
                         qyaxis->setRange(0,qyaxis->max()/2);
                     }


                 }
                 //Check the range
                 if(y > qyaxis->max())
                 {
                     qyaxis->setRange(0,y+5);
                 }
                 if(y < qyaxis->max()/2)
                 {
                     //flag = 1;
                 }



           }
           inputFile2.close();
        }
    }


private slots:
    void post2(QString str);
    void inforecv(QString str);
    void statusinf(QString str);
    void advance();
    void on_pushButton_clicked();
    void reflist(cmd obj);
    void scroll();
    void on_graphleft_clicked();
    void on_graphright_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_xaxisspinner_valueChanged(double arg1);
    void on_yaxisspinner_valueChanged(double arg1);
    void on_pushButton_2_clicked();
    void replyFinished(QNetworkReply *);
    void on_request_activated(const QString &arg1);
    void on_pushButton_4_clicked();
    void updatehostname( QString Q)
    {
        update();
        qDebug("Changing hostname");
        hostname = Q;
        update();
    }
    void updategraphname(QString Q)
    {
        qDebug("Changing graph name");
        graphtitle = Q;
        update();
    }
    void updatedataname(QString Q)
    {
        qDebug("Changing data name");
        dataname = Q;
        update();
    }
    void updateheader(bool B)
    {

    }
    void updatebody(bool B )
    {

    }
    void updatepaths(QString snrp, QString ap , QString qp , QString configp)
    {
        series->removePoints(0,series->count());
        cpuseries->removePoints(0,cpuseries->count());
        qseries->removePoints(0,qseries->count());
        snrpath = snrp;
        appath = ap;
        qpath = qp;
        configpath = configp;
        initializegraphs();
    }


    void on_pushButton_3_clicked();

    void on_checkBox_toggled(bool checked);

    void on_checkBox_2_toggled(bool checked);

    int find_ue(QString a,QString b);

    void newue(QString apid,QString ueid , int id);

    void show_graph();
    //void newap(int apid);

    void on_apbox_activated(const QString &arg1);

    void on_uebox_activated(const QString &arg1);

signals:
    void conclicked();

private:
    Ui::MainWindow *ui;
    Dialog *savecmd;
    int *num;
    int reqtype;
    QChart *chart;
    QChartView *chartview;
    QTimer *timer;
    QLineSeries *series;
    qreal time;
    qreal mintime;
    QValueAxis *xaxis;
    QValueAxis *yaxis;
    QString graphtitle;
    qreal xrange;
    qreal yrange;
    int flag=0;
    QNetworkAccessManager *mgr;
    QString hostname;
    QLineSeries *deadseries;
    QString dataname;
    QAreaSeries *areaseries;
    qreal xdiff;

    // CPU DEfines
    QTimer *usage;
    QChart *cpuchart;
    QChartView *cpuview;
    QLineSeries *cpuseries;
    qreal cputime;
    QValueAxis *cpuxaxis;
    QValueAxis *cpuyaxis;
    QLineSeries *deadcpuseries;
    QAreaSeries *cpuareaseries;

    // Dialogs and popups
    config *configedit;
    Settings *settingmenu;

    //QThroughput Chart Defines
    QLineSeries *qseries;
    QValueAxis *qxaxis;
    QValueAxis *qyaxis;
    QChartView *qchartview;
    QChart *qchart;


    //File PAth Defines

    QString snrpath;
    QString configpath;
    QString appath;
    QString qpath;
    QString splitter;

    // For AP-UE  graphs
    QHash<QString, int> packet_count; // Stores pakcet count depending on AP name
    QList<ue*> series_list; // Stores ue class list and creates dynamic series
    QList<QString> ap_list; // List of AP id
    QList<QString> uelist; // List of UE id

    int last_series_id;
    bool autoscale;
};

#endif // MAINWINDOW_H

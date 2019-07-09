#ifndef AP_H
#define AP_H

#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QHash>


class ap
{
public:
    ap();
    QList<QLineSeries*> series_list;
    QHash<QString,int> series_hash;

};

#endif // AP_H

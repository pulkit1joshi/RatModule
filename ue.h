#ifndef UE_H
#define UE_H

#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>


class ue : public QLineSeries
{
public:
    ue();
    QString ap;
    QString ueid;


};

#endif // UE_H

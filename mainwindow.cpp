#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpu.h"
#include "config.h"
#include "myserver.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //---------Initial Paths to store the recieved information--------------------------------------------------------//

    snrpath = "/home/pulkit/Desktop/snr.csv";
    qpath = "/home/pulkit/Desktop/qpth.csv";
    appath = " chck";
    configpath = "/home/pulkit/Desktop/config.txt";

    //---------------------------------------------------------------------------------------------------------------//


    //Initial hostname to connect to.

    hostname = "ptsv2.com/t/posttest/post";

    //---------------------------------------------------------------------------------------------------------------//

    //-----All the initial UI and how it looks is defined here-------------------------------------------------------//


    ui->setupUi(this);
    QFont font = ui->pushButton->font();
    QFont font2 = ui->pushButton_3->font();
    font.setPointSize(10);
    font2.setPointSize(10);
    this->setStyleSheet("background-color: rgba(255, 255, 255, 1);");
    ui->Menu->setStyleSheet("background-color: rgba(0, 0, 0, 1); background:qlineargradient(x1: 0, y1: 0, x2: 10, y2: 10);");
    ui->pushButton->setStyleSheet("color: white;");
    ui->pushButton_3->setStyleSheet("color: white;");
    ui->pushButton_4->setStyleSheet("color: white;");
    ui->doubleSpinBox->setMaximum(60000);
    ui->checkBox_2->setChecked(true);
    ui->statusbar->setFontWeight(6);
    ui->pushButton->setFont(font);
    ui->pushButton_3->setFont(font2);
    ui->xaxisspinner->setMaximum(10000);
    ui->yaxisspinner->setMaximum(10000);
    ui->cpubar->setValue(0);



    //------------Initial Initialisations-----------------------------------------------------------------------//

    autoscale = true;               // Initially autoscale is set true. This means that the graph updates every mintime seconds
    mintime = 0.001;                    // Minimum time for graph/data update. In seconds.
    time = 0;                       // Initial time. Dont touch. Dont know what it was for
    xrange = 60;                    // Initial xaxis range
    yrange = 60;                    // Initial y axis rande
    cputime = 0;                    // Initial cpu-progress bar value. Dont touch.
    graphtitle="SNR Graph";         // Graph title.
    dataname="SNR";                 // No use anymore

    //---------------------------------------------------------------------------------------------------------------//

    //##############################################################################################################//
    //---------------Dynamic allocation of memory-------------------------------------------------------------------//

    //---Axes----------------------------------------------------------------
    xaxis = new QValueAxis();           // xaxis of main ap/ue graph
    yaxis = new QValueAxis();           // yaxis of main ap/ue graph
    cpuxaxis = new QValueAxis();        // xaxis of CPU data graph
    cpuyaxis = new QValueAxis();        // yacis of CPU data graph
    qxaxis = new QValueAxis();          // xaxis of Third graph called queuethroughput graph
    qyaxis = new QValueAxis();          // yaxis of q   throughput graph
    //---Line series: They hold (x,y) points of any graph--------------------
    series = new QLineSeries();         // Obsolete. Just for old memories
    deadseries = new QLineSeries();     // Obsolete. Just for old memories
    cpuseries = new QLineSeries();      // The series for CPU graph.
    deadcpuseries = new QLineSeries();  // Dead series - CPU graph for making area series
    qseries = new QLineSeries();        // Series for qthroughput
    //--------------------QPART---------------------
    qchart = new QChart();
    chart = new QChart();
    cpuchart = new QChart();
    //---timer---
    timer = new QTimer(this);

    //------------Axis:Range,tickcount,gridlines and other paramets-------------------------
    qxaxis->setRange(0,10);
    qyaxis->setRange(0,1);
    xaxis->setGridLineVisible(true);
    yaxis->setTickCount(16);
    xaxis->setTickCount(11);
    cpuxaxis->setGridLineVisible(true);
    cpuyaxis->setGridLineVisible(true);
    cpuxaxis->setRange(0,10);
    cpuyaxis->setRange(0,100);
    cpuxaxis->setTickCount(10);
    cpuxaxis->hide();
    xaxis->setRange(0,xrange);
    yaxis->setRange(-10,yrange);
    ui->xaxisspinner->setValue(xrange);
    ui->yaxisspinner->setValue(yrange);
    cpuchart->setMargins(QMargins(0,0,0,0));
    qchart->setMargins(QMargins(0,0,0,0));
    cpuchart->setBackgroundRoundness(0);
    cpuchart->setTitle("CPU Usage");
    cpuseries->setName("Cpu %");
    qchart->setTitle("Queue Throughput");
    qseries->setName("Value");
    qchart->setAxisY(qyaxis,qseries);
    qchart->setAxisX(qxaxis,qseries);
    qchart->addSeries(qseries);
    cpuchart->setAxisY(cpuyaxis,cpuseries);
    cpuchart->setAxisX(cpuxaxis,cpuseries);
    cpuchart->setAxisY(cpuyaxis,deadcpuseries);
    cpuchart->setAxisX(cpuxaxis,deadcpuseries);
    chart->setAxisY(yaxis,deadseries);
    chart->setAxisX(xaxis,deadseries);
    cpuchart->setAnimationOptions(QChart::AllAnimations);
    chart->setTitle(graphtitle);
    chart->setAxisY(yaxis,series);
    chart->setAxisX(xaxis,series);
    chart->setAnimationOptions(QChart::NoAnimation);
    series->setName(dataname);

    qchartview = new QChartView(qchart);
    cpuview = new QChartView(cpuchart);
    chartview = new QChartView(chart);
    chartview->setSceneRect(0,0,10,10);
    ui->qview->addWidget(qchartview);
    ui->horizontalLayout->addWidget(chartview);
    ui->cpuframe->addWidget(cpuview);

    //------------------Timer---------------------------------------------------------------------//
    timer->start(mintime*1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(advance()));

    //---------------READING INITIALLY-------------
    initializegraphs();
    settingmenu->path(snrpath,appath,qpath,configpath);
    qseries->attachAxis(qxaxis);
    qseries->attachAxis(qyaxis);
    series->attachAxis(xaxis);
    series->attachAxis(yaxis);
    cpuseries->attachAxis(cpuxaxis);
    cpuseries->attachAxis(cpuyaxis);


    //-------------REQUESTS PART: Posting cmd for sdn controller -------------
    num= new int();
    count(reqtype);
    reqtype = 1;
    list(reqtype);
    savecmd = new Dialog(num);
    configedit = new config(configpath);
    connect(this,&MainWindow::conclicked,configedit,&config::created);
    settingmenu = new Settings(hostname,this);
    connect(settingmenu,&Settings::updatepaths,this,&MainWindow::updatepaths);
    connect(settingmenu,&Settings::hostnamechanged,this,&MainWindow::updatehostname);
    connect(settingmenu,&Settings::graphboxchanged,this,&MainWindow::updategraphname);
    connect(settingmenu,&Settings::datanameboxchanged,this,&MainWindow::updatedataname);
    connect(settingmenu,&Settings::headerchanged,this,&MainWindow::updateheader);
    connect(settingmenu,&Settings::bodychanged,this,&MainWindow::updatebody);

    mgr  = new QNetworkAccessManager(this);
    connect(savecmd,SIGNAL(refnow(cmd)),this,SLOT(reflist(cmd)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

    //-----------REQUEST PART ENDS---------------------------------------------------------------------------------//

        cpuareaseries = new QAreaSeries(deadcpuseries, cpuseries);
        //series->setName("Batman");
        QPen pen(0x059605);
        pen.setWidth(2);
        cpuareaseries->setPen(pen);

        QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
        gradient.setColorAt(0.0, 0x3cc63c);
        gradient.setColorAt(1.0, 0x26f626);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        cpuareaseries->setBrush(gradient);
        cpuchart->addSeries(cpuareaseries);
        cpuchart->setAxisY(cpuyaxis,cpuareaseries);
        cpuchart->setAxisX(cpuxaxis,cpuareaseries);
        cpuareaseries->setName("CPU %");


        areaseries = new QAreaSeries(deadseries, series);
        QLinearGradient gradient2(QPointF(0, 0), QPointF(0, 0.5));
        gradient2.setColorAt(0.0,Qt::red);
        gradient2.setColorAt(1.0, Qt::blue);
        gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
        areaseries->setBrush(gradient2);
        //chart->addSeries(areaseries);
        areaseries->setOpacity(0.2);
        QPen pen2(Qt::blue);
        pen.setWidth(2);
        areaseries->setPen(pen2);
        cpuareaseries->setOpacity(0.5);

        chart->setAxisY(yaxis,areaseries);
        chart->setAxisX(xaxis,areaseries);
        chart->legend()->setVisible(false);


        series->setPointsVisible(true);
        series->setPointLabelsFormat("(@xPoint,@yPoint)");
        qseries->setPointsVisible(true);


        deadseries->append(0,0);
        series->append(0,0);


        MyServer *mServer = new MyServer(this);


        //connect(mServer,SIGNAL(send),this,SLOT());
        connect(mServer,&MyServer::sendinf,this,&MainWindow::inforecv);
        connect(mServer,&MyServer::statusinf,this,&MainWindow::statusinf);
        connect(this->configedit,&config::sockwrite,mServer,&MyServer::sockwrite);

        mServer->StartServer();


        //Over server things

}

//----Advance----------------------------------------------------------------------------------------------------//
//  Aim : Every second this advance is called and the graphs are scrolled.
void MainWindow::advance()
{
    time = time+mintime;
    if(autoscale)
    {
    if(time-xdiff/2 >= 0){
    xaxis->setMax(time+xdiff/2);
    xaxis->setMin(time-xdiff/2); }
    else {
        xaxis->setMax(xdiff);
        xaxis->setMin(0);
    }
    }
    cpuchart->scroll(cpuchart->plotArea().width()/cpuxaxis->tickCount(),0);
    //xaxis->setRange(time-((ui->doubleSpinBox->text()).toInt())/2,time+((ui->doubleSpinBox->text()).toInt())/2);
}

void MainWindow::scroll()
{
    chart->scroll((xaxis->tickCount()/chart->plotArea().width()), 0);
    cpuchart->scroll((cpuxaxis->tickCount()/cpuchart->plotArea().width()), 0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//----------------------------Show up commands dialog: commands stand for the sdn commands that we can add-------------------//
void MainWindow::on_pushButton_clicked()
{
    savecmd->setModal(false);
    savecmd->exec();
}

void MainWindow::reflist(cmd obj)
{
    int i = ui->commandslist->count();
    ui->commandslist->addItem(obj.name);
}


void MainWindow::list(int req)
{

    //---------Reading all cmds--------------

    cmd obj;
    QFile inputFile("/home/pulkit/Desktop/cmd.txt");
    if (inputFile.open(QIODevice::ReadWrite))
    {

       QDataStream in(&inputFile);
       while(!in.atEnd())
       {
       in >> obj;
       if(req==obj.type)
       ui->commandslist->addItem(obj.name);
       }
    }
    else {
    }
    inputFile.close();

}

void MainWindow::count(int req)
{

    //---------Reading all cmds--------------

    cmd obj;
    QFile inputFile("/home/pulkit/Desktop/cmd.txt");
    if (inputFile.open(QIODevice::ReadWrite))
    {

       QDataStream in(&inputFile);
       while(!in.atEnd())
       {
       in >> obj;
       *num =  *num+1;
       }
       inputFile.close();
    }


}

void MainWindow::on_graphleft_clicked()
{
    chart->scroll((chart->plotArea().width() / 2), 0);
}


void MainWindow::on_graphright_clicked()
{

    chart->scroll(-(chart->plotArea().width() / 2), 0);
}

void MainWindow::on_xaxisspinner_valueChanged(double arg1)
{
    if(arg1 >= 0)
    {
    xdiff = arg1;
    xaxis->setRange(0,arg1);
    }
}

void MainWindow::on_yaxisspinner_valueChanged(double arg1)
{
    if(arg1 >= 1)
    {
    yaxis->setRange(-arg1,arg1);
    }
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if(arg1 >= 1)
    {
    mintime = (ui->doubleSpinBox->text()).toInt();
    }
}

cmd read2(QString name)
{

    cmd obj;
    QFile inputFile("/home/pulkit/Desktop/cmd.txt");

    if (inputFile.open(QIODevice::ReadOnly))
        {
           QDataStream in(&inputFile);
           while (!in.atEnd())
               {
                   in >> obj;
                   int x = QString::compare(obj.name, name, Qt::CaseInsensitive);
                   if(!x)
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


void MainWindow::on_pushButton_2_clicked()
{


    if(ui->commandslist->selectedItems().size() != 0)
    {
        cmd obj = read2(ui->commandslist->currentItem()->text());
            if(reqtype==1)
            {
                this->postf();
            }
            if(reqtype==2)
            {
                this->getf();
            }
              QString str;
              str.append("Send Request");
              if(reqtype==1) str.append(": Post\n");
              if(reqtype==2) str.append(": Get\n");
              str.append("Host: ");
              str.append(hostname);
              str.append("\nRequest Body:");
              str.append(obj.cmdstr);
              QMessageBox::information(this,"Info",str);
    }


}
void MainWindow::update()
{

    chart->setTitle(graphtitle);
    series->setName(dataname);

}

void MainWindow::post2(QString str)
{

    QProcess process;
    QString str2 = "curl -d " + str +" -X POST " + "https://" + hostname;
    //qDebug() << str2 << "\n" << str;
    process.start(str2);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    ui->Response->setText(output);
    //qDebug() << output;

}

void MainWindow::inforecv(QString str)
{

    //qDebug() << str;
    char x;
    QString first;
    QString cpu;
    int i ;
    for(i = 0 ; i < str.length(); i++)
    {
        if(str[i] == ',')
            break;
        first.append(str[i]);
    }




    //--------Defining new vars----------
    float y;

    y = first.toFloat();
    //---------Reading of File-----------
    if(deadseries->at(deadseries->count()-1).x() != y)
    {
    deadseries->append(time,0);
    series->append(time,y);
    //qDebug() << time;
    }

    if(autoscale)
    {
        int X = int(time);
        int Y = int(xaxis->max());
        //Check the range
        if(y > yaxis->max())
        {
            yaxis->setRange(yaxis->min(),y+5);
        }

        if(y < yaxis->min())
        {
            yaxis->setRange(y-2,yaxis->max());
        }

    }


    //Update the range
    chart->setAnimationOptions(QChart::NoAnimation);
    i=i+1;
    for(; i < str.length(); i++)
    {
        if(str[i] == ',')
            break;
        cpu.append(str[i]);
    }
    qreal cpuv = cpu.toFloat();
    ui->cpubar->setValue(cpuv);
    cpuseries->append(time,cpuv);
    deadcpuseries->append(time,0);
    //cputime +=1;
    if(time > 5) {
        cpuxaxis->setMax(time+5);
        cpuxaxis->setMin(time-5);
    }

    i=i+2;

    while(i < str.length())
    {
        QString ap;
        for(;i<str.length();i++)
        {
            if(str[i] == ",") break;
            ap.append(str[i]);
        }
        i=i+1;
        QString ue;
        for(;i<str.length();i++)
        {
            if(str[i] == ",") break;
            ue.append(str[i]);
        }
        i=i+1;
        QString snr_value_str;
        for(;i<str.length();i++)
        {
            if(str[i] == ",") break;
            snr_value_str.append(str[i]);
        }
        int snr_value = snr_value_str.toInt();
        snr_value = -1*snr_value;
        int x = find_ue(ap,ue);
        if(x != -10)
        {
            series_list[x]->append(time,snr_value);
            packet_count[ap] += 1;
            ui->no_packets->setStyleSheet("QLabel {color : white; }");
            ui->no_packets->setText(QString::number(packet_count[ui->apbox->currentText()]));
        }
        else {
            newue(ap,ue,series_list.count());
            series_list[series_list.count()-1]->append(time,snr_value);
            ui->no_packets->setStyleSheet("QLabel {color : white; }");
            ui->no_packets->setText(QString::number(packet_count[ui->apbox->currentText()]));
        }

        i=i+1;
    }






    /*QString qtput;

    i=i+1;
    for(; i < str.length(); i++)
    {
        if(str[i] == ',')
            break;
        qtput.append(str[i]);
    }
    qreal qtputv = qtput.toFloat();
    qseries->append(time,qtputv);
    //cputime +=1;
    if(time > qxaxis->max()) {
        qxaxis->setRange(0,time+1);
    }

    if(qtputv > qyaxis->max())
    {
        qyaxis->setRange(0,qtputv+3);
    }*/



    QFile outputFile(snrpath);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text | QFile::Text))
    {
           QTextStream out(&outputFile);
           QString str = QString::number(time) + ',' + QString::number(y) + '\n';
           out << str;
           //qDebug() << str;
           outputFile.flush();
           outputFile.close();

    }
//    QFile inputFile2(qpath);
//    if (inputFile2.open(QIODevice::WriteOnly | QIODevice::Text| QIODevice::Append))
//    {
//        QTextStream in(&inputFile2);
//        QString str = QString::number(time) + ',' + QString::number(qtputv) + '\n';
//        in << str;
//       inputFile2.close();
//    }



}

void MainWindow::statusinf(QString str)
{
    QString str2 = "Status: " + str;
    ui->statusbar->setText(str2);
}

void MainWindow::postf()
{

    QProcess process;
    QString str = ui->commandslist->currentItem()->text();
    cmd obj = read2(str);
    QString str2 = "curl -d " + obj.cmdstr +" -X POST " + "https://" + hostname;
    process.start(str2);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    ui->Response->setText(output);
    //qDebug() << output;

    //QString req;
    //mgr->post(QNetworkRequest(QUrl("https://" + hostname)),obj.cmdstr.toUtf8());

}
void MainWindow::getf()
{

    QString str = ui->commandslist->currentItem()->text();
    cmd obj = read2(str);
    QString req;
    mgr->get(QNetworkRequest(QUrl("https://" + hostname + "/" + obj.cmdstr)));
    //qDebug() << "https://" + hostname + obj.cmdstr;

}


void MainWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
       {
           //qDebug() << reply->errorString();
           ui->Response->setText(reply->errorString());
       }
       else
       {
//           qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
//           qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
//           qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
//           qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//           qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

           QString str;
           str.append("Content-Header: ");
           str.append(reply->header(QNetworkRequest::ContentTypeHeader).toString());
           str.append(" \n");
//           str.append("Last Modified Header: ");
//           str.append(reply->header(QNetworkRequest::LastModifiedHeader).toString());
//           str.append("\n");
//           str.append("Content Length Header: ");
//           str.append(reply->header(QNetworkRequest::ContentLengthHeader).toString());
//           str.append("\n");
           str.append("HTTPS status code: ");
           str.append(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
           str.append("\n");
//           str.append("HttpReasonPhraseAttribute: ");
//           str.append(reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
//           str.append("\n");
           str.append(reply->readAll());

//           qDebug() << str;
           ui->Response->setText(str);

    }

       reply->deleteLater();


}

void MainWindow::on_request_activated(const QString &arg1)
{
    if(arg1 == "Post")
    {
            reqtype = 1;
            int i = ui->commandslist->count();
            for(int k = 0; k<i;k++)
            {
                ui->commandslist->takeItem(k);
            }
            list(reqtype);
    }
    else if(arg1 == "Get")
    {
            reqtype = 2;
            int i = ui->commandslist->count();
            for(int k = 0; k<i;k++)
            {
                ui->commandslist->takeItem(k);
            }
            list(reqtype);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    //qDebug() << *num;
    settingmenu->setModal(true);
    settingmenu->exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    emit conclicked();
    configedit->setModal(true);
    configedit->exec();
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    series_list[last_series_id]->setPointLabelsVisible(checked);

}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    autoscale = checked;
}

int MainWindow::find_ue(QString a, QString b)
{
    int total = series_list.count();
    int i = 0;
    for(;i<total;i++)
    {
        //qDebug() << "Checking";
        if(series_list[i]->ueid == b && series_list[i]->ap == a) return i;
        //qDebug() << "All ok";
    }
    //qDebug() << "All ok";
    return -10;
}

void MainWindow::newue(QString apid, QString ueid, int id)
{
    ue *new_ue = new ue();
    new_ue->append(time-1,0);
    new_ue->attachAxis(xaxis);
    new_ue->attachAxis(yaxis);
    new_ue->ap = apid;
    new_ue->ueid = ueid;
    series_list.append(new_ue);
    //chart->addSeries(series_list[series_list.count()-1]);
    chart->setAxisX(xaxis,series_list[series_list.count()-1]);
    chart->setAxisY(yaxis,series_list[series_list.count()-1]);
    series_list[series_list.count()-1]->setPointsVisible(true);
    series_list[series_list.count()-1]->setPointLabelsFormat("(@xPoint,@yPoint)");
    if(ap_list.indexOf(apid) == -1)
    {
        packet_count[apid] =0;
        packet_count[apid] +=1;
        ui->apbox->addItem(apid);
        ap_list.append(apid);
    }

    if(uelist.indexOf(ueid) == -1)
    {
        ui->uebox->addItem(ueid);
        uelist.append(ueid);
    }
    show_graph();
}

void MainWindow::show_graph()
{
    int id = -1;
    QString ueid = ui->uebox->currentText();
    QString apid = ui->apbox->currentText();

    int total = series_list.count();
    int i = 0;
    for(;i<total;i++)
    {
        //qDebug() << "Checking";
        if(series_list[i]->ueid == ueid && series_list[i]->ap == apid)
        {
            id = i; break;
        }
        //qDebug() << "All ok";
    }
    if(i == total || id == -1)
    {
        ui->statusbar->setText("Status: The Ap or Ue for the selected doesn't exist. Graph will not be displayed");
        qDebug() << "Error:The Ap or Ue for the selected doesn't exist. Graph will not be displayed";
    }
    else {

        if(last_series_id != -1) chart->removeSeries(series_list[last_series_id]);
        chart->addSeries(series_list[id]);
        last_series_id = id;
        chart->setAxisX(xaxis,series_list[id]);
        chart->setAxisY(yaxis,series_list[id]);
        series_list[id]->setPointsVisible(true);
    }
}

void MainWindow::on_apbox_activated(const QString &arg1)
{
    show_graph();
    ui->no_packets->setStyleSheet("QLabel {color : white; }");
    ui->no_packets->setText(QString::number(packet_count[ui->apbox->currentText()]));
}

void MainWindow::on_uebox_activated(const QString &arg1)
{
    show_graph();
}

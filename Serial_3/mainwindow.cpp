#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QChartView>
#include <string.h>
#include <string>
#include <QScrollBar>
#include <QKeyEvent>
#include <QComboBox>
#include <QScrollBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      step(0)
{
    ui->setupUi(this);
    createChart();
    count=0;
    flag=true;
    mark=true;
    on_off=true;
    serial=new QSerialPort(this);
//    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
//    serial->setReadBufferSize(1);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
//    serial->open(QIODevice::ReadOnly);

//    connect(serial,&QIODevice::readyRead,this,&MainWindow::serialRecieve);


    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->serialBox->addItem(port.portName());

        if(port.serialNumber().startsWith("TKA-ODS_CALIBR"))
        {
            qDebug()<<"the choosen one "<<port.portName()<<" was found";
//            ui->serialList->setCurrentText(port.portName());
            ui->serialBox->setCurrentText(port.portName());
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
    delete serial;
}

void MainWindow::serialRecieve()
{
        //QByteArray ba;
        //ba=serial->readLine('\r');
        //QString re=QString::fromUtf8(ba);
//        QString re=serial->readBufferSize();
        QString re=serial->readLine('\n');
        qDebug()<<re;
        //    double reDoub;
        response+=re;
        //    count++;
        const char* elem=qPrintable(response);
        double average=atof(elem);
        if(average!=0){
            Vec1.push_back(average);
            qDebug()<<Vec1.last();
        }
        else{
            if(!Vec1.isEmpty()){
                int tmp=Vec1.last();
                Vec1.push_back(tmp);
            }
        }
        response.clear();
        if(Vec1.length()>2){
            m_series->append(msec_count, Vec1.at(step));
            step++;
            msec_count+=0.2;
            qDebug()<<step;
            if(Vec1.at(step)>=250 && Vec1.at(step)<500){
                m_axisY->setMax(500);

            }
            if(Vec1.at(step)>=500 && Vec1.at(step)<750){
                m_axisY->setMax(750);
            }
            if(Vec1.at(step)>=750 && Vec1.at(step)<1000){
                m_axisY->setMax(1000);
            }
            if(Vec1.at(step)>=1000 && Vec1.at(step)<1250){
                m_axisY->setMax(1250);
            }
            if(Vec1.at(step)>=1250 && Vec1.at(step)<1500){
                m_axisY->setMax(1500);
            }
            if(Vec1.at(step)>=1500 && Vec1.at(step)<2000){
                m_axisY->setMax(2000);
            }
            if(Vec1.at(step)>=2000 && Vec1.at(step)<2500){
                m_axisY->setMax(2500);
            }
            if(Vec1.at(step)>=2500 && Vec1.at(step)<3000){
                m_axisY->setMax(3000);
            }
            if(Vec1.at(step)>=3000 && Vec1.at(step)<3500){
                m_axisY->setMax(3500);
            }
            if(Vec1.at(step)>=3500 && Vec1.at(step)<4000){
                m_axisY->setMax(4000);
            }
            if(Vec1.at(step)>=4000){
                m_axisY->setMax(4500);
            }

            if(msec_count>=1.8 && step%2==0){
                qreal x=chart->plotArea().width()/m_axisX->tickCount();
                chart->scroll(x,0);
            }
        }
}

void MainWindow::startChart()
{
    if(flag){
        flag=false;
        createChart();
        qDebug()<<"getting chart";
    }
}

void MainWindow::createChart()
{
    qDebug()<<"start creating chart";
    chart=new QChart();
    m_series = new QSplineSeries();
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    QPen green(Qt::red);

    green.setWidth(3);
    m_series->setPen(green);
    m_series->append(0, 0);

    chart->addSeries(m_series);

    chart->addAxis(m_axisX,Qt::AlignBottom);
    chart->addAxis(m_axisY,Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    m_axisX->setTickCount(5);
    chart->setTitle("Dynamic spline chart");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
   m_axisX->setRange(0, 2);
    m_axisY->setRange(0, 250);
    qDebug()<<"chart created";

    chartView=new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
//    QScrollBar* scrollBar=new QScrollBar();
//    scrollBar->isVisible();
//    scrollBar->setEnabled(true);
//    chartView->setHorizontalScrollBar(scrollBar);
//    chartView->addScrollBarWidget(scrollBar,Qt::AlignBottom);
//    chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//chartView->horizontalScrollBar()->setEnabled(true);
//chartView->horizontalScrollBar()->setVisible(true);

    ui->verticalLayout->addWidget(chartView);
      msec_count=1;

}


void MainWindow::on_pushButton_clicked()
{
    if(on_off){
      connect(serial,&QIODevice::readyRead,this,&MainWindow::serialRecieve);
        serial->setPortName(ui->serialBox->currentText());
        serial->open(QIODevice::ReadOnly);
        on_off=false;
        ui->pushButton->setText("Disconnect");
    }
    else{
        if(!on_off){
            disconnect(serial,&QIODevice::readyRead,this,&MainWindow::serialRecieve);
            serial->close();
            on_off=true;
            ui->pushButton->setText("Connect");
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
 if(mark){
     disconnect(serial,&QIODevice::readyRead,this,&MainWindow::serialRecieve);
     mark=false;
 }
 else{
     if(!mark){
         connect(serial,&QIODevice::readyRead,this,&MainWindow::serialRecieve);
         mark=true;
     }
 }

}

void MainWindow::on_pushButton_3_clicked()
{
    ui->serialBox->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->serialBox->addItem(port.portName());

        if(port.serialNumber().startsWith("TKA-ODS_CALIBR"))
        {
            qDebug()<<"the choosen one "<<port.portName()<<" was found";
            ui->serialBox->setCurrentText(port.portName());
        }
    }
}

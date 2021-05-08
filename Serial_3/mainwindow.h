#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QKeyEvent>
#include <QVector>
#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <QtCharts/QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void serialRecieve();//получение
    void startChart();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    void createChart();
    //void serialRecieve();


private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    QString response;
    QVector <int> Vec1;
    int step;
    int count;

    QChart* chart;
    QTimer m_timer;
    QTimer m_timer_2;
    QTimer m_timer_3;
    QSplineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QChartView *chartView;
    bool flag;
    bool mark;
    bool on_off;
    double msec_count;

};
#endif // MAINWINDOW_H

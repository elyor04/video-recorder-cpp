#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HKIPcamera.h"
#include <opencv2/opencv.hpp>
#include <QSystemTrayIcon>
#include <QString>
#include <QTimer>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void _init();
    void quitAction();
    void activatedIcon(QSystemTrayIcon::ActivationReason reason);
    void myTimer();

public:
    Ui::MainWindow *ui;
    HKIPcamera hkipc;
    VideoWriter writer;
    QSystemTrayIcon* trayIcon;
    QTimer* myTm;
    QString recDir;
    bool success;
    bool isReady;
    bool reOpen;

private slots:
    void on_okButton_clicked();
    void on_browseTB_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
};
#endif // MAINWINDOW_H

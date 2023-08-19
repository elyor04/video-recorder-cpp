#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QMenu>
#include <QStyle>
#include <QApplication>
#include <QAction>
#include <QObject>
#include <QIcon>

bool mkdir_cd(QDir &path, QString dirName) {
    path.mkdir(dirName);
    return path.cd(dirName);
}

QImage cvMatToQImage(Mat &inMat) {
    switch (inMat.type()) {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows,
                     static_cast<int>(inMat.step), QImage::Format_ARGB32);
        return image;
    }
    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows,
                     static_cast<int>(inMat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows,
                     static_cast<int>(inMat.step), QImage::Format_Grayscale8);
        return image;
    }
    // none
    default:
        qWarning() << "cvMatToQImage() - Mat image type not handled in switch:"
                   << inMat.type();
        return QImage();
    }
}

QPixmap cvMatToQPixmap(Mat &inMat) {
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

void f_ImageDataCallBack(Mat bgrMat, void *pUser) {
    MainWindow *self = static_cast<MainWindow *>(pUser);

    if (self->reOpen) {
        if (self->writer.isOpened())
            self->writer.release();
        self->reOpen = false;
    }
    if (self->isReady) {
        if (!self->writer.isOpened()) {

            QDir myDir = QDir(self->recDir);
            QDateTime now = QDateTime::currentDateTime();
            mkdir_cd(myDir, now.toString("yyyy"));
            mkdir_cd(myDir, now.toString("MM"));
            mkdir_cd(myDir, now.toString("dd"));
            QString myFile = myDir.filePath(now.toString("hh_mm_ss") + ".mp4");

            int codec = VideoWriter::fourcc('m','p','4','v');
            double fps = 25;
            Size frameSize = Size(bgrMat.cols, bgrMat.rows);
            bool isColor = (bgrMat.type() == CV_8UC3);

            self->writer.open(myFile.toStdString(), codec, fps, frameSize, isColor);
        }
        else
            self->writer.write(bgrMat);
    }
    else if (self->writer.isOpened())
        self->writer.release();

    if (self->ui->tabWidget->currentIndex() == 1) {
        Mat smallMat;
        resize(bgrMat, smallMat,
               Size(self->ui->videoLabel->width(), self->ui->videoLabel->height()), 0, 0, INTER_AREA);
        self->ui->videoLabel->setPixmap(cvMatToQPixmap(smallMat));
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _init();
}

MainWindow::~MainWindow()
{
    delete ui;
    hkipc.release();
    writer.release();
}

void MainWindow::_init() {
    ui->ipAddress->setText("192.168.64.12");
    ui->userName->setText("admin");
    ui->password->setText("abcd1234");
    ui->port->setText("8000");

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon_i = style()->standardIcon(QStyle::StandardPixmap::SP_FileDialogListView);
    QMenu* icon_m = new QMenu(this);

    {
        QAction* act1 = new QAction("Show", this);
        QObject::connect(act1, &QAction::triggered, this, &MainWindow::show);
        icon_m->addAction(act1);

        QAction* act2 = new QAction("Hide", this);
        QObject::connect(act2, &QAction::triggered, this, &MainWindow::hide);
        icon_m->addAction(act2);

        icon_m->addSeparator();

        QAction* act3 = new QAction("Quit", this);
        QObject::connect(act3, &QAction::triggered, this, &MainWindow::quitAction);
        icon_m->addAction(act3);
    }

    trayIcon->setIcon(icon_i);
    trayIcon->setContextMenu(icon_m);
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::activatedIcon);
    trayIcon->show();

    myTm = new QTimer(this);
    QObject::connect(myTm, &QTimer::timeout, this, &MainWindow::myTimer);
    myTm->start(10);

    success = false;
    isReady = false;
    reOpen = false;
}

void MainWindow::quitAction() {
    trayIcon->hide();
    QApplication::quit();
}

void MainWindow::activatedIcon(QSystemTrayIcon::ActivationReason reason) {
    if ((reason == QSystemTrayIcon::ActivationReason::Trigger)
        || (reason == QSystemTrayIcon::ActivationReason::DoubleClick)) {
        showNormal();
    }
}

void MainWindow::myTimer() {
    if (isReady) {
        QDateTime now = QDateTime::currentDateTime();
        int sec = ui->duration->value() * 60 - now.toString("ss").toInt();
        myTm->start((sec + 1) * 1000);
        reOpen = true;
    }
    else
        myTm->start(10);
}

void MainWindow::on_okButton_clicked()
{
    if (success) {
        hkipc.release();
        success = false;
    }
    if (hkipc.login((char*)(ui->ipAddress->text().toStdString().c_str()),
                    (char*)(ui->userName->text().toStdString().c_str()),
                    (char*)(ui->password->text().toStdString().c_str()),
                    ui->port->text().toInt())) {
        hkipc.setImageDataCallBack(f_ImageDataCallBack, this);
        success = true;
        QMessageBox::information(this, "Start process", "Success\nYou can go to another page");
    }
    else {
        ui->videoLabel->clear();
        success = false;
        QMessageBox::warning(this, "Start process", "Failed\nTry again");
    }
}

void MainWindow::on_browseTB_clicked()
{
    QString _f = QFileDialog::getExistingDirectory(this, "Choose a Folder to save Recordings");
    if (!_f.isEmpty())
        ui->folder->setText(_f);
}

void MainWindow::on_startButton_clicked()
{
    if (isReady)
        QMessageBox::about(this, "Record process", "Already started");
    else if (QFileInfo(ui->folder->text()).isDir()) {
        recDir = ui->folder->text();
        isReady = true;
        QMessageBox::information(this, "Record process", "Success\nRecording started");
    }
    else
        QMessageBox::warning(this, "Record process", "Error\nInvalid folder");
}

void MainWindow::on_stopButton_clicked()
{
    if (isReady) {
        isReady = false;
        QMessageBox::information(this, "Record process", "Success\nRecording stopped");
    }
    else
        QMessageBox::warning(this, "Record process", "Error\nNothing to stop");
}

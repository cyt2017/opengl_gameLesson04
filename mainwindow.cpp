#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString name0 = "main.jpg";
    std::vector<QString> fileName;
    fileName.push_back(name0);
    name0 = "fog.bmp";
    fileName.push_back(name0);
    ui->widget->init_QGW(fileName);
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    timer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimeOut()
{
    ui->widget->drawImage();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    xoroshiroinverse(new QProcess)
{
    program = "java";
    arguments << "-jar" << "xoroshiroinverse.jar";
    xoroshiroinverse->setReadChannelMode(QProcess::MergedChannels);
    xoroshiroinverse->start(program, QStringList() << arguments);
    xoroshiroinverse->waitForStarted(1500);
    xoroshiroinverse->waitForFinished(1500);

    QByteArray output = xoroshiroinverse->readAll();
    ui->setupUi(this);
    ui->textBrowser->setText(output);
}

MainWindow::~MainWindow()
{
    delete ui;
}


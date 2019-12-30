#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    xoroshiroinverse(new QProcess)
{
    program = "java";
    arguments << "-jar" << "xoroshiroinverse.jar";
    xoroshiroinverse->setReadChannelMode(QProcess::SeparateChannels);
    xoroshiroinverse->setStandardInputFile("filein");
    xoroshiroinverse->setStandardOutputFile("fileout");
    xoroshiroinverse->start(program, arguments);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    xoroshiroinverse(new QProcess)
{
    IVvalidator.setRange(0, 31);
    program = "java";
    arguments << "-jar" << "xoroshiroinverse.jar";
    xoroshiroinverse->setReadChannelMode(QProcess::MergedChannels);
    xoroshiroinverse->start(program, QStringList() << arguments);
    /*
    xoroshiroinverse->waitForStarted(500);
    xoroshiroinverse->waitForFinished(500);
    xoroshiroinverse->write("31 31 31 31 31 31\n");
    xoroshiroinverse->waitForStarted(500);
    xoroshiroinverse->waitForFinished(500);
    QByteArray output = xoroshiroinverse->readAll(); */
    ui->setupUi(this);
    connect(ui->calculateButton, SIGNAL(clicked()), this, SLOT(processInput()));
    errBox.setIcon(QMessageBox::Warning);
    // TODO: chenge this mess to promoting the widget to IVLineEdit subclassing the QLineEdit
    ui->HPLE1_3->setValidator(&IVvalidator);
    ui->AtkLE1_3->setValidator(&IVvalidator);
    ui->DefLE1_3->setValidator(&IVvalidator);
    ui->SpALE1_3->setValidator(&IVvalidator);
    ui->SpDLE1_3->setValidator(&IVvalidator);
    ui->SpeLE1_3->setValidator(&IVvalidator);

    ui->HPLE1_4->setValidator(&IVvalidator);
    ui->AtkLE1_4->setValidator(&IVvalidator);
    ui->DefLE1_4->setValidator(&IVvalidator);
    ui->SpALE1_4->setValidator(&IVvalidator);
    ui->SpDLE1_4->setValidator(&IVvalidator);
    ui->SpeLE1_4->setValidator(&IVvalidator);

    ui->HPLE2->setValidator(&IVvalidator);
    ui->AtkLE2->setValidator(&IVvalidator);
    ui->DefLE2->setValidator(&IVvalidator);
    ui->SpALE2->setValidator(&IVvalidator);
    ui->SpDLE2->setValidator(&IVvalidator);
    ui->SpeLE2->setValidator(&IVvalidator);

    ui->HPLE3->setValidator(&IVvalidator);
    ui->AtkLE3->setValidator(&IVvalidator);
    ui->DefLE3->setValidator(&IVvalidator);
    ui->SpALE3->setValidator(&IVvalidator);
    ui->SpDLE3->setValidator(&IVvalidator);
    ui->SpeLE3->setValidator(&IVvalidator);
    // ui->consoleBrowser->setText(output);
}

MainWindow::~MainWindow()
{
    delete ui;
    xoroshiroinverse->kill();
}

void MainWindow::processInput()
{
    bool IVInputValid;
    IVInputValid =
            ui->HPLE1_3->hasAcceptableInput() & ui->AtkLE1_3->hasAcceptableInput() & ui->DefLE1_3->hasAcceptableInput() &
            ui->SpALE1_3->hasAcceptableInput() & ui->SpDLE1_3->hasAcceptableInput() & ui->SpeLE1_3->hasAcceptableInput() &
            ui->HPLE1_4->hasAcceptableInput() & ui->AtkLE1_4->hasAcceptableInput() & ui->DefLE1_4->hasAcceptableInput() &
            ui->SpALE1_4->hasAcceptableInput() & ui->SpDLE1_4->hasAcceptableInput() & ui->SpeLE1_4->hasAcceptableInput() &
            ui->HPLE2->hasAcceptableInput() & ui->AtkLE2->hasAcceptableInput() & ui->DefLE2->hasAcceptableInput() &
            ui->SpALE2->hasAcceptableInput() & ui->SpDLE2->hasAcceptableInput() & ui->SpeLE2->hasAcceptableInput() &
            ui->HPLE3->hasAcceptableInput() & ui->AtkLE3->hasAcceptableInput() & ui->DefLE3->hasAcceptableInput() &
            ui->SpALE3->hasAcceptableInput() & ui->SpDLE3->hasAcceptableInput() & ui->SpeLE3->hasAcceptableInput();
    if(!IVInputValid)
    {
        errBox.setText("Check your IV input");
        errBox.exec();
        return;
    }
    QStringList stats1_3;
    stats1_3 << ui->HPLE1_3->text() << ui->AtkLE1_3->text() << ui->DefLE1_3->text() << ui->SpALE1_3->text() << ui->SpDLE1_3->text() << ui->SpeLE1_3->text();
    qDebug() << stats1_3.join(" ");
}

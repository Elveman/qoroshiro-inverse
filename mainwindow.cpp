#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO: add label so they could be translated

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    xoroshiroinverse(new QProcess)
{
    IVvalidator.setRegExp(QRegExp("([12]{0,1}[0-9])|([3][01])"));
    ui->setupUi(this);
    connect(ui->calculateButton, SIGNAL(clicked()), this, SLOT(processInput()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(processExit()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(processStop()));
    ui->stopButton->setDisabled(true);
    ui->possibleSeedLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // FIXME: change this mess to promoting the widget to IVLineEdit subclassing the QLineEdit
    IVLERefTable =
    {
        ui->HPLE1_3, ui->AtkLE1_3, ui->DefLE1_3, ui->SpALE1_3, ui->SpDLE1_3, ui->SpeLE1_3, // div 6 = 0
        ui->HPLE1_4, ui->AtkLE1_4, ui->DefLE1_4, ui->SpALE1_4, ui->SpDLE1_4, ui->SpeLE1_4, // div 6 = 1
        ui->HPLE2, ui->AtkLE2, ui->DefLE2, ui->SpALE2, ui->SpDLE2, ui->SpeLE2, // div 6 = 2
        ui->HPLE3, ui->AtkLE3, ui->DefLE3, ui->SpALE3, ui->SpDLE3, ui->SpeLE3 // div 6 = 3
    };

    for (auto IVLE : IVLERefTable) {
        IVLE->setValidator(&IVvalidator);
    }
    // ui->consoleBrowser->setText(output);
}

MainWindow::~MainWindow()
{
    delete ui;
    xoroshiroinverse->kill();
}

void MainWindow::processInput()
{
    // sanity checks
    // TODO: more comments

    QStringList IVs1_3;
    QStringList IVs1_4;
    QStringList IVs2;
    QStringList IVs3;

    for (int i = 0; i < 6; i++) IVs1_3 << IVLERefTable.at(i)->text();
    for (int i = 6; i < 12; i++) IVs1_4 << IVLERefTable.at(i)->text();
    for (int i = 12; i < 18; i++) IVs2 << IVLERefTable.at(i)->text();
    for (int i = 18; i < 24; i++) IVs3 << IVLERefTable.at(i)->text();

    bool IVGeneralInputValid = true;
    bool HAInputCompatible;
    QStringList charCompare;
    charCompare << "31" << IVs1_4;
    bool characteristicValid;
    bool IVPerfectInputValid;
    int perfectCount;


    // FIXME: make a vector of pointers to widgets and provide a reference table
    for (auto IVLE : IVLERefTable) {
        IVGeneralInputValid &= IVLE->hasAcceptableInput();
    }

    if(!IVGeneralInputValid)
    {
        errBox.setIcon(QMessageBox::Critical);
        errBox.setText("Check your IV input");
        errBox.exec();
        return;
    }

    IVPerfectInputValid = true;
    perfectCount = 0;
    for (int i = 0; i < 6; i++) {
        if (IVs1_3.at(i) == "31") perfectCount++;
    }
    if (perfectCount != 3) IVPerfectInputValid = false;
    perfectCount = 0;
    for (int i = 0; i < 6; i++) {
        if (IVs1_4.at(i) == "31") perfectCount++;
    }
    if (perfectCount != 4) IVPerfectInputValid = false;
    perfectCount = 0;
    for (int i = 0; i < 6; i++) {
        if (IVs2.at(i) == "31") perfectCount++;
    }
    if ((perfectCount < 3) | (perfectCount > 4)) IVPerfectInputValid = false;
    perfectCount = 0;
    for (int i = 0; i < 6; i++) {
        if (IVs3.at(i) == "31") perfectCount++;
    }
    if ((perfectCount < 3) | (perfectCount > 4)) IVPerfectInputValid = false;
    if (!IVPerfectInputValid)
    {
        errBox.setIcon(QMessageBox::Critical);
        errBox.setText("The amount of perfect IVs is invalid");
        errBox.exec();
        return;
    }

    HAInputCompatible = !(((ui->HA1->checkState() == Qt::Unchecked) & (ui->abilityCombo1->currentText() == "Hidden")) |
            ((ui->HA2->checkState() == Qt::Unchecked) & (ui->abilityCombo2->currentText() == "Hidden")) |
            ((ui->HA3->checkState() == Qt::Unchecked) & (ui->abilityCombo3->currentText() == "Hidden")));
    if(!HAInputCompatible)
    {
        errBox.setIcon(QMessageBox::Critical);
        errBox.setText("HA possibility not checked, but hidden ability is chosen");
        errBox.exec();
        return;
    }

    characteristicValid = (charCompare.at(ui->characteristicCombo1->currentIndex()) == "31");
    if(!characteristicValid)
    {
        errBox.setIcon(QMessageBox::Critical);
        errBox.setText("Invalid characteristic of 4IV 4th frame pokemon");
        errBox.exec();
        return;
    }

    // now the validity check
    vldState validityCheck = checkValidity(IVs1_3.join(" "), IVs1_4.join(" "));
    if (validityCheck == Invalid)
    {
        errBox.setIcon(QMessageBox::Warning);
        errBox.setText("This set if IVs on frame 4 Pokemon has very low probability of finding the seed! Do you want to continue?");
        errBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = errBox.exec();
        switch(ret)
        {
            case QMessageBox::Yes:
                break;
            default:
                return;
        }
    } else if (validityCheck == NotValid4IV)
    {
        errBox.setIcon(QMessageBox::Critical);
        errBox.setText("Invalid IVs of 4IV 4th frame pokemon");
        errBox.exec();
        return;
    }

    // lock the fields
    // TODO: I seriously need the reference table to fix this mess
    for (auto IVLE : IVLERefTable) {
        IVLE->setDisabled(true);
    }

    ui->natureCombo1->setDisabled(true);
    ui->natureCombo2->setDisabled(true);
    ui->natureCombo3->setDisabled(true);

    ui->abilityCombo1->setDisabled(true);
    ui->abilityCombo2->setDisabled(true);
    ui->abilityCombo3->setDisabled(true);

    ui->characteristicCombo1->setDisabled(true);

    ui->starAmountCombo2->setDisabled(true);
    ui->starAmountCombo3->setDisabled(true);

    ui->HA1->setDisabled(true);
    ui->HA2->setDisabled(true);
    ui->HA3->setDisabled(true);

    ui->genderEval1->setDisabled(true);
    ui->genderEval2->setDisabled(true);
    ui->genderEval3->setDisabled(true);

    ui->calculateButton->setDisabled(true);
    // enable Stop button
    ui->stopButton->setDisabled(false);

    // now we compile everything we need
    QString IVs1_3Ready = IVs1_3.join(" "); // precise IVs of the 1st raid pokemon
    QStringList consecutiveList;
    for (int i = 0; i < 6; i++)
    {
        if (IVs1_4.at(i) != "31") consecutiveList << IVs1_4.at(i);
    }
    QString consecutiveIVs = consecutiveList.join(" "); // rest of consecutive IVs
    QString stars2Ready = ui->starAmountCombo2->currentText(); // number of stars of 2nd raid pokemon
    QString IVs2Ready = IVs2.join(" "); // lower/upper bounds of IVs of the 2nd raid pokemon
    QString stars3Ready = ui->starAmountCombo3->currentText(); // number of stars of 3rd raid pokemon
    QString IVs3Ready = IVs3.join(" "); // lower/upper bounds of IVs of the 3rd raid pokemon

    QString HAPossibility1 = ((ui->HA1->checkState() == Qt::Unchecked) ? "n" : "y"); // a chance that a 1st raid has a hidden ability
    QString Ability1; // an ability of the 1st raid pokemon
    switch(ui->abilityCombo1->currentIndex())
    {
    case 0:
        Ability1 = "0";
        break;
    case 1:
        Ability1 = "1";
        break;
    case 2:
        Ability1 = "hidden";
        break;
    default:
        Ability1 = ((ui->HA1->checkState() == Qt::Unchecked) ? "ignore" : "ordinary");
        break;
    }
    QString genderEval1 = ((ui->genderEval1->checkState() == Qt::Unchecked) ? "n" : "y");
    QString Nature1 = ui->natureCombo1->currentText();

    QString HAPossibility2 = ((ui->HA2->checkState() == Qt::Unchecked) ? "n" : "y"); // a chance that a 2nd raid has a hidden ability
    QString Ability2; // an ability of the 2nd raid pokemon
    switch(ui->abilityCombo2->currentIndex())
    {
    case 0:
        Ability2 = "0";
        break;
    case 1:
        Ability2 = "1";
        break;
    case 2:
        Ability2 = "hidden";
        break;
    default:
        Ability2 = ((ui->HA2->checkState() == Qt::Unchecked) ? "ignore" : "ordinary");
        break;
    }
    QString genderEval2 = ((ui->genderEval2->checkState() == Qt::Unchecked) ? "n" : "y");
    QString Nature2 = ui->natureCombo2->currentText();

    QString HAPossibility3 = ((ui->HA3->checkState() == Qt::Unchecked) ? "n" : "y"); // a chance that a 2nd raid has a hidden ability
    QString Ability3; // an ability of the 2nd raid pokemon
    switch(ui->abilityCombo3->currentIndex())
    {
    case 0:
        Ability3 = "0";
        break;
    case 1:
        Ability3 = "1";
        break;
    case 2:
        Ability3 = "hidden";
        break;
    default:
        Ability3 = ((ui->HA3->checkState() == Qt::Unchecked) ? "ignore" : "ordinary");
        break;
    }
    QString genderEval3 = ((ui->genderEval3->checkState() == Qt::Unchecked) ? "n" : "y");
    QString Nature3 = ui->natureCombo3->currentText();
    QString ECString;
    int trueOrder[6] = {0, 1, 2, 5, 3, 4}; // pre-gen 8 order of stats in gen 8 order of stats
    int newOrder[6] = {0, 1, 2, 4, 5, 3}; // gen 8 order of stats in pre-gen 8 order of stats
    if (ui->characteristicCombo1->currentIndex() == 0)
    {
        ECString = "ignore";
    } else
    {
        if(IVs1_4.at(trueOrder[abs(newOrder[(ui->characteristicCombo1->currentIndex()-1)] - 1)]) == "31")
        {
            ECString = QString::number(abs(newOrder[(ui->characteristicCombo1->currentIndex()-1)]) % 2);
        } else ECString = "ignore";
    }
    QStringList inputData;
    inputData << IVs1_3Ready;
    inputData << consecutiveIVs;
    inputData << stars2Ready;
    inputData << IVs2Ready;
    inputData << IVs2Ready;
    inputData << stars3Ready;
    inputData << IVs3Ready;
    inputData << IVs3Ready;
    inputData << HAPossibility1;
    inputData << Ability1;
    inputData << genderEval1;
    inputData << Nature1;
    inputData << HAPossibility2;
    inputData << Ability2;
    inputData << genderEval2;
    inputData << Nature2;
    inputData << HAPossibility3;
    inputData << Ability3;
    inputData << genderEval3;
    inputData << Nature3;
    inputData << ECString;

    ui->possibleSeedLabel->setText("Your seed should appear here, if found");
    ui->consoleBrowser->setText("");

    program = "java";
    arguments << "-jar" << "xoroshiroinverse.jar";
    xoroshiroinverse->setReadChannelMode(QProcess::MergedChannels);
    xoroshiroinverse->start(program, QStringList() << arguments);
    xoroshiroinverse->waitForStarted(500);

    for (QString inputString : inputData) {
        xoroshiroinverse->waitForFinished(100);
        qDebug() << inputString;
        xoroshiroinverse->write(inputString.toUtf8());
        xoroshiroinverse->write("\n");
    }
    xoroshiroinverse->waitForFinished(500);
    QByteArray output = xoroshiroinverse->readAll();
    ui->consoleBrowser->setText(output);
    connect(xoroshiroinverse, SIGNAL(readyRead()), this, SLOT(setNewText()));
}

void MainWindow::setNewText()
{
    xoroshiroinverse->waitForFinished(100);
    QRegExp seedRegExp("[0][x]([0-9A-Fa-f]{1,16})");
    QByteArray output = xoroshiroinverse->readAll();
    QByteArray currBrowser = ui->consoleBrowser->toPlainText().toUtf8();
    currBrowser.append(output);
    ui->consoleBrowser->setText(currBrowser);
    if (QString(currBrowser).contains(seedRegExp))
    {
        seedRegExp.indexIn(QString(currBrowser));
        ui->possibleSeedLabel->setText(seedRegExp.capturedTexts().at(0));
        processStop();
    }
    if (QString(currBrowser).contains("finish")) processStop();
}

MainWindow::vldState MainWindow::checkValidity(QString frameIVs, QString frameIVs4)
{
            QRegExp re("([12]{0,1}[0-9])|([3][01]) ([12]{0,1}[0-9])|([3][01]) ([12]{0,1}[0-9])|([3][01]) ([12]{0,1}[0-9])|([3][01]) ([12]{0,1}[0-9])|([3][01]) ([12]{0,1}[0-9])|([3][01])");
            QString currentIVs = frameIVs;
            QString currentIVs4 = frameIVs4;
            QVector<int> currentIVsMod8 = {-1, -1, -1, -1, -1, -1};
            QVector<int> currentPerfectIVs = {-1, -1, -1, -1, -1, -1};
            std::vector<std::string> statNames = {"HP", "Atk", "Def", "SpA", "SpD", "Spe"};
            int perfectIVcount = 0;
            int skipAmount = 0;
            int lastReroll = -1;
            bool IVmatchFlag4 = true;
            QStringList currentIVnums = currentIVs.split(" ");
            for (int i = 0; i < 6; i++) // check for errors
            {
                if (currentIVnums.at(i) == "31")
                {
                    perfectIVcount++;
                    currentPerfectIVs[i] = 1;
                }
                currentIVsMod8[i] = currentIVnums.at(i).toInt() % 8;
            }
            for (int i = 0; i < 6; i++) // check validity
            {
                if (currentPerfectIVs.at(i) == 1)
                {
                    continue;
                }
                if ((currentIVsMod8.at(i) >= 6) || (currentPerfectIVs.at(currentIVsMod8.at(i)) == 1))
                {
                    skipAmount++;
                } else
                {
                    lastReroll = i;
                    break;
                }
            }
            if (skipAmount == 2)
            {
                currentPerfectIVs[currentIVsMod8[lastReroll]] = 1;
                for (int i = 0; i < 6; i++)
                {
                    if ((currentPerfectIVs.at(i) == 1) & (currentIVs4.split(" ").at(i) != "31"))
                    {
                        IVmatchFlag4= false;
                    }
                }
                return (IVmatchFlag4 ? Valid : NotValid4IV);
            } else
            {
                return Invalid;
            }
}

void MainWindow::processExit()
{
    QCoreApplication::quit();
}

void MainWindow::processStop()
{
    xoroshiroinverse->kill();
    xoroshiroinverse->waitForFinished(1000);
    delete xoroshiroinverse;
    xoroshiroinverse = new QProcess;

    for (auto IVLE : IVLERefTable) {
        IVLE->setDisabled(false);
    }

    ui->natureCombo1->setDisabled(false);
    ui->natureCombo2->setDisabled(false);
    ui->natureCombo3->setDisabled(false);

    ui->abilityCombo1->setDisabled(false);
    ui->abilityCombo2->setDisabled(false);
    ui->abilityCombo3->setDisabled(false);

    ui->characteristicCombo1->setDisabled(false);

    ui->starAmountCombo2->setDisabled(false);
    ui->starAmountCombo3->setDisabled(false);

    ui->HA1->setDisabled(false);
    ui->HA2->setDisabled(false);
    ui->HA3->setDisabled(false);

    ui->genderEval1->setDisabled(false);
    ui->genderEval2->setDisabled(false);
    ui->genderEval3->setDisabled(false);

    ui->calculateButton->setDisabled(false);
    ui->stopButton->setDisabled(true);
}

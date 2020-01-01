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
    ui->setupUi(this);
    connect(ui->calculateButton, SIGNAL(clicked()), this, SLOT(processInput()));
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
    // sanity checks
    QStringList IVs1_3;
    QStringList IVs1_4;
    QStringList IVs2;
    QStringList IVs3;

    IVs1_3 << ui->HPLE1_3->text() << ui->AtkLE1_3->text() << ui->DefLE1_3->text() <<
              ui->SpALE1_3->text() << ui->SpDLE1_3->text() << ui->SpeLE1_3->text();
    IVs1_4 << ui->HPLE1_4->text() << ui->AtkLE1_4->text() << ui->DefLE1_4->text() <<
              ui->SpALE1_4->text() << ui->SpDLE1_4->text() << ui->SpeLE1_4->text();
    IVs2 << ui->HPLE2->text() << ui->AtkLE2->text() << ui->DefLE2->text() <<
              ui->SpALE2->text() << ui->SpDLE2->text() << ui->SpeLE2->text();
    IVs3 << ui->HPLE3->text() << ui->AtkLE3->text() << ui->DefLE3->text() <<
              ui->SpALE3->text() << ui->SpDLE3->text() << ui->SpeLE3->text();

    bool IVGeneralInputValid;
    bool HAInputCompatible;
    QStringList charCompare;
    charCompare << "31" << IVs1_4;
    bool characteristicValid;
    bool IVPerfectInputValid;
    int perfectCount;

    IVGeneralInputValid =
            ui->HPLE1_3->hasAcceptableInput() & ui->AtkLE1_3->hasAcceptableInput() & ui->DefLE1_3->hasAcceptableInput() &
            ui->SpALE1_3->hasAcceptableInput() & ui->SpDLE1_3->hasAcceptableInput() & ui->SpeLE1_3->hasAcceptableInput() &
            ui->HPLE1_4->hasAcceptableInput() & ui->AtkLE1_4->hasAcceptableInput() & ui->DefLE1_4->hasAcceptableInput() &
            ui->SpALE1_4->hasAcceptableInput() & ui->SpDLE1_4->hasAcceptableInput() & ui->SpeLE1_4->hasAcceptableInput() &
            ui->HPLE2->hasAcceptableInput() & ui->AtkLE2->hasAcceptableInput() & ui->DefLE2->hasAcceptableInput() &
            ui->SpALE2->hasAcceptableInput() & ui->SpDLE2->hasAcceptableInput() & ui->SpeLE2->hasAcceptableInput() &
            ui->HPLE3->hasAcceptableInput() & ui->AtkLE3->hasAcceptableInput() & ui->DefLE3->hasAcceptableInput() &
            ui->SpALE3->hasAcceptableInput() & ui->SpDLE3->hasAcceptableInput() & ui->SpeLE3->hasAcceptableInput();

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
    for (QString inputString : inputData) {
        xoroshiroinverse->waitForFinished(100);
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
    QByteArray output = xoroshiroinverse->readAll();
    QByteArray currBrowser = ui->consoleBrowser->toPlainText().toUtf8();
    currBrowser.append(output);
    ui->consoleBrowser->setText(currBrowser);
}

MainWindow::vldState MainWindow::checkValidity(QString frameIVs, QString frameIVs4)
{
            std::regex re(R"((\d+) (\d+) (\d+) (\d+) (\d+) (\d+))");
            std::string currentIVs = frameIVs.toStdString();
            std::string currentIVs4 = frameIVs4.toStdString();
            std::vector<int> currentIVsMod8 = {-1, -1, -1, -1, -1, -1};
            std::vector<int> currentPerfectIVs = {-1, -1, -1, -1, -1, -1};
            std::vector<std::string> statNames = {"HP", "Atk", "Def", "SpA", "SpD", "Spe"};
            int perfectIVcount = 0;
            int skipAmount = 0;
            int lastReroll = -1;
            bool IVmatchFlag4 = true;
            std::istringstream iss(currentIVs);
            std::vector<int> currentIVnums(std::istream_iterator<int>{iss}, std::istream_iterator<int>());
            for (int i = 0; i < 6; i++) // check for errors
            {
                if (currentIVnums[i] == 31)
                {
                    perfectIVcount++;
                    currentPerfectIVs[i] = 1;
                }
                currentIVsMod8[i] = currentIVnums[i] % 8;
            }
            for (int i = 0; i < 6; i++) // check validity
            {
                if (currentPerfectIVs[i] == 1)
                {
                    continue;
                }
                if ((currentIVsMod8[i] >= 6) || (currentPerfectIVs[currentIVsMod8[i]] == 1))
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
                    if ((currentPerfectIVs[i] == 1) & (QString::fromStdString(currentIVs4).split(" ").at(i) != "31"))
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

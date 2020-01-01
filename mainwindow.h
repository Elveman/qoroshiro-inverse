#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QThread>
#include <QValidator>
#include <QMessageBox>
#include <iterator>
#include <string>
#include <vector>
#include <regex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum vldState {
        Invalid,
        Valid,
        NotValid4IV
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    vldState checkValidity(QString frameIVs, QString frameIVs4);
    Ui::MainWindow *ui;
    QMessageBox errBox;
    QProcess *xoroshiroinverse;
    QString program;
    QStringList arguments;
    QIntValidator IVvalidator;
private slots:
    void processInput();
    void setNewText();
};
#endif // MAINWINDOW_H

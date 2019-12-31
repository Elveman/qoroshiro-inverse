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
    Ui::MainWindow *ui;
    QMessageBox errBox;
    QProcess *xoroshiroinverse;
    QString program;
    QStringList arguments;
    QIntValidator IVvalidator;
private slots:
    void processInput();
};
#endif // MAINWINDOW_H

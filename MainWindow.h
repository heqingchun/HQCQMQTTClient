#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QMessageBox>
#include <QtMqtt/qmqttclient.h>
#include "Dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

private slots:
    void on_btnConnect_clicked();

    void on_btnSubscribe_clicked();

    void on_btnPublish_clicked();

private:
    Ui::CMainWindow *ui;
private:
    QMqttClient *pQMqttClient = nullptr;
};
#endif // CMAINWINDOW_H

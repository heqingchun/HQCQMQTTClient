#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtMqtt/qmqttmessage.h>
#include <QtMqtt/qmqttsubscription.h>

namespace Ui {
class CDialog;
}

class CDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDialog(QMqttSubscription *subscription,QWidget *parent = nullptr);
    ~CDialog();

private:
    Ui::CDialog *ui;
private slots:
    void slotStateChanged(QMqttSubscription::SubscriptionState state);
private:
    QMqttSubscription *p_QMqttSubscription = nullptr;
};

#endif // DIALOG_H

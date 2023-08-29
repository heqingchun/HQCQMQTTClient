#include "Dialog.h"
#include "ui_Dialog.h"

CDialog::CDialog(QMqttSubscription *subscription,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialog)
{
    ui->setupUi(this);
    p_QMqttSubscription = subscription;
    ui->labelSubscription->setText(p_QMqttSubscription->topic().filter());
    ui->labelQoS->setText(QString::number(p_QMqttSubscription->qos()));
    slotStateChanged(p_QMqttSubscription->state());
    connect(p_QMqttSubscription,&QMqttSubscription::messageReceived,this,[&](QMqttMessage msg){
        ui->textEdit->insertPlainText(msg.payload() + '\n');
    });
    connect(p_QMqttSubscription,&QMqttSubscription::qosChanged,[this](quint8 qos){
        ui->labelQoS->setText(QString::number(qos));
    });
    connect(ui->pushButton,&QAbstractButton::clicked,p_QMqttSubscription,&QMqttSubscription::unsubscribe);
}

CDialog::~CDialog(){
    p_QMqttSubscription->unsubscribe();
    if(p_QMqttSubscription) p_QMqttSubscription->deleteLater();
    delete ui;
}
void CDialog::slotStateChanged(QMqttSubscription::SubscriptionState state){
    switch(state){
    case QMqttSubscription::Unsubscribed:
        ui->labelStatus->setText("Unsubscribed");
        break;
    case QMqttSubscription::SubscriptionPending:
        ui->labelStatus->setText("Pending");
        break;
    case QMqttSubscription::Subscribed:
        ui->labelStatus->setText("Subscribed");
        break;
    case QMqttSubscription::Error:
        ui->labelStatus->setText("Error");
        break;
    case QMqttSubscription::UnsubscriptionPending:
        ui->labelStatus->setText("Pending Unsubscription");
        break;
    default:
        ui->labelStatus->setText("--Unknown--");
        break;
    }
}

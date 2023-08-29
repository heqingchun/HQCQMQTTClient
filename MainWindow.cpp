#include "MainWindow.h"
#include "ui_MainWindow.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    pQMqttClient = new QMqttClient();
    connect(pQMqttClient,&QMqttClient::stateChanged,[&](){
        QString tempLog = QString("Log:%1 StateChange:%2").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).arg(pQMqttClient->state());
        ui->textEditSubMsg->insertPlainText(tempLog);
    });
    connect(pQMqttClient,&QMqttClient::disconnected,this,[&](){
        ui->lineEditHost->setEnabled(true);
        ui->lineEditPort->setEnabled(true);
        ui->lineEditUsername->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->btnConnect->setText("开始连接");
    });
    connect(pQMqttClient,&QMqttClient::messageReceived,this,[this](const QByteArray &message,const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString() + " Received Topic: " + topic.name() + " Message: " + message + u'\n';
        ui->textEditSubMsg->insertPlainText(content);
    });
    connect(pQMqttClient,&QMqttClient::pingResponseReceived,this,[this](){
        const QString content = QDateTime::currentDateTime().toString() + " PingResponse\n";
        ui->textEditSubMsg->insertPlainText(content);
    });
}

CMainWindow::~CMainWindow(){
    if(pQMqttClient) pQMqttClient->deleteLater();
    delete ui;
}


void CMainWindow::on_btnConnect_clicked(){
    pQMqttClient->setHostname(ui->lineEditHost->text());
    pQMqttClient->setPort(ui->lineEditPort->text().toUInt());
    pQMqttClient->setUsername(ui->lineEditUsername->text());
    pQMqttClient->setPassword(ui->lineEditPassword->text());

    if(pQMqttClient->state() == QMqttClient::Disconnected) {
        ui->lineEditHost->setEnabled(false);
        ui->lineEditPort->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->btnConnect->setText("断开连接");
        pQMqttClient->connectToHost();
    }else{
        ui->lineEditHost->setEnabled(true);
        ui->lineEditPort->setEnabled(true);
        ui->lineEditUsername->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->btnConnect->setText("开始连接");
        pQMqttClient->disconnectFromHost();
    }
}

void CMainWindow::on_btnSubscribe_clicked(){
    QMqttSubscription *tempQMqttSubscription = pQMqttClient->subscribe(ui->lineEditSubTopic->text(),ui->lineEditSubQoS->text().toUInt());
    if(!tempQMqttSubscription){
        QMessageBox::critical(this,"Error","Could not subscribe. Is there a valid connection?");
        return;
    }
    CDialog *tempDialog = new CDialog(tempQMqttSubscription);
    tempDialog->setWindowTitle(tempQMqttSubscription->topic().filter());
    tempDialog->show();
}

void CMainWindow::on_btnPublish_clicked(){
    if(-1 == pQMqttClient->publish(ui->lineEditPubTopic->text(),
                             ui->lineEditPubMsg->text().toUtf8(),
                             ui->lineEditPubQoS->text().toUInt(),
                             ui->checkBoxRetain->isChecked()))
        QMessageBox::critical(this,"Error","Could not publish message");
}

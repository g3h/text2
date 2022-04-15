#include "login.h"
#include "ui_login.h"
#include"mainwindow.h"
#include<QIcon>
#include<QString>
#include<QNetworkInterface>
#include<QMessageBox>
#include<QtDebug>
#include<QPropertyAnimation>
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("://images/LoginToken.png"));
    this->setWindowTitle("Login");
    //初始化输入值
    initControl();
    getIP();
}

Login::~Login()
{
    delete ui;
}

void Login::initControl(){
    ui->userLineEdit->setPlaceholderText(QStringLiteral("昵称"));
    ui->userLineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->ipComboBox->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->loginPushButton->setStyleSheet("background-color: rgb(255,255,255); border:2px groove gray;border-radius:10px;padding:2px 4px;");
}

QString Login::getIP(){
    QList<QHostAddress>list = QNetworkInterface::allAddresses();
    ui->ipComboBox->clear();
    foreach(QHostAddress address,list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)

            ui->ipComboBox->addItem(address.toString());
            //qDebug()<<address.toString();
    }
    return 0;
}

QString Login::setIP(){
    gottenip = ui->ipComboBox->currentText();
    return  gottenip;
}

void Login::on_loginPushButton_clicked()//登录用户并传入用户相关信息
{
    users.name=ui->userLineEdit->text();
    if(ui->userLineEdit->text()==""){
        QMessageBox::warning(0,tr("错误"),tr("请输入你的用户名（用户名不能为空）"),QMessageBox::Ok);
        return;
    }
    users.hostaddress=setIP();
    users.address=setIP();
    mainwindow = new MainWindow(0,users);
    mainwindow->show();
    this->hide();
}


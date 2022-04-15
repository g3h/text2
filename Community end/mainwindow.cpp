#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesrvdlg.h"
#include "vediosrvdlg.h"
#include"filecntdlg.h"
#include <QMessageBox>
#include<QString>
#include<QtDebug>

MainWindow::MainWindow(QWidget *parent,User user) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUser(user);
    this->setWindowIcon(QIcon("://images/MainToken.png"));
    this->setWindowTitle("欢迎："+user.name+" ("+user.address+")");
    initMainWindow();
    on_serchPushBtn_clicked();
    ui->chatTextEdit->installEventFilter(this);
    ui->sendPushBtn->setShortcut(Qt::Key_Return);
    ui->serchPushBtn->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUser(User u)
{
    user.name = u.name;
    user.address = u.address;
    user.hostaddress = u.hostaddress;
    qDebug() << user.name<<user.hostaddress<<user.address;
}

void MainWindow::initMainWindow(){

    //创建UDP套接字
    myUdpSocket = new QUdpSocket(this);
    //默认端口
    myUdpPort = 23232;
    //qDebug()<<"myUdpPort:45454";
    //绑定端口
    myUdpSocket->bind(myUdpPort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    //将套接字与槽函数绑定
    connect(myUdpSocket,SIGNAL(readyRead()),this,SLOT(recvAndProcessChatMsg()));
    myfsrv=new FileSrvDlg(this);
    connect(myfsrv,SIGNAL(sendFileName(QString)),this,SLOT(getSfileName(QString)));

    connect(ui->chatTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this,SLOT(currentFormatChanged(QTextCharFormat)));
}

void MainWindow::on_serchPushBtn_clicked()
{
    //qDebug()<<"on_serchPushBtn_clicked()";
    sendChatMsg(OnLine,user.name);
}

void MainWindow::sendChatMsg(ChatMsgType msgType, QString rmtName){//发送上线，下线，传输文件，接收文件的消息
    qDebug()<<"sendChatMsg:";
    QByteArray qba;
    QDataStream write(&qba, QIODevice::WriteOnly);
    QString locHostIp = user.address;
    QString locChatMsg = getChatMsg();

    write<<msgType<<rmtName;

    switch (msgType) {
     case ChatMsg:
        write<<locHostIp<<locChatMsg;
        break;
     case OnLine:
        //qDebug()<<"Online"<<rmtName;
        write<<locHostIp;
        break;
     case OffLine:
        break;
    case SFileName:
        write<<locHostIp<<rmtName<<myFileName;
        break;
    case ReFile:
        write<<locHostIp<<rmtName;
        break;
    }
   myUdpSocket->writeDatagram(qba, qba.length(),QHostAddress::Broadcast,myUdpPort);
}

void MainWindow::recvAndProcessChatMsg(){
    while (myUdpSocket->hasPendingDatagrams()) {//接收上线，下线，传输文件，接收文件的消息
        QByteArray qba;
        qba.resize(myUdpSocket->pendingDatagramSize());
        myUdpSocket->readDatagram(qba.data(),qba.size());
        QDataStream read(&qba,QIODevice::ReadOnly);
        int msgType;
        read>>msgType;
        QString name, hostip, chatmsg, rname, fname;
        QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (msgType) {
        case ChatMsg:{
            read>>name>>hostip>>chatmsg;
            qDebug()<<"Chat:"<<name;
            ui->chatTextBrowser->setTextColor(Qt::darkGray);
            ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 14));
            ui->chatTextBrowser->append("【"+name+"】"+curtime);
            ui->chatTextBrowser->append(chatmsg);
            break;
        }
        case OnLine:
            read>>name>>hostip;
            qDebug()<<"Online:"<<name;
            onLine(name, curtime,hostip);
            break;
        case OffLine:
            read>>name;
            offLine(name,curtime);
            break;
        case SFileName:
            read>>name>>hostip>>rname>>fname;
            recvFileName(name,hostip,rname,fname);
            break;
        case ReFile:
            read>>name>>hostip>>rname;
            if(user.name==rname)myfsrv->cntRefused();
            break;
        }
    }
}

void MainWindow::onLine(QString name, QString time, QString Address){
    bool notExist = ui->userListTableWidget->findItems(name,Qt::MatchExactly).isEmpty();
    if(notExist){
        QTableWidgetItem*newuserName=new QTableWidgetItem(name);
        QTableWidgetItem*newuserAdd=new QTableWidgetItem(Address);
        ui->userListTableWidget->insertRow(0);
        ui->userListTableWidget->setItem(0,0,newuserName);
        ui->userListTableWidget->setItem(0,1,newuserAdd);
        ui->chatTextBrowser->setTextColor(Qt::darkGray);
        ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
        ui->chatTextBrowser->append(tr("%1 %2 上线").arg(time).arg(name));
        sendChatMsg(OnLine,name);
        on_serchPushBtn_clicked();
        ui->numLabel->setText(tr("在线人数： %1").arg(ui->userListTableWidget->rowCount()));
    }
}

void MainWindow::offLine(QString name, QString time){
    int row=ui->userListTableWidget->findItems(name,Qt::MatchExactly).first()->row();
    ui->userListTableWidget->removeRow(row);
    ui->chatTextBrowser->setTextColor(Qt::darkGray);
    ui->chatTextBrowser->setCurrentFont(QFont("Times New Roman", 12));
    ui->chatTextBrowser->append(tr("%1 %2 离线").arg(time).arg(name));
    ui->numLabel->setText(tr("在线人数： %1").arg(ui->userListTableWidget->rowCount()));
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("关闭窗口\n"),
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        sendChatMsg(OffLine,user.name);
        event->accept();
    }
}

QString MainWindow::getChatMsg(){
    QString chatmsg=ui->chatTextEdit->toHtml();
    ui->chatTextEdit->clear();
    ui->chatTextEdit->setFocus();
    return chatmsg;
}

void MainWindow::on_sendPushBtn_clicked()
{
    if(ui->chatTextEdit->document()->isEmpty()){
        QMessageBox::warning(0,tr("错误"),tr("请勿发送空信息"),QMessageBox::Ok);
    }else{
        sendChatMsg(ChatMsg,user.name);
    }
}

void MainWindow::on_shareBtn_clicked()
{
    if(ui->userListTableWidget->selectedItems().isEmpty()){
        QMessageBox::warning(0,tr("选择好友"),tr("请先选择文件接收方。"),QMessageBox::Ok);
        return;
    }
    myfsrv->show();
}

void MainWindow::getSfileName(QString fname){
    myFileName=fname;
    qDebug()<<myFileName;
    int row=ui->userListTableWidget->currentRow();
    QString rmtName=ui->userListTableWidget->item(row,0)->text();
    sendChatMsg(SFileName,rmtName);
}

void MainWindow::recvFileName(QString name, QString hostip, QString rmtname, QString filename){
    if(user.name==rmtname){
        int result=QMessageBox::information(this,tr("收到文件"),tr("好友%1向您发送文件：\r\n%2，是否接收？") .arg(name).arg(filename),QMessageBox::Yes,QMessageBox::No);
        if(result==QMessageBox::Yes){
            QString fname=QFileDialog::getSaveFileName(0,tr("保存"),filename);
            if(!fname.isEmpty()){
                FileCntDlg*fcnt=new FileCntDlg(this);
                fcnt->getLocPath(fname);
                fcnt->getSrvAddr(QHostAddress(hostip));
                fcnt->show();
            }
        }else {
            sendChatMsg(ReFile,name);
        }
    }
}

void MainWindow::on_callBtn_clicked()
{
    if(ui->userListTableWidget->selectedItems().isEmpty()){
        QMessageBox::warning(0,tr("选择好友"),tr("请先选择通话方。"),QMessageBox::Ok);
        return;
    }
    int row=ui->userListTableWidget->currentRow();
    QString vedName=ui->userListTableWidget->item(row,1)->text();
    addip.ip=vedName;
    myvrsv = new VedioSrvDlg(0,addip);
    myvrsv->show();
}

void MainWindow::on_bodeBtn_clicked(bool checked)
{
    if(checked){
        ui->chatTextEdit->setFontWeight(QFont::Bold);
    }
    else {
        ui->chatTextEdit->setFontWeight(QFont::Normal);
   }
    ui->chatTextEdit->setFocus();
}

void MainWindow::on_ItalicBtn_clicked(bool checked)
{
    ui->chatTextEdit->setFontItalic(checked);
    ui->chatTextEdit->setFocus();
}

void MainWindow::on_underlineBtn_clicked(bool checked)
{
    ui->chatTextEdit->setFontUnderline(checked);
    ui->chatTextEdit->setFocus();
}

void MainWindow::on_fontSizeBox_currentIndexChanged(const QString &arg1)
{
    ui->chatTextEdit->setFontPointSize(arg1.toDouble());
    ui->chatTextEdit->setFocus();
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->chatTextEdit->setCurrentFont(f);
    ui->chatTextEdit->setFocus();
}

void MainWindow::currentFormatChanged(const QTextCharFormat &format)
{
    ui->fontComboBox->setCurrentFont(format.font());
    if(format.fontPointSize() < 9) {
        ui->fontSizeBox->setCurrentIndex(3);
    }
    else {
        ui->fontSizeBox->setCurrentIndex(ui->fontSizeBox
                                          ->findText(QString::number(format.fontPointSize())));
    }
    ui->bodeBtn->setChecked(format.font().bold());
    ui->ItalicBtn->setChecked(format.font().italic());
    ui->underlineBtn->setChecked(format.font().underline());
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)//实现绑定回车键发送消息
{
    if(target == ui->chatTextEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 //on_serchPushBtn_clicked();
                 on_sendPushBtn_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void MainWindow::on_exitPushBtn_clicked()
{
    sendChatMsg(OffLine,user.name);
    close();

}

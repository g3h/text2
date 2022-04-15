#include "vediosrvdlg.h"
#include "ui_vediosrvdlg.h"
#include <QTime>
#include <QLCDNumber>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

VedioSrvDlg::VedioSrvDlg(QWidget *parent,AddIp addip)://) :
    QDialog(parent),
    ui(new Ui::VedioSrvDlg)
{
    ui->setupUi(this);
  qDebug()<<"通话IP："<<addip.ip;
    Cip=addip.ip;

    MyIp=GetIpAddress();
    timer1 =new QTimer();
    connect(timer1,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    ui->timeNumber->setDigitCount(12);//设置LCD时钟位数精确毫秒
    //ui->timeNumber->setDigitCount(8);//设置LCD时钟位数精确秒

    //开始通话
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    //timer->start(3000);
    udpSocket= new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any,10004);//绑定10004端口

    //加一个发送 同意则 否则关闭
    myAudioRec =new AudioRecThread();
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(RecreadyReadSlot()));
    //收到网络数据报就开始往outputDevice写入，进行播放
    myAudioRec->start();
    myAudioRec->Pause();

}

VedioSrvDlg::~VedioSrvDlg()
{
    delete ui;
}

void VedioSrvDlg::onTimeout()//向局域网中其他设备广播自己地址
{
   QString msg="{\"IP\":"+MyIp+"}";
   udpSocket->writeDatagram(msg.toUtf8(),QHostAddress("255.255.255.255"),10004);
}

QString VedioSrvDlg::GetIpAddress()//获取本地IP
{

    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
       int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
   qDebug()<<"本机IP:"<<strIpAddress;
     return strIpAddress;
}
void VedioSrvDlg::MicroPhoneonReadyRead()//传播音频
{
  //qDebug()<<"It's sending audio!"<<endl;
    video vp;
    memset(&vp,0,sizeof(vp));
    //读取音频
    vp.lens =myAudio->inputDevice->read(vp.audiodata,1024);//读取音频
    QHostAddress destaddr;
    destaddr.setAddress(Cip);
    //qDebug()<<Cip;
    //将此结构体发送给目标主机，端口是10004
    udpSocket->writeDatagram((const char*)&vp,sizeof(vp),destaddr,10004);
}

void VedioSrvDlg::RecreadyReadSlot()//Udp接受到数据，播放音频
{
    //qDebug()<<Cip<<"and"<<MyIp;
        myAudioRec->Resume();
        while(udpSocket->hasPendingDatagrams())
        {
            QHostAddress senderip;
            quint16 senderport;
          //qDebug()<<"audio is being received...";//<<endl;
            video vp;
            memset(&vp,0,sizeof(vp));
            udpSocket->readDatagram((char*)&vp,sizeof(vp),&senderip,&senderport);
            myAudioRec->outputDevice->write(vp.audiodata,vp.lens);
        }
        myAudioRec->Pause();

}

void VedioSrvDlg::on_cutButton_clicked()//挂断电话并结束通话
{
    QString str = count.toString("hh:mm:ss.zzz");
    qDebug()<<str;
    if(str==""){
        this->hide();
        return;
    }
    myAudio->Pause();
    Cip="";
    this->hide();
    ui->callButton->setText("发起会话");
}

void VedioSrvDlg::on_callButton_clicked()//拨打电话，传入数据
{
    count.setHMS(0,0,0,0);
    timer1->start(1);
    //timer1->start(1000);
    //count.setHMS(0,0,0);

    myAudio= new AudioThread();
    connect(myAudio->inputDevice,SIGNAL(readyRead()),this,SLOT(MicroPhoneonReadyRead()));
  //槽函数，当inputDevice收到input写入的音频数据之后,调用onReadyRead函数，发送数据到目标主机上
    myAudio->start();
    ui->callButton->setText("会话中");
 // myAudio->Pause();
}

void VedioSrvDlg::timerUpDate(){//实现时钟的更新
    count = count.addMSecs(1);
    QString str = count.toString("hh:mm:ss.zzz");
    //count = count.addSecs(1);
    //QString str = count.toString("hh:mm:ss");
    ui->timeNumber->display(str);

}

void VedioSrvDlg::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("关闭通话窗口\n"),
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
        on_cutButton_clicked();
    }
}

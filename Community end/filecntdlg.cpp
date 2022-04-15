#include "filecntdlg.h"
#include "ui_filecntdlg.h"

FileCntDlg::FileCntDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileCntDlg)
{
    ui->setupUi(this);
   myCntSocket=new QTcpSocket(this);
    initCnt();
}

FileCntDlg::~FileCntDlg()
{
    delete ui;
}

void FileCntDlg::initCnt(){

    mySrvPort=5555;
    connect(myCntSocket,SIGNAL(readyRead()),this,SLOT(readChatMsg()));
    myFileNameSize=0;
    myTotalBytes=0;
    myRcvedBytes=0;
}

void FileCntDlg::newConnect(){
    myBlockSize=0;
    myCntSocket->abort();
    myCntSocket->connectToHost(mySrvAddr,mySrvPort);
    mytime.start();
}

void FileCntDlg::readChatMsg(){
    QDataStream in(myCntSocket);
    in.setVersion(QDataStream::Qt_5_12);
    float usedTime=mytime.elapsed();
    if(myRcvedBytes<=sizeof (qint64)*2){
        if((myCntSocket->bytesAvailable()>=sizeof (qint64)*2)&&(myFileNameSize==0)){
            in>>myTotalBytes>>myFileNameSize;
            myRcvedBytes+=sizeof (qint64)*2;
        }
        if((myCntSocket->bytesAvailable()>=myFileNameSize)&&(myFileNameSize!=0)){
            in>>myFileName;
            myRcvedBytes+=myFileNameSize;
            myLocPathFile->open(QFile::WriteOnly);
            ui->rfileNameLineEdit->setText(myFileName);
        }else{
            return;
        }
    }
    if(myRcvedBytes<myTotalBytes){
        myRcvedBytes+=myCntSocket->bytesAvailable();
        myInputBlock=myCntSocket->readAll();
        myLocPathFile->write(myInputBlock);
        myInputBlock.resize(0);
    }
    ui->recvProgressBar->setMaximum(myTotalBytes);
    ui->recvProgressBar->setValue(myRcvedBytes);
    double transpeed=myRcvedBytes/usedTime;
    ui->rfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024))+"MB");
    ui->recvSizeLineEdit->setText(tr("%1").arg(myRcvedBytes/(1024*1024))+"MB");
    if(myRcvedBytes==myTotalBytes){
        myLocPathFile->close();
        myCntSocket->close();
    }
}

void FileCntDlg::on_cntClosePushButton_clicked()
{
    initCnt();
    myCntSocket->abort();
    myLocPathFile->close();
    close();
}

void FileCntDlg::getLocPath(QString lpath){
    myLocPathFile=new QFile(lpath);
}

void FileCntDlg::getSrvAddr(QHostAddress saddr){
    mySrvAddr=saddr;
    newConnect();
}


void FileCntDlg::closeEvent(QCloseEvent *){

    on_cntClosePushButton_clicked();
}


#include "filesrvdlg.h"
#include "ui_filesrvdlg.h"
#include<QObject>

FileSrvDlg::FileSrvDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSrvDlg)
{
    ui->setupUi(this);

    myTcpSrv=new QTcpServer(this);
    mySrvPort=5555;
    qDebug()<<1<<mySrvPort;
    connect(myTcpSrv,SIGNAL(newConnection()),this,SLOT(sndChatMsg()));
    initServer();
}

FileSrvDlg::~FileSrvDlg()
{
    delete ui;
}

void FileSrvDlg::initServer()
{
    myTcpSrv->close();
    myTotalBytes=0;
    mySendBytes=0;
    myBytesTobeSend=0;
    myPayloadSize=64*1024;
    ui->sendProgressBar->reset();
    ui->openFilePushButton->setEnabled(true);
    ui->sendFilePushButton->setEnabled(false);
}

void FileSrvDlg::sndChatMsg()
{
    ui->sendFilePushButton->setEnabled(false);
    mySrvSocket=myTcpSrv->nextPendingConnection();
    connect(mySrvSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(refreshProgress(qint64)));
    myLocPathFile=new QFile(myPathFile);
    myLocPathFile->open(QFile::ReadOnly);
    myTotalBytes=myLocPathFile->size();
    QDataStream sendOut(&myOutputBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_12);
    mytime.start();
    QString curFile=myPathFile.right(myPathFile.size()-myPathFile.lastIndexOf('/')-1);
    sendOut<<qint64(0)<<qint64(0)<<curFile;
    myTotalBytes+=myOutputBlock.size();
    sendOut.device()->seek(0);
    sendOut<<myTotalBytes<<qint64((myOutputBlock.size()-sizeof (qint64)*2));
    myBytesTobeSend=myTotalBytes-mySrvSocket->write(myOutputBlock);
    myOutputBlock.resize(0);
}

void FileSrvDlg::refreshProgress(qint64 bynum){
    qApp->processEvents();
    mySendBytes+=(int)bynum;
    if(myBytesTobeSend>0){
        myOutputBlock=myLocPathFile->read(qMin(myBytesTobeSend,myPayloadSize));
        myBytesTobeSend-=(int)mySrvSocket->write(myOutputBlock);
        myOutputBlock.resize(0);
    }else {
        myLocPathFile->close();
    }
    ui->sendProgressBar->setMaximum(myTotalBytes);
    ui->sendProgressBar->setValue(mySendBytes);
    ui->sfileSizeLineEdit->setText(tr("%1").arg(myTotalBytes/(1024*1024))+"MB");
    ui->sendSizeLineEdit->setText(tr("%1").arg(mySendBytes/(1024*1024))+"MB");

    if(mySendBytes==myTotalBytes){
        myLocPathFile->close();
        myTcpSrv->close();
        QMessageBox::information(0,QObject::tr("??????"),"??????????????????");
    }
}
void FileSrvDlg::on_openFilePushButton_clicked()
{
    myPathFile=QFileDialog::getOpenFileName(this);
    if(!myPathFile.isEmpty()){
        myFileName=myPathFile.right(myPathFile.size()-myPathFile.lastIndexOf('/')-1);
        ui->sfileNameLineEdit->setText(tr("%1").arg(myFileName));
        ui->sendFilePushButton->setEnabled(true);
        ui->openFilePushButton->setEnabled(false);
    }
}

void FileSrvDlg::on_sendFilePushButton_clicked()
{
    ui->sendFilePushButton->setText("????????????");
    qDebug()<<myTcpSrv;
    if(!myTcpSrv->listen(QHostAddress::Any, mySrvPort)){
        QMessageBox::warning(0,QObject::tr("??????"),"??????TCP???????????????????????????????????????");
        close();
        return;
    }
    emit sendFileName(myFileName);

}

void FileSrvDlg::on_srvClosePushButton_clicked()
{
    if(myTcpSrv->isListening()){
        myTcpSrv->close();
        myLocPathFile->close();
        mySrvSocket->abort();
    }
    close();
}

void FileSrvDlg::closeEvent(QCloseEvent *event){
    initServer();
    event->accept();
}

void FileSrvDlg::cntRefused(){
    if(myTcpSrv->isListening()){
        qDebug()<<"????????????";
    }
    myTcpSrv->close();
    if(myTcpSrv->isListening()){
        qDebug()<<"????????????2";
    }
    QMessageBox::warning(0,QObject::tr("??????"),"???????????????");

}


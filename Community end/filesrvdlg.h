#ifndef FILESRVDLG_H
#define FILESRVDLG_H

#include <QDialog>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QTime>
#include<QTcpServer>
#include<QTcpSocket>
#include<QCloseEvent>

class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class FileSrvDlg;
}

class FileSrvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSrvDlg(QWidget *parent = nullptr);
    ~FileSrvDlg();
    //初始化TCPServer
    void initServer();
    //客户端拒绝后的解决方案
    void cntRefused();


protected:
    void closeEvent(QCloseEvent *);

private slots:
    //发送消息方法
    void sndChatMsg();
    //刷新服务器进度条方法
    void refreshProgress(qint64 bynum);

    void on_openFilePushButton_clicked();

    void on_sendFilePushButton_clicked();

    void on_srvClosePushButton_clicked();

private:
    Ui::FileSrvDlg *ui;
    //TCP服务器对象指针
    QTcpServer*myTcpSrv;
    //TCP服务器套接口指针
    QTcpSocket*mySrvSocket;
    qint16 mySrvPort;

    //文件对象指针
    QFile *myLocPathFile;
    //含路径文件名
    QString myPathFile;
    //不含路径文件名
    QString myFileName;

    qint64 myTotalBytes;
    qint64 mySendBytes;
    qint64 myBytesTobeSend;
    qint64 myPayloadSize;
    QByteArray myOutputBlock;
    QTime mytime;

signals:
    void sendFileName(QString name);
};

#endif // FILESRVDLG_H

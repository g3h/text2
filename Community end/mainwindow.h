#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "vediosrvdlg.h"
#include "filesrvdlg.h"
#include<QWidget>
#include<QtNetwork/QUdpSocket>
#include<QNetworkInterface>
#include<QDateTime>
#include<QFile>
#include<QFileDialog>
#include<QCloseEvent>
#include <QTextCharFormat>
#include<QKeyEvent>

namespace Ui {
class MainWindow;
}

struct User{
    QString name;
    QString hostaddress;
    QString address;
};


enum ChatMsgType{ ChatMsg, OnLine, OffLine, SFileName, ReFile,SVedioName, ReVedio};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,User u = {"","",""});
    ~MainWindow();

    //设置用户
    void setUser(User u);
    //窗口初始化
    void initMainWindow();
    //用户上线
    void onLine(QString name, QString time,QString Address);
    //用户离线
    void offLine(QString name, QString time);
    //发送UDP消息
    void sendChatMsg(ChatMsgType msgType, QString rmtName="");
    //获取IP地址
    QString getIP();
    //获取聊天信息
    QString getChatMsg();
    //获取用户名
    QString getUserName();
    //接收文档
    void recvFileName(QString name, QString hostip, QString rmtname,QString filename);


protected:
    void closeEvent(QCloseEvent* event);

    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

private slots:
    void recvAndProcessChatMsg();

    void getSfileName(QString);

    void on_serchPushBtn_clicked();

    void on_sendPushBtn_clicked();

    void on_shareBtn_clicked();

    void on_callBtn_clicked();


    void on_bodeBtn_clicked(bool checked);

    void on_ItalicBtn_clicked(bool checked);

    void on_underlineBtn_clicked(bool checked);

    void on_fontSizeBox_currentIndexChanged(const QString &arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

     void currentFormatChanged(const QTextCharFormat &format);

     void on_exitPushBtn_clicked();

private:
    Ui::MainWindow *ui;
    User user;
    QUdpSocket*myUdpSocket;
    qint16 myUdpPort;
    QString myFileName;
    FileSrvDlg*myfsrv;

    QUdpSocket*udpSocket;//用于音频信号的发送
    VedioSrvDlg*myvrsv;
    AddIp addip;

};

#endif // MAINWINDOW_H


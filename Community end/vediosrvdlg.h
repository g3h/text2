#ifndef VEDIOSRVDLG_H
#define VEDIOSRVDLG_H
#include <QtNetwork/QUdpSocket>
#include <QNetworkInterface>
#include <QDialog>
#include <QTimer>
#include <QTime>

#include "audiothread.h"
#include "audiorecthread.h"
#include <QAudio>//一下这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

namespace Ui {
class VedioSrvDlg;
}
class QTcpServer;
class QTcpSocket;

struct AddIp{
    QString ip;
};
class VedioSrvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit VedioSrvDlg(QWidget *parent = nullptr,AddIp addip = {""});//);
    ~VedioSrvDlg();
    VedioSrvDlg(QObject *parent=0);
    QString GetIpAddress();
    QString Cip;
    QString MyIp;

private slots:
    void on_cutButton_clicked();
    void on_callButton_clicked();
    void timerUpDate();
    void MicroPhoneonReadyRead();
    void RecreadyReadSlot();
    void onTimeout();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::VedioSrvDlg *ui;
    QTime count;
    QTimer* timer;
    QTimer* timer1;
    QUdpSocket *udpSocket;//用于音频信号的发送
    AudioThread *myAudio;
    AudioRecThread *myAudioRec;
    AddIp addip;

};

#endif // VEDIOSRVDLG_H

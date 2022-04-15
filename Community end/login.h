#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QString>
#include<mainwindow.h>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    //初始化控件
    void initControl();
    //获取ip地址
    QString getIP();
    QString setIP();
    void closePlay();

private slots:
    void on_loginPushButton_clicked();

private:
    Ui::Login *ui;
    User users;    
    MainWindow *mainwindow;
    QString gottenip;
};


#endif // LOGIN_H

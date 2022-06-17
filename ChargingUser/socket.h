#ifndef SOCKET_H
#define SOCKET_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>

#define SERVER_IP "127.0.0.1" //服务器IP地址
#define SERVER_PORT 8080 //服务器端口号
#define BUF_LEN 1024 //数据包缓存区大小

namespace Ui {
class Socket;
}

class Socket : public QWidget
{
    Q_OBJECT

public:
    bool connected;

    explicit Socket(QWidget *parent = nullptr);
    ~Socket();
    static Socket& Instance();
    bool SendRequest(QString request); //发送数据

signals:
    void pass_login_recv(QString recv); // 登录回复
    void pass_logon_recv(QString recv); // 注册回复
    void pass_state_recv(QString recv); // 回复状态
    void pass_ahead_recv(QString recv); // 回复前车数量
    void pass_queue_recv(QString recv); // 回复排队号
    void pass_detail_recv(QString recv); // 回复详单
    void pass_bill_recv(QString recv);   // 回复应付金额
    void socket_disconnect();

private:
    Ui::Socket *ui;
    QTcpSocket* server_socket; //服务器套接字

    bool StartConnection(); //建立连接
    void Disconnect(); // 拆除连接
    void RecvResponce(); //接收数据
    void SendSignal(QString recv); // 向处理函数发送信号
    void MessageShow(QString str); //打印调试信息
};

#endif // SOCKET_H

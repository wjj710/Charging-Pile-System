#include "socket.h"
#include "ui_socket.h"

Socket::Socket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Socket)
{
    ui->setupUi(this);
    connected = StartConnection();
}

Socket::~Socket()
{
    delete ui;
    if (server_socket != NULL)
    {
        server_socket->disconnectFromHost();
        server_socket = NULL;
    }
}

Socket& Socket::Instance()
{
    static Socket sock;
    return sock;
}

bool Socket::StartConnection()
{
    server_socket = new QTcpSocket();
    server_socket->connectToHost(SERVER_IP, SERVER_PORT);
    if (!server_socket->waitForConnected(10000)) {
        MessageShow("client:与服务器连接失败");
        return false;
    } else {
        qDebug() << "与服务器连接成功";
        connect(server_socket, &QTcpSocket::readyRead, this, &Socket::RecvResponce);
        connect(server_socket, &QTcpSocket::disconnected, this, &Socket::Disconnect);
        return true;
    }
}

void Socket::Disconnect()
{
    server_socket->disconnectFromHost();
    server_socket = NULL;
}

void Socket::MessageShow(QString str)
{
    //QMessageBox::information(this, "QT网络通信", str);
    qDebug() << str;
}

void Socket::RecvResponce()
{
    char buffer[BUF_LEN] = {0};
    int recv_succ = server_socket->read(buffer, BUF_LEN);
    if (recv_succ < 0) {
        qDebug() << "数据接收错误";
        return;
    } else {
        QString packet_info = buffer;
        qDebug() << "从服务器数据接收成功:" << packet_info;
        SendSignal(packet_info);
    }
}

void Socket::SendSignal(QString recv)
{
    QStringList packet_item = recv.split('\t');
    for (int i = 0; i < packet_item.length()-1; i++)
    {
        QString resp_packet = packet_item.at(i);
        QStringList recv_item = resp_packet.split('/');
        QString type = recv_item.at(0);
        QString responce = resp_packet.replace(type+"/", "");
        if (type == "login") // 向login请求回信
            emit pass_login_recv(responce);
        else if (type == "logon") // 向logon请求回信
            emit pass_logon_recv(responce);
        else if (type == "state") // 向state请求回信
            emit pass_state_recv(responce);
        else if (type == "getAheadNum") // 向查看前车等待数量回信
            emit pass_ahead_recv(responce);
        else if (type == "getQueueNum") // 向查看排队号回信
            emit pass_queue_recv(responce);
        else if (type == "getDetail") // 向查看详单回信
            emit pass_detail_recv(responce);
        else if (type == "bill") // 向bill请求回信
            emit pass_bill_recv(responce);
    }
}

bool Socket::SendRequest(QString request)
{
    request += '\t';
    char buffer[BUF_LEN] = {0};
    strcpy_s(buffer, request.toStdString().c_str());

    if (server_socket->isValid()) {
        int send_succ = server_socket->write(buffer, strlen(buffer));
        server_socket->flush();
        if (send_succ <= 0) {
            MessageShow("向服务器发送数据失败");
            return false;
        } else {
            qDebug() << "向服务器发送数据成功:" << request;
            return true;
        }
    } else {
        MessageShow("服务器套接字无效");
        return false;
    }
}

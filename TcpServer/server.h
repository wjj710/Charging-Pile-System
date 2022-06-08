#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include <QList>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0, int port = 0);
protected:
    void incomingConnection(qintptr socketDescriptor);//只要出现一个新的连接，就会自动调用这个函数
protected slots:
    void slotshow(QString, QHostAddress,int,bool);//用来处理tcpclient发过来的信号
    void slotclientdisconnect(int); //用来处理客户端断开连接
    void slotsend(QString,int); //用来处理tcpsocket发来的报文，并把请求加入处理队列
public slots:
    void init(int fastPileNum, int tricklePileNum, int waitingSize, int chargingQueueLen);
signals:
    void loopquit();
    void showserver(QString, QHostAddress,int,bool);//发送信号给界面，让界面更新信息
};

#endif // SERVER_H

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include <QList>
#include "pilecontroller.h"
#include "pileinfocontroller.h"
#include "querycontroller.h"
#include "requestcontroller.h"
#include "usercontroller.h"
//using namespace std;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    PileController pileController;
    PileInfoController pileInfoController;
    QueryController queryController;
    RequestController requestController;
    UserController userController;

    Server(QObject *parent = 0, int port = 0);
    void sendMsg(QString msg, int descriptor); //向指定描述符对应的socket发消息
protected:
    void incomingConnection(int socketDescriptor);//只要出现一个新的连接，就会自动调用这个函数
protected slots:
    void slotshow(QString, QHostAddress,int,bool);//用来处理tcpclient发过来的信号
    void slotclientdisconnect(int); //用来处理客户端断开连接
    void slotsend(QString,int); //用来处理tcpsocket发来的报文，并把请求加入处理队列
public slots:
    void init(int fastPileNum, int tricklePileNum, int waitingSize, int chargingQueueLen);
signals:
    void showserver(QString, QHostAddress,int,bool);//发送信号给界面，让界面更新信息
};

#endif // SERVER_H

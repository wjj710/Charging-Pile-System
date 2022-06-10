#include "server.h"
#include <QHostAddress>
#include <QDebug>
#include "global.h"
#include <stdio.h>
#include <QProcess>
using namespace std;

Server::Server(QObject *parent, int port):QTcpServer(parent)
{
    listen(QHostAddress::Any, port); //监听
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    //只要有新的连接就生成一个新的通信套接字
    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    tcpclientsocket->setSocketDescriptor(socketDescriptor);

    //将这个套接字加入客户端套接字列表中
    Global::tcpclientsocketlist.append(tcpclientsocket);


    //接收到tcpclientsocket发送过来的更新界面的信号
    connect(tcpclientsocket, &TcpClientSocket::showserver, this, &Server::slotshow);
    connect(tcpclientsocket, &TcpClientSocket::clientdisconnected, this, &Server::slotclientdisconnect);
    connect(tcpclientsocket, &TcpClientSocket::sendserver, this, &Server::slotsend);
}

void Server::slotshow(QString msg, QHostAddress addr,int port,bool send)
{
    //将这个信号发送给界面
    emit showserver(msg,addr,port,send);
}

void Server::slotsend(QString msg,int descriptor){

    QStringList msgList = msg.split("/");
    msgList.append(QString::number(descriptor));
    if(msgList[0]=="yes" || msgList[0]=="no"){ //说明不是请求，是充电桩的返回消息
        Global::buffer=msgList; //将收到的消息放到缓冲区
        //emit loopquit(); //向处理线程发信号，通知loop退出
        Global::condition.wakeAll();
    }else{
        Global::handleList.append(msgList);
    }
}

void Server::slotclientdisconnect(int descriptor)
{
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            Global::tcpclientsocketlist.removeAt(i);//如果有客户端断开连接， 就将列表中的套接字删除
            return;
        }
    }
    return;
}

void Server::init(int fastPileNum, int tricklePileNum, int waitingSize, int chargingQueueLen)
{
    //初始化全局变量
    Global::fastChargingPileNum=fastPileNum;
    Global::trickleChargingPileNum=tricklePileNum;
    Global::waitingAreaSize=waitingSize;
    Global::chargingQueueLen=chargingQueueLen;

    int power=30; //表示充电桩功率
    //创建快充电桩(注意，充电桩编号和排队号都从1开始）
    for(int i=1; i<=fastPileNum; i++){
        string s=string("pileproc")+string(" F")+to_string(i)+string(" ")
                +to_string(power)+string(" ")+to_string(chargingQueueLen)+string("\0");
        QProcess *process=new QProcess;
        process->startCommand(QString::fromStdString(s));
        qDebug()<<"create process "<<process->processId()<<Qt::endl;
        Global::proclist.append(process);
        Global::lp.append("F"+to_string(i));
    }

    power=10;
    //创建慢充电桩
    for(int i=1; i<=tricklePileNum; i++){
        string s=string("pileproc")+string(" T")+to_string(i)+string(" ")
                +to_string(power)+string(" ")+to_string(chargingQueueLen)+string("\0");
        QProcess *process=new QProcess;
        process->startCommand(QString::fromStdString(s));
        qDebug()<<"create process "<<process->processId()<<Qt::endl;
        Global::proclist.append(process);
        Global::lp.append("T"+to_string(i));
    }
}

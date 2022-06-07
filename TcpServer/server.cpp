#include "server.h"
#include <QHostAddress>
#include "global.h"
#include <tchar.h>
#include <windows.h>
using namespace std;

Server::Server(QObject *parent, int port):QTcpServer(parent)
{
    listen(QHostAddress::Any, port); //监听
}

void Server::sendMsg(QString msg, qintptr descriptor)
{
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toUtf8().data());
            emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
            return;
        }
    }
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
        Global::loop.quit(); //使pileController中的事件循环退出
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
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        string s=string("pileproc")+string(" F")+to_string(i)+string(" ")
                +to_string(power)+string(" ")+to_string(chargingQueueLen)+string("\0");
        LPWSTR command = (LPWSTR)s.c_str();
        CreateProcess(NULL,
                command,
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi
         );
        Global::lp.append("F"+to_string(i));
    }

    power=10;
    //创建慢充电桩
    for(int i=1; i<=tricklePileNum; i++){
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        string s=string("pileproc")+string(" T")+to_string(i)+string(" ")
                +to_string(power)+string(" ")+to_string(chargingQueueLen)+string("\0");
        LPWSTR command = (LPWSTR)s.c_str();
        CreateProcess(NULL,
                command,
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi
         );
        Global::lp.append("T"+to_string(i));
    }

    //进入请求处理循环
    while(1){
        if(Global::handleList.size()){ //如果处理队列不空，就从中取出一个进行处理
            QStringList msgList = Global::handleList.at(0); //待处理的请求
            Global::handleList.removeAt(0); //在处理队列中删除它
            QString ans=""; // 回复数据包
            int descriptor = msgList[msgList.size()-1].toInt(); //从请求中分离出socket描述符
            msgList.removeLast(); //分离之后将socket描述符删除

            if(msgList[0]=="logon"){
                //ans=userController.logon(msgList[1], msgList[2]);
                //注意用户登录时要创建用户类，加入用户队列usr并更新mstr2Int和mint2Str
            }else if (msgList[0]=="register") {
                //ans=userController.register(msgList[1], msgList[2]);
            }else if(msgList[0]=="pay"){
                //ans=userController.pay(msgList[1]);
                //详见消息格式文档
            }else if (msgList[0]=="startRequest") {
                ;
            }else if (msgList[0]=="changeRequest") {
                ;
            }else if (msgList[0]=="endRequest") {
                ;
            }else if (msgList[0]=="getQueueNum") {
                try {
                    User &tmpUsr = QueryController::getUserByID(QString::fromStdString(Global::mint2Str[descriptor]));
                    ans = "yes/" + QueryController::getQueueNum(tmpUsr) + "\t";
                } catch (const char *msg) {
                    ans = "no/" + QString::fromStdString(msg) + "\t";
                }
            }else if (msgList[0]=="getAheadNum") {
                try {
                    User &tmpUsr = QueryController::getUserByID(QString::fromStdString(Global::mint2Str[descriptor]));
                    ans = "yes/" + QString::fromStdString(std::to_string(QueryController::getAheadNum(tmpUsr))) + "\t";
                } catch (const char *msg) {
                    ans = "no/" + QString::fromStdString(msg) + "\t";
                }
            }else if (msgList[0]=="getDetail") {
                try {
                    User &tmpUsr = QueryController::getUserByID(QString::fromStdString(Global::mint2Str[descriptor]));
                    Detail ret = QueryController::getDetail(tmpUsr);
                    ans = "yes/" + QString::fromStdString(std::to_string(ret.detailNo)) + "\t";
                    // ans = "yes/" + Detail2Stream(ret) + "\t"; // TODO: Detail -> TCPStream
                } catch (const char *msg) {
                    ans = "no/" + QString::fromStdString(msg) + "\t";
                }
                ;
            }else if (msgList[0]=="turnOnPile") {
                ;
            }else if (msgList[0]=="turnOffPile") {
                ;
            }else if (msgList[0]=="getPileInfo") {
                ans = "yes/"+QString::fromStdString(pileInfoController.generatePileInfo(msgList[1].toStdString()))+"\t";
            }else if (msgList[0]=="getCarInfo") {
                ans = "yes/"+QString::fromStdString(pileInfoController.generateCarInfo(msgList[1].toStdString()))+"\t";
            }else if (msgList[0]=="getReport") {
                ans = "yes/"+QString::fromStdString(pileInfoController.getReport(msgList[1].toStdString()))+"\t";
            }else if (msgList[0]=="malfunction") { //服务器前端发来的充电桩故障
                //首先通知充电桩进程有故障
                QString msg="malfunction\t";
                sendMsg(msg,descriptor);
                Global::loop.exec();
                if(Global::buffer[0]=="no"){ //如果充电桩返回错误，则忽略此次请求
                    continue;
                }
                pileController.malfunction(descriptor,msgList[1].toInt());
            }else if (msgList[0]=="recover") { //服务器前端发来的故障恢复
                //首先通知充电桩进程故障恢复
                QString msg="malfunctionRecover\t";
                sendMsg(msg,descriptor);
                Global::loop.exec();
                if(Global::buffer[0]=="no"){ //如果充电桩返回错误，则忽略此次请求
                    continue;
                }
                pileController.malfunction(descriptor,2);
            }else if(msgList[0]=="call"){ //充电桩叫号
                //先检查call请求有没有带参数Request，如果有就加入l2
                if(msgList.size()>1){
                    char *s=msgList[1].toUtf8().data();
                    Request r=*(Request *)s;
                    Global::l2.append(r);
                    sendMsg("chargingToFinish\t",Global::mstr2Int[r.ownerID]); //给对应的用户发异步消息通知状态变化
                }
                string ID=pileController.call(descriptor); //如果没有成功调度，则返回空字符串
                if(ID!=""){
                    sendMsg("waitingToCharging\t",Global::mstr2Int[ID]); //给对应的用户发异步消息通知状态变化
                }
            }else if(msgList[0]=="adminLogon"){ //管理员登录，管理员端需要在建立连接后发此消息，获取初始化信息（此消息无参数）
                ans="yes/"+QString::number(fastPileNum)+"/"+QString::number(tricklePileNum)
                    +"/"+QString::number(waitingSize)+"/"+QString::number(chargingQueueLen)+"\t";
            }else if(msgList[0]=="pileLogon"){ //充电桩登录，使服务器获取充电桩信息，充电桩进程建立连接后发送此消息
                Global::mint2Str[descriptor]=msgList[1].toStdString();
                Global::mstr2Int[msgList[1].toStdString()]=descriptor;
            }else{
                ans = "no/message format error\t";
            }
            if(ans=="") return; //如果ans为空，说明不需要发返回消息
            sendMsg(ans,descriptor); //发送同步返回消息
        }
    }
}

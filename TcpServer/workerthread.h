#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QThread>
#include <QDebug>
#include "global.h"
#include "tcpclientsocket.h"
#include "querycontroller.h"
#include "pilecontroller.h"

/* 新建一个 WorkerThread 类继承于 QThread */
class WorkerThread : public QThread
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

signals:
    void showserver(QString, QHostAddress,int,bool);//发送信号给界面，让界面更新信息

/*public slots:
    void loopquit(){
        if(Global::loopNo==0){
            loop.quit();
        }else if(Global::loopNo==1){
            //pileController.loop.quit();
        }else if(Global::loopNo==2){
            pileInfoController.loop.quit();
        }else if(Global::loopNo==3){
            //requestController.loop.quit();
        }
    }
*/
public:
    //QEventLoop loop;

    PileController pileController;
    PileInfoController pileInfoController;
    //QueryController queryController;
    //RequestController requestController;
    UserController userController;

    WorkerThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写 run 方法，继承 QThread 的类，只有 run 方法是在新的线程里 */
    void run() override {
        //进入请求处理循环
        qDebug()<<"start handling..."<<Qt::endl;
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
                }/*else if (msgList[0]=="getQueueNum") {
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
                        std::string ret_str((char *)(&ret), sizeof(ret));
                        ans = "yes/" + QString::fromStdString(ret_str) + "\t";
                    } catch (const char *msg) {
                        ans = "no/" + QString::fromStdString(msg) + "\t";
                    }
                }*/else if (msgList[0]=="turnOnPile") {
                    if(msgList[1]=="0")
                        for(auto & it : Global::lp) {
                            sendMsg("turnOn\t", Global::mstr2Int[it]);
                        }
                    else sendMsg("turnOn\t", Global::mstr2Int[msgList[1].toStdString()]);
                    // pileController.turnOnPile(descriptor); TODO: 是否有充电桩返回
                    // Global::loop.exec();
                    // if(Global::buffer[0]=="no"){
                    //     continue;
                    // }
                }else if (msgList[0]=="turnOffPile") {
                    if(msgList[1]=="0")
                        for(auto & it : Global::lp) {
                            sendMsg("turnOff\t", Global::mstr2Int[it]);
                        }
                    sendMsg("turnOff\t", Global::mstr2Int[msgList[1].toStdString()]);
                    // pileController.turnOffPile(descriptor);
                    // Global::loop.exec();
                    // if(Global::buffer[0]=="no"){
                    //     continue;
                    // }
                }else if (msgList[0]=="getPileInfo") {
                    ans = QString::fromStdString(pileInfoController.generatePileInfo(msgList[1].toStdString()))+"\t";
                }else if (msgList[0]=="getCarInfo") {
                    ans = QString::fromStdString(pileInfoController.generateCarInfo(msgList[1].toStdString()))+"\t";
                }else if (msgList[0]=="getReport") {
                    ans = QString::fromStdString(pileInfoController.getReport(msgList[1].toStdString()))+"\t";
                }/*else if (msgList[0]=="malfunction") { //服务器前端发来的充电桩故障
                    //首先通知充电桩进程有故障
                    QString msg="malfunction\t";
                    sendMsg(msg,descriptor);
                    // Global::loop.exec();
                    // if(Global::buffer[0]=="no"){ //如果充电桩返回错误，则忽略此次请求
                    //     continue;
                    // }
                    pileController.malfunction(descriptor,msgList[1].toInt());
                }else if (msgList[0]=="recover") { //服务器前端发来的故障恢复
                    //首先通知充电桩进程故障恢复
                    QString msg="malfunctionRecover\t";
                    sendMsg(msg,descriptor);
                    // Global::loop.exec();
                    // if(Global::buffer[0]=="no"){ //如果充电桩返回错误，则忽略此次请求
                    //     continue;
                    // }
                    pileController.malfunction(descriptor,2);
                }*/else if(msgList[0]=="call"){ //充电桩叫号
                    qDebug()<<"handlesize"<<Global::handleList.size()<<Qt::endl;
                    //先检查call请求有没有带参数Request，如果有就加入l2
//                    if(msgList.size()>1){
//                        char *s=msgList[1].toUtf8().data();
//                        Request r=*(Request *)s;
//                        Global::l2.append(r);
//                        sendMsg("chargingToFinish\t",Global::mstr2Int[r.ownerID]); //给对应的用户发异步消息通知状态变化
//                    }
                    try {
                        Request req = pileController.call(QString::fromStdString(Global::mint2Str[descriptor]));
                        req.requestChargingCapacity=40;
                        // Global::mstr2Int[req.ownerID] = descriptor + 1; // FIXME: 冒烟测试专用
                        // QueryController::getUserByID(QString::fromStdString(req.ownerID)).changeState(CHARGING); TODO: 与 User 类对接
                        std::string req_str((char *)(&req), sizeof(req));
                        ans = QString::fromStdString("insertIntoPileList/" + req_str + "\t");
                        //sendMsg("waitingToCharging\t",Global::mstr2Int[req.ownerID]);
                    } catch (...) {
                        Global::l_call.append(Global::mint2Str[descriptor]); // 放入无可调度的充电桩信息
                    }
                }else if(msgList[0]=="adminLogon"){ //管理员登录，管理员端需要在建立连接后发此消息，获取初始化信息（此消息无参数）
                    ans="yes/"+QString::number(Global::fastChargingPileNum)+"/"+QString::number(Global::trickleChargingPileNum)
                        +"/"+QString::number(Global::waitingAreaSize)+"/"+QString::number(Global::chargingQueueLen)+"\t";
                }else if(msgList[0]=="pileLogon"){ //充电桩登录，使服务器获取充电桩信息，充电桩进程建立连接后发送此消息
                    Global::mint2Str[descriptor]=msgList[1].toStdString();
                    Global::mstr2Int[msgList[1].toStdString()]=descriptor;
                }else{
                    ans = "no/message format error\t";
                }
                if(ans=="") continue; //如果ans为空，说明不需要发返回消息
                sendMsg(ans,descriptor); //发送同步返回消息
            }else{
                continue;
            }
        }
    }

    void sendMsg(QString msg, int descriptor){//向指定描述符对应的socket发消息
        for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
        {
            QTcpSocket *item = Global::tcpclientsocketlist.at(i);
            if(item->socketDescriptor() == descriptor)
            {
                item->write(msg.toLatin1().data(),msg.size());
                item->flush();
                qDebug()<<"send to pile: "<<descriptor<<msg;
                emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
                break;
            }
        }
    }
};

#endif // WORKERTHREAD_H

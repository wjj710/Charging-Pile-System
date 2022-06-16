#include "requestcontroller.h"
#include "pilecontroller.h"

#define MAX_REQUEST_NUM 1000
int slowQueueNum=0;
int fastQueueNum=0;
int req=0; //记录是否是第一次请求的全局变量

/*这里是list<request>相关操作的声明与定义*/
bool checkList()//查l1，若请求已满，返回false
{
    if(Global::l1.size()>=MAX_REQUEST_NUM)
    {
        return false;
    }
    return true;
}

int getNewNum(int mode)//返回可用新排队号
{
    if(mode==0)//慢充
    {
        slowQueueNum++;
        return slowQueueNum;
    }
    else//快充
    {
        fastQueueNum++;
        return fastQueueNum;
    }
}

void add(Request r,int list)//将请求r加入list，list为操作的list号，l1或l2
{
    if(list==1)//l1
    {
        Global::l1.push_back(r);
    }
    else if(list==2)//l2
    {
        Global::l2.push_back(r);
    }
}

Request deleteRequest(int queueNum,int mode)//删除此请求并返回，同时顺延排队号
{
    /*先摘出此请求*/
    int target=-1;
    for(int i=0;i<Global::l1.size();i++)
    {
        if(Global::l1[i].queueNum==queueNum&&Global::l1[i].chargingMode==mode)//定位
        {
            target=i;
        }
    }
    Request r=Global::l1.takeAt(target);//摘出
    /*修改总数标识*/
    if(mode==0)
    {
        slowQueueNum--;
    }
    else
    {
        fastQueueNum--;
    }
    /*顺延*/
    for(int i=0;i<Global::l1.size();i++)
    {
        if(Global::l1[i].queueNum>queueNum&&Global::l1[i].chargingMode==mode)//定位
        {
            Global::l1[i].queueNum--;//顺延
        }
    }
    /*返回*/
    return r;
}

void changeCapacity(int queueNum, int mode, int value)//修改充电量
{
    for(int i=0;i<Global::l1.size();i++)
    {
        if(Global::l1[i].queueNum==queueNum&&Global::l1[i].chargingMode==mode)//定位
        {
            Global::l1[i].requestChargingCapacity=value;//修改
        }
    }
}

/*RequestController*/
QString RequestController::startRequest(int v, User *user,int mode, double capacity, double batteryCapacity)//创建请求
{
    if(!req){
        req=1;
        Global::t=time(0);
        Global::t1=Global::t-Global::t%(24*3600)+6*3600;
        //向所有充电桩发基准时间
        for(int i=0; i<Global::lp.size();i++){
            std::string pN=Global::lp.at(i);
            int descriptor = Global::mstr2Int[pN];
            std::string req_str((char *)(&Global::t), sizeof(Global::t));
            std::string ret = "setTime/" + req_str + "\t";
            QString msg=QString::fromStdString(ret);
            for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
            {
                QTcpSocket *item = Global::tcpclientsocketlist.at(i);
                if(item->socketDescriptor() == descriptor)
                {
                    item->write(msg.toLatin1().data(),msg.size());
                    item->flush();
                    qDebug()<<"send to pile: "<<descriptor<<msg;
                    //emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
                    break;
                }
            }
//            Global::mutex.lock();
//            Global::condition.wait(&Global::mutex);
//            Global::mutex.unlock();
        }
    }
    /*3.提交充电请求*/
    int number;//新的排队号
    /*1.1-1.2*/
//    if (user.isFinish() == false)//isFinish()来自user.h
//    {
//        return "no/用户仍有未完成的请求！\t";
//    }
    /*1.3-1.4*/
    if (checkList() == false)//checkList()来自list<request>操作
    {
        return "no/无空闲车位！\t";
    }
    /*1.5*/
    number = newQueueNum(mode);
    /*1.6-1.8*/
    char userID[8];
    for(int i=0;i<8;i++) if(i<user->getID().length())
    {
        userID[i]= user->getID().at(i).unicode();
    }
    struct Request temp={
        number, v,{userID[0],userID[1],userID[2],userID[3],userID[4],userID[5],userID[6],userID[7]}, mode, capacity,0,batteryCapacity,0,0,0,0,0
    };
//    add(temp,1);
//    /*1.9-1.10*/
//    user.writeQueueNum(QString::number(number));//writeQueueNum(number)来自user.h
//    user.writeMode(mode);//writeMode(mode)来自user.h
//    /*1.11-1.12*/
//    user.changeState("waiting");//changeState(state)来自user.h
    //为全局变量赋值
    std::string s=mode?"F":"T";
    Global::mq2v[s+std::to_string(number)]=v;
    //Global::mv2q[v]=s+std::to_string(number);
    int flag=0; //标识有无未处理的call
    QString pNo; //记录充电桩编号
    //新请求来时检查l_call中有没有之前未处理的call，如果有就发给对应的充电桩
    flag=PileController::handleNewRequest(temp,Global::l1);
//    for(int j=0; j<Global::l_call.size(); j++){
//        std::string k=Global::l_call.at(j);
//        if(k.substr(0,1)==s){ //说明有call
//            flag=1;
//            pNo=QString::fromStdString(k);
//            int descriptor = Global::mstr2Int[k];
//            std::string req_str((char *)(&temp), sizeof(temp));
//            std::string ret = "insertIntoPileList/" + req_str + "\t";
//            QString msg=QString::fromStdString(ret);
//            for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
//            {
//                QTcpSocket *item = Global::tcpclientsocketlist.at(i);
//                if(item->socketDescriptor() == descriptor)
//                {
//                    item->write(msg.toLatin1().data(),msg.size());
//                    item->flush();
//                    qDebug()<<"send to pile: "<<descriptor<<msg;
//                    //emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
//                    break;
//                }
//            }
//            Global::mutex.lock();
//            Global::condition.wait(&Global::mutex);
//            Global::mutex.unlock();
//            Global::l_call.removeAt(j);
//            break;
//        }
//    }
    //修改user类中车辆的信息，记录当前车辆的排队号、状态和所在充电桩号
    user->p[v].mode=mode;
    user->p[v].queueNum=number;
    if(!flag){ //等待
        //add(temp,1);
        user->p[v].state="waiting";
    }else{
        user->p[v].state="charging";
        user->p[v].pileNo=pNo;
    }
    return "yes\t";
}

QString RequestController::changeRequest(int v, User *user,int mode, double value)//更改请求，mode=0时，value为充电量；mode=1时，value为模式（慢0快1）
{
    /*4.修改充电请求*/
    int oldNumber;//旧充电号
    int oldMode;//旧充电模式
    /*1.1-1.2*/
//    if (user.isWaiting() == false)//isWaiting()来自user.h
//    {
//        return "no/用户不处于等待区\t";
//    }
    /*1.3-1.4*/
    oldNumber = user->p[v].queueNum;//getNumber()来自user.h
    oldMode=user->p[v].mode;
    /*mode=0*/
    if (mode == 0)//修改充电量
    {
        /*1.1-1.2*/
        user->p[v].capacity = value; // 修改user类中充电量信息
        changeCapacity(oldNumber,oldMode,value);//changeCapacity(oldNumber,value)来自list<request>操作
    }
    /*mode=1*/
    else if (mode == 1)//修改充电模式
    {
        /*1.1*/
        int newNumber = newQueueNum(value);
        /*1.2-1.3*/
        Request r=deleteRequest(oldNumber,oldMode);//deleteNum(number,mode)来自list<request>操作
        /*1.4-1.5*/
        char userID[8];
        for(int i=0;i<8;i++)
        {
            userID[i]= user->getID().at(i).unicode();
        }
        struct Request temp={
            newNumber,v, {userID[0],userID[1],userID[2],userID[3],userID[4],userID[5],userID[6],userID[7]}, (int)value, r.requestChargingCapacity,0,r.batteryCapacity,0,0,0,0,0
        };
//        add(temp,1);
//        /*1.6-1.7*/
//        user.writeQueueNum(QString::number(newNumber));
//        user.writeMode(value);
        //为全局变量赋值
        std::string s=mode?"F":"T";
        Global::mq2v[s+std::to_string(newNumber)]=v;
        //Global::mv2q[v]=s+std::to_string(number);
        int flag=0; //标识有无未处理的call
        QString pNo; //记录充电桩编号
        //新请求来时检查l_call中有没有之前未处理的call，如果有就发给对应的充电桩
        flag=PileController::handleNewRequest(temp,Global::l1);
//        for(int j=0; j<Global::l_call.size(); j++){
//            std::string k=Global::l_call.at(j);
//            if(k.substr(0,1)==s){ //说明有call
//                flag=1;
//                pNo=QString::fromStdString(k);
//                int descriptor = Global::mstr2Int[k];
//                std::string req_str((char *)(&temp), sizeof(temp));
//                std::string ret = "insertIntoPileList/" + req_str + "\t";
//                QString msg=QString::fromStdString(ret);
//                for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
//                {
//                    QTcpSocket *item = Global::tcpclientsocketlist.at(i);
//                    if(item->socketDescriptor() == descriptor)
//                    {
//                        item->write(msg.toLatin1().data(),msg.size());
//                        item->flush();
//                        qDebug()<<"send to pile: "<<descriptor<<msg;
//                        //emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
//                        break;
//                    }
//                }
//                Global::mutex.lock();
//                Global::condition.wait(&Global::mutex);
//                Global::mutex.unlock();
//                Global::l_call.removeAt(j);
//                break;
//            }
//        }
        //修改user类中车辆的信息，记录当前车辆的排队号、状态和所在充电桩号
        user->p[v].mode=mode;
        user->p[v].queueNum=newNumber;
        if(!flag){ //等待
            //add(temp,1);
            user->p[v].state="waiting";
        }else{
            user->p[v].state="charging";
            user->p[v].pileNo=pNo;
        }
    }
    return "yes\t";
}

QString RequestController::endRequest(int v, User *user)//结束请求
{
    /*5.结束充电*/
    /*1.1-1.2*/
//    if (user.isFinish() == false)//isFinish()来自user.h
//    {
//        return "no/用户仍有未完成的请求！\t";
//    }
    /*1.3-1.4*/
    int state = user->isWaiting(v);//isWaiting()来自user.h
    /*1.5-1.6*/
    int number = user->p[v].queueNum;//getNumber()来自user.h
    int mode=user->p[v].mode;//getMode()来自user.h
    /*state=1，在等待区*/
    if (state == 1)
    {
        /*1.1-1.2*/
        Request r=deleteRequest(number,mode);
        /*1.7-1.8*/
        add(r,2);
    }
    /*state=0，不在等待区，向充电桩发消息*/
    else if (state == 0)
    {
        /*1.1-1.2*/
        //QString pileNo = user.getPileNo();//getPileNo()来自user.h
        /*1.3-1.4*/
        //r = removeFromPileList(pileNo, user.getID());//removeFromPileList(pileNo, ownerID)来自pile.h
        /*1.7-1.8*/
        //add(r,2);
        int descriptor = Global::mstr2Int[user->p[v].pileNo.toStdString()];
        std::string ret = "removeFromPileList/" + std::to_string(number) + "\t";
        QString msg=QString::fromStdString(ret);
        for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
        {
            QTcpSocket *item = Global::tcpclientsocketlist.at(i);
            if(item->socketDescriptor() == descriptor)
            {
                item->write(msg.toLatin1().data(),msg.size());
                item->flush();
                qDebug()<<"send to pile: "<<descriptor<<msg;
                //emit showserver(msg,item->peerAddress(),item->peerPort(),1);// 发送给客户端设置为1
                break;
            }
        }
        Global::mutex.lock();
        Global::condition.wait(&Global::mutex);
        Global::mutex.unlock();
    }
    /*1.9-1.10*/
    //user.changeState("finished");
    user->p[v].state="finished";
    return "yes\t";
}

int RequestController::newQueueNum(int mode)//分配新的排队号
{
    return getNewNum(mode);//getNewNum(mode)返回新排队号，来自list<request>
}

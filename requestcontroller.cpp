#include "requestcontroller.h"

#define MAX_REQUEST_NUM 1000
int slowQueueNum=0;
int fastQueueNum=0;

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
QString RequestController::startRequest(User user,int mode, int capacity)//创建请求
{
    /*3.提交充电请求*/
    int number;//新的排队号
    /*1.1-1.2*/
    if (user.isFinish() == false)//isFinish()来自user.h
    {
        return "no/用户仍有未完成的请求！\t";
    }
    /*1.3-1.4*/
    if (checkList() == false)//checkList()来自list<request>操作
    {
        return "no/无空闲车位！\t";
    }
    /*1.5*/
    number = newQueueNum(mode);
    /*1.6-1.8*/
    add(Request(number, user.getID(), mode, capacity),1);
    /*1.9-1.10*/
    user.writeQueueNum(QString::number(number));//writeQueueNum(number)来自user.h
    user.writeMode(mode);//writeMode(mode)来自user.h
    /*1.11-1.12*/
    user.changeState("waiting");//changeState(state)来自user.h
    return "yes\t";
}

QString RequestController::changeRequest(User user,int mode, int value)//更改请求，mode=0时，value为充电量；mode=1时，value为模式（慢0快1）
{
    /*4.修改充电请求*/
    int oldNumber;//旧充电号
    int oldMode;//旧充电模式
    /*1.1-1.2*/
    if (user.isWaiting() == false)//isWaiting()来自user.h
    {
        return "no/用户不处于等待区\t";
    }
    /*1.3-1.4*/
    oldNumber = user.getNumber().toInt();//getNumber()来自user.h
    oldMode=user.getMode();
    /*mode=0*/
    if (mode == 0)//修改充电量
    {
        /*1.1-1.2*/
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
        add(Request(newNumber, user.getID(), value, r.requestChargingCapacity),1);
        /*1.6-1.7*/
        user.writeQueueNum(QString::number(newNumber));
        user.writeMode(value);
    }
    return "yes\t";
}

QString RequestController::endRequest(User user)//结束请求
{
    /*5.结束充电*/
    /*1.1-1.2*/
    if (user.isFinish() == false)//isFinish()来自user.h
    {
        return "no/用户仍有未完成的请求！\t";
    }
    /*1.3-1.4*/
    int state = user.isWaiting();//isWaiting()来自user.h
    /*1.5-1.6*/
    int number = user.getNumber().toInt();//getNumber()来自user.h
    int mode=user.getMode();//getMode()来自user.h
    /*state=1，在等待区*/
    if (state == 1)
    {
        /*1.1-1.2*/
        Request r=deleteRequest(number,mode);
        /*1.7-1.8*/
        add(r,2);
    }
    /*state=0，不在等待区*/
    else if (state == 0)
    {
        /*1.1-1.2*/
        //QString pileNo = user.getPileNo();//getPileNo()来自user.h
        /*1.3-1.4*/
        //r = removeFromPileList(pileNo, user.getID());//removeFromPileList(pileNo, ownerID)来自pile.h
        /*1.7-1.8*/
        //add(r,2);
    }
    /*1.9-1.10*/
    user.changeState("finished");
    return "yes\t";
}

int RequestController::newQueueNum(int mode)//分配新的排队号
{
    return getNewNum(mode);//getNewNum(mode)返回新排队号，来自list<request>
}

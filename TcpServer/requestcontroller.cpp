#include "requestcontroller.h"

/*这里是list<request>相关操作的声明与定义*/
std::string checkList()
{
    //...
}
//...

std::string RequestController::startRequest(int mode, int capacity)//创建请求
{
    /*3.提交充电请求*/
    int number;//新的排队号
    /*1.1-1.2*/
    if (isFinish() == "no\t")//isFinish()来自user.h
    {
        return "no/用户仍有未完成的请求！\t";
    }
    /*1.3-1.4*/
    if (checkList() == "no\t")//checkList()来自list<request>操作
    {
        return "no/无空闲车位！\t";
    }
    /*1.5*/
    number = newQueueNum(mode);
    /*1.6-1.8*/
    if (add(Request(number, ownerID, mode, capacity, batteryCapacity)) == "no\t")//add(r)来自list<request>操作，ownerID和bCapacity由全局变量获取
    {
        return "no/添加Request入l1失败\t";
    }
    /*1.9-1.10*/
    if (writeQueueNum(number) == "no\t")//writeQueueNum(number)来自user.h
    {
        return "no/记录排队号失败\t";
    }
    /*1.11-1.12*/
    if (changeState(waitingState) == "no\t")//changeState(state)来自user.h
    {
        return "no/更改状态失败\t";
    }
    return "yes\t";
}

std::string RequestController::changeRequest(int mode, int value)//更改请求
{
    /*4.修改充电请求*/
    int oldNumber;//旧充电号
    /*1.1-1.2*/
    if (isWaiting() == "no\t")//isWaiting()来自user.h
    {
        return "no/用户不处于等待区\t";
    }
    /*1.3-1.4*/
    oldNumber = getNumber();//getNumber()来自user.h
    /*mode=0*/
    if (mode == 0)//修改充电量
    {
        /*1.1-1.2*/
        if (changeCapacity(oldNumber, value) == "no\t")//changeCapacity(oldNumber,value)来自list<request>操作
        {
            return "no/修改充电量失败\t";
        }
    }
    /*mode=1*/
    else if (mode == 1)
    {
        /*1.1*/
        int newNumber = newQueueNum(cMode);//cMode是更改后的充电模式，从全局变量获取
        /*1.2-1.3*/
        if (deleteNum(oldNumber) == "no\t")//deleteNum(number)来自list<request>操作
        {
            return "no/旧请求删除失败\t";
        }
        /*1.4-1.5*/
        if (addNum(newNumber) == "no\t")//addNum(newNumber)来自list<request>操作
        {
            return "no/新请求添加失败\t";
        }
        /*1.6-1.7*/
        if (writeQueueNum(newNumber) == "no\t")//writeQueueNum(number)来自user.h
        {
            return "no/记录排队号失败\t";
        }
    }
    return "yes\t";
}

std::string RequestController::endRequest()//结束请求
{
    /*5.结束充电*/
    Request r;//r为request对象，后面可能用到
    /*1.1-1.2*/
    if (isFinish() == "no\t")//isFinish()来自user.h
    {
        return "no/用户仍有未完成的请求！\t";
    }
    /*1.3-1.4*/
    int state = isWaiting();//isWaiting()来自user.h
    /*1.5-1.6*/
    int number = getNumber();//getNumber()来自user.h
    /*state=1，在等待区*/
    if (state == 1)
    {
        /*1.1-1.2*/
        if (deleteNum(number) == "no\t")//deleteNum(number)来自list<request>操作
        {
            return "no/旧请求删除失败\t";
        }
    }
    /*state=0，不在等待区*/
    else if (state == 0)
    {
        /*1.1-1.2*/
        int pileNo = getPileNo();//getPileNo()来自user.h
        /*1.3-1.4*/
        r = removeFromPileList(pileNo, ownerID);//removeFromPileList(pileNo, ownerID)来自pile.h，ownerID来自全局变量，返回一个Request对象赋给r
    }
    /*1.7-1.8*/
    if (add(r) == "no\t")//add(r)来自list<request>操作
    {
        return "no/添加Request入l1失败\t";
    }
    /*1.9-1.10*/
    if (changeState(finishState) == "no\t")
    {
        return "no/更改状态失败\t";
    }
    return "yes\t";
}

int RequestController::newQueueNum(int mode);//分配新的排队号
{
    return getNewNum(mode);//getNewNum(mode)返回新排队号，来自list<request>
}

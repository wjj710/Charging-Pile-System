#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include "user.h"
#include "request.h"
//#include "pile.h"
#include "global.h"

class RequestController {
public:
    QString startRequest(int v, User *user,int mode,double capacity, double batteryCapacity);       //创建请求
    QString changeRequest(int v, User *user,QString mode,double value);     //更改请求，mode=0时，value为充电量；mode=1时，value为模式（慢0快1）
    QString endRequest(int v, User *user);      //结束请求
    int newQueueNum(int mode);     //分配新的排队号
};

#endif // REQUESTCONTROLLER_H

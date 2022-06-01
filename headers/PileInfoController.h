#ifndef PILEINFOCONTROLLER_H
#define PILEINFOCONTROLLER_H
#include "Pile.h"
//pileinfocontroller
//有返回值的方法，类型为字符串，可直接做消息主体部分发出给管理员
class PileInfoController
{
public:
    void getPileInfo(char* pileNo);
    char* generatePileInfo(char* pileNo);  //生成某充电桩状态信息，供服务器调用
    void getCarInfo(char* pileNo);
    char* generateCarInfo(char* pileNo);  //生成某充电桩排队车辆信息，供服务器调用
    char* getReport();                    //生成报表，供服务器调用
};

#endif // PILEINFOCONTROLLER_H

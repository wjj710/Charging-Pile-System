#ifndef PILEINFOCONTROLLER_H
#define PILEINFOCONTROLLER_H
#include <string>
#include <QEventLoop>

//pileinfocontroller
//有返回值的方法，类型为字符串，可直接做消息主体部分发出给管理员
class PileInfoController
{
public:
    //QEventLoop loop;
    std::string generatePileInfo(std::string pileNo);  //生成某充电桩状态信息，供服务器调用
    std::string generateCarInfo(std::string pileNo);  //生成某充电桩排队车辆信息，供服务器调用
    std::string getReport(std::string pileNo);        //生成报表，供服务器调用

};

#endif // PILEINFOCONTROLLER_H

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
    void generatePileInfo(std::string pileNo, int admin_socketNo);  //生成某充电桩状态信息，供服务器调用
    void generateCarInfo(std::string pileNo,int admin_socketNo);  //生成某充电桩排队车辆信息，供服务器调用
    void getReport(std::string pileNo,int admin_socketNo);        //生成报表，供服务器调用
    void sendArrayMsg(QByteArray msg, int descriptor);//向指定描述符对应的socket发消息
};

#endif // PILEINFOCONTROLLER_H

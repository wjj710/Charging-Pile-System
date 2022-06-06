#ifndef REPORT_H
#define REPORT_H
#include <QDateTime>
//Report相关文件可能不打算用了，因为pileinfo里面设置了结构体，就不太需要单独准备一个类
class Report
{
public:
    QDateTime* time;               //时间
    char* pileNo;                  //充电桩编号
    int totalChargingNumber;       //充电桩总次数
    int totalChargingTime;         //充电桩总时间
    double totalChargingCapacity;  //充电桩总量
    double totalChargingFee;       //累计充电费用
    double totalServiceFee;        //累计服务费用
    double totalAllFee;            //累计总费用
};

#endif // REPORT_H

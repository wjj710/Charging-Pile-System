#include <time.h>
#include <string>
using namespace std;

//充电桩状态
struct PileInfo{
    int workingState;             //充电桩工作状态，开启、关闭或故障（分别为0、1、2）
    int totalChargingNumber;      //累计充电次数
    time_t totalChargingTime;     //累计充电时间（秒）
    double totalChargingCapacity; //累计充电量
};

//车辆信息
struct CarInfo{
    string ownerID;           //发出请求的用户id
    double batteryCapacity;   //电池容量
    double requestChargingCapacity;  //请求充电电量
    time_t queueTime;         //排队时长（秒）
};

//报表信息
struct ReportInfo{
    string pileNo;        //充电桩编号，格式与排队号相似，比如快充电桩为F后跟一数字
    int totalChargingNumber;       //累计充电次数
    time_t totalChargingTime;      //累计充电时间
    double totalChargingCapacity;  //累计充电量
    double totalChargingFee;       //累计充电费用
    double totalServiceFee;        //累计服务费用
    double totalAllFee;            //累计总费用
};
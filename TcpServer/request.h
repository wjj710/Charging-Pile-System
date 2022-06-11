#ifndef REQUEST_H
#define REQUEST_H
#include <string>
//using namespace std;

//请求类定义
//class Request
//{
//    public:
//        int queueNum;             //排队号
//        std::string ownerID;           //发出请求的用户id
//        int chargingMode;         //充电模式，0表示慢充，1表示快充
//        double requestChargingCapacity;  //请求充电电量
//        double chargingCapacity;  //实际充电电量
//        double batteryCapacity;   //电池容量
//        //这里前端可以加个判断，如果请求充电电量超过电池容量，则请求失败
//        time_t requestTime;       //发出请求的时间
//        //查看等候车辆信息时用当前时间（通过time(0)获取）减去requestTime可以得到排队时间（秒）
//        time_t startChargingTime; //开始充电的时间
//        time_t endChargingTime;   //结束充电的时间
//        //生成详单时用endChargingTime减去startChargingTime可以得到充电时间（秒）
//        double chargingFee;       //充电费用
//        double serviceFee;        //服务费用
//        //生成详单时用chargingFee加上serviceFee可以得到总费用

//        //类构造函数
//        Request(int qNum, std::string oID, int cMode, double rChargingCapacity, double bCapacity);
//};

struct Request
{
        int queueNum;             //排队号
        char ownerID[8];           //发出请求的用户id
        int chargingMode;         //充电模式，0表示慢充，1表示快充
        double requestChargingCapacity;  //请求充电电量
        double chargingCapacity;  //实际充电电量
        double batteryCapacity;   //电池容量
        //这里前端可以加个判断，如果请求充电电量超过电池容量，则请求失败
        time_t requestTime;       //发出请求的时间
        //查看等候车辆信息时用当前时间（通过time(0)获取）减去requestTime可以得到排队时间（秒）
        time_t startChargingTime; //开始充电的时间
        time_t endChargingTime;   //结束充电的时间
        //生成详单时用endChargingTime减去startChargingTime可以得到充电时间（秒）
        double chargingFee;       //充电费用
        double serviceFee;        //服务费用
        //生成详单时用chargingFee加上serviceFee可以得到总费用

};

#endif // REQUEST_H

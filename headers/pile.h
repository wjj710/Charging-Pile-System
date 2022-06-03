#include "Request.h"
#include <vector>
#include <windows.h>
#include <time.h>
using namespace std;

class Pile
{
    public:
        string pileNo;        //充电桩编号，格式与排队号相似，比如快充电桩为F后跟一数字
        int workingState;     //充电桩工作状态，开启、关闭或故障（分别为0、1、2），刚初始化时为开启状态
        double power;         //充电桩功率（单位是度/小时）
        int chargingQueueLen; //充电桩队列最大长度
        time_t totalChargingTime;      //累计充电时间
        int totalChargingNumber;       //累计充电次数
        double totalChargingFee;       //累计充电费用
        double totalChargingCapacity;  //累计充电量
        vector<Request> chargingQueue; //充电桩排队队列

        Pile(char *argv[]);               //构造函数
        string turnOn();                  //打开充电桩
        string turnOff();                 //关闭充电桩
        string insertIntoPileList(Request r1);                    //将请求插入充电桩队列
        string removeFromPileList(DWORD threadid, int qNum);      //将请求从充电桩队列中移除
        string isEmpty();                 //检查充电桩队列是否为空
        string select(int mode);          //从充电桩中获取信息，mode=0表示获取充电桩状态，mode=1表示获取车辆信息，mode=2表示获取报表信息
        string clearQueue();              //把充电桩的等候队列清空
        double calculateFee(time_t start, time_t end);            //计算充电费用（假定充电时间不超过一天）
};

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
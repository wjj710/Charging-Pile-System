#include "Request.h"
#include <vector>

class Pile
{
    public:
        string pileNo;        //充电桩编号，格式与排队号相似，比如快充电桩为F后跟一数字
        int workingState;     //充电桩工作状态，开启、关闭或故障（分别为0、1、2），刚初始化时为开启状态
        double power;         //充电桩功率（单位是度/小时）
        int chargingQueueLen; //充电桩队列最大长度
        int chargingQueueNum; //正在充电的充电请求排队号
        time_t totalChargingTime;      //累计充电时间
        time_t totalChargingNumber;    //累计充电次数
        double totalChargingFee;       //累计充电费用
        vector<Request> chargingQueue; //充电桩排队队列

        Pile(char *argv[]);            //构造函数
        int turnOn();                  //打开充电桩
        int turnOff();                 //关闭充电桩
        int insertIntoPileList();      //将请求插入充电桩队列
        int removeFromPileList();      //将请求从充电桩队列中移除
        int isEmpty();                 //检查充电桩队列是否为空
        int select();                  //从充电桩中获取信息
        int clearQueue();              //把充电桩的等候队列清空
        double calculateFee(time_t start, time_t end);  //计算充电费用
};
#ifndef ADMIN_H
#define ADMIN_H
struct Setting
{
    int FastCharingPileNum = 0;
    int TrickleChargingPileNum = 0;
    int WaitingAreaSize = 0;
    int ChargingQueueLen = 0;
};

struct AdminMessage
{
    int flag;
    /*消息体不同类型:
     * 0表示初始化消息，
     * 1表示开启充电桩，
     * 2表示开启全部充电桩，
     * 3表示关闭充电桩
     * 4表示关闭全部充电桩，
     * 5表示查询信息，
     * 6表示查询全部信息
     * 7表示查询车信息
     * 8表示查询全部车信息
     * 9查报表
    */
    Setting setting;
    int OnNo;   //开启一个充电桩时，编号
    int OffNo; //关闭一个充电桩时，编号
    int GetStateNo;
    int GetInfoNo;
};

#endif // ADMIN_H

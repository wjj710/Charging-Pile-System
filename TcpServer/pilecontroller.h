#ifndef CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H
#define CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H

//#include "../headers/pile.h"
#include <QString>
#include "request.h"

// PileController
// 接口参数与内部细节仍需调整，尚未完工
class PileController {
public:
    int scheduleMode = 0;               // 调度模式 0:等候区队列中调度 1:优先级调度 2:时间顺序调度

//    void turnOnPile(int);             // 打开充电桩
//    void turnOffPile(int);            // 关闭充电桩
    void call(std::string);              // 充电桩叫号，返回值是被叫请求的用户ID，传入的参数是发出叫号请求的充电桩socket描述符
    void malfunction(std::string, int);     // 故障处理，参数为充电桩编号和调度方式
    QList<Request> getAllRequestFromPile(std::string);
    Request getRequest(std::string);
    static void handleNewRequest(Request, QList<Request>&);
    static std::string getIdlePile(int);

    static bool cmpRequest(Request, Request);

    void sendMsg(QString msg, int descriptor);
    static void sendMsg2(QString msg, int descriptor);


//    int getScheduleMode();              // 获取调度模式
//    void changeScheduleMode(int);       // 修改调度模式
//    void schedule();                  // 调度
//    void reschedule();                // 重调度
};


#endif //CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H

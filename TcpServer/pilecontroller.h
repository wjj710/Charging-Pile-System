#ifndef CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H
#define CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H
#include <string>

// PileController
// 接口参数与内部细节仍需调整，尚未完工
class PileController {
public:
    int scheduleMode = 0;           // 调度模式 0:等候区队列中调度 1:优先级调度 2:时间顺序调度

    void turnOnPile();         // 打开充电桩
    void turnOffPile();        // 关闭充电桩
    std::string call(int pile);     // 充电桩叫号，返回值是被叫请求的用户ID，传入的参数是发出叫号请求的充电桩socket描述符
    void malfunction(int pile, int mode);  // 故障处理，参数为充电桩socket描述符和调度方式
    int getScheduleMode();          // 获取调度模式
    void changeScheduleMode(int);   // 修改调度模式
    void schedule();                // 调度
    void reschedule();              // 重调度
};


#endif //CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H

//
// Created by Tony Zhao on 2022/5/28.
//

#ifndef CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H
#define CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H

#include "Pile.h"

// PileController
// 接口参数与内部细节仍需调整，尚未完工
class PileController {
public:
    int scheduleMode = 0;           // 调度模式 0:等候区队列中调度 1:优先级调度 2:时间顺序调度

    void turnOnPile(Pile&);         // 打开充电桩
    void turnOffPile(Pile&);        // 关闭充电桩
    void call(Pile&);               // 充电桩叫号
    void malfunction();             // 故障
    int getScheduleMode();          // 获取调度模式
    void changeScheduleMode(int);   // 修改调度模式
    void schedule();                // 调度
    void reschedule();              // 重调度
};


#endif //CHARGING_PILE_SYSTEM_PILE_CONTROLLER_H

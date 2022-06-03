#ifndef CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H
#define CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H

#include "user.h"
#include "detail.h"

// QueryController
// 无需实例化，直接调用相关函数即可。
class QueryController {
public:
    int getQueueNum(User&);     // 查看本车排队号码
    int getAheadNum(User&);     // 查看本充电模式下前车等待数量
    Detail& getDetail(User&);   // 查看详单
};

#endif //CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H

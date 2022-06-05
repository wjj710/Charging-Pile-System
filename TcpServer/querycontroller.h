#ifndef CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H
#define CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H

#include "user.h"
#include "detail.h"
#include <QString>

// QueryController
// 无需实例化，直接调用相关函数即可。
class QueryController {
public:
    static QString getQueueNum(User&);          // 查看本车排队号码
    static int getAheadNum(User&);              // 查看本充电模式下前车等待数量
    static Detail getDetail(User&);             // 查看详单

    static User &getUserByID(const QString&);   // 辅助函数，获取User类
};

#endif //CHARGING_PILE_SYSTEM_QUERY_CONTROLLER_H

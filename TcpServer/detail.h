#ifndef CHARGING_PILE_SYSTEM_DETAIL_H
#define CHARGING_PILE_SYSTEM_DETAIL_H

#include "request.h"
#include <QDateTime>

class Detail {
public:
    int detailNo;                   // 详单编号
    QDateTime generateTime;         // 生成时间
    int pileNo;                     // 充电桩编号
    double chargingCapacity;        // 充电容量
    int chargingTime;               // 充电时间，以秒为单位
    QDateTime startChargingTime;    // 充电开始时间
    QDateTime endChargingTime;      // 充电结束时间
    int chargingFee;                // 充电费用，以分为单位
    int serviceFee;                 // 服务费用，以分为单位
    int allFee;                     // 总费用，以分为单位

    explicit Detail(Request&);
};

#endif //CHARGING_PILE_SYSTEM_DETAIL_H

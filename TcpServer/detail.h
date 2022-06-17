#ifndef CHARGING_PILE_SYSTEM_DETAIL_H
#define CHARGING_PILE_SYSTEM_DETAIL_H

#include "request.h"
#include <QDateTime>
#include <QString>

class Detail {
public:
    int detailNo;                   // 详单编号
    QDateTime generateTime;         // 生成时间
    QString pileNo;                 // 充电桩编号
    double chargingCapacity;        // 充电容量
    time_t chargingTime;               // 充电时间，以秒为单位
    QDateTime startChargingTime;    // 充电开始时间
    QDateTime endChargingTime;      // 充电结束时间
    double chargingFee;             // 充电费用
    double serviceFee;              // 服务费用
    double allFee;                  // 总费用

    explicit Detail(Request&);

private:
    static int detailCount;
    static QString getPileNo(const QString&);
};

#endif //CHARGING_PILE_SYSTEM_DETAIL_H

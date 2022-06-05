//
// Created by Tony Zhao on 2022/5/28.
//

#include "detail.h"
#include "global.h"
#include <QString>

int Detail::detailCount = 0;

Detail::Detail(Request& request) {
    detailNo = ++detailCount;
    generateTime = QDateTime::currentDateTime();
    pileNo = getPileNo(QString::fromStdString(request.ownerID));
    chargingCapacity = request.chargingCapacity;
    chargingTime = int(request.endChargingTime - request.startChargingTime);
    startChargingTime = QDateTime::fromSecsSinceEpoch(request.startChargingTime);
    endChargingTime = QDateTime::fromSecsSinceEpoch(request.endChargingTime);
    chargingFee = FeeManager.getChargingFee(request);                                   // FeeManager 亟待对接
    serviceFee = FeeManager.getServiceFee(request);                                     // FeeManager 亟待对接
    allFee = chargingFee + serviceFee;
}

QString Detail::getPileNo(const QString& userID) {
    for(auto & it : Global::usr)
        if(it.ID == userID) return it.pileNo;
}
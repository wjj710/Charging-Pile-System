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
    pileNo = Global::usr[0].p[request.vNum].pileNo;
    chargingCapacity = request.chargingCapacity;
    chargingTime = request.endChargingTime - request.startChargingTime;
    startChargingTime = QDateTime::fromSecsSinceEpoch(qint64(request.startChargingTime));
    endChargingTime = QDateTime::fromSecsSinceEpoch(qint64(request.endChargingTime));
    chargingFee = request.chargingFee;
    serviceFee = request.chargingCapacity*0.8;
    allFee = chargingFee + serviceFee;
}

QString Detail::getPileNo(const QString& userID) {
    for(auto & it : Global::usr)
        if(it.getID() == userID) return it.getPileNo();
}

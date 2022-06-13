//
// Created by Tony Zhao on 2022/5/28.
//

#include "pilecontroller.h"
//#include "pile.h"
#include "global.h"
#include "request.h"
#include <QString>
#include <QList>

//void PileController::turnOnPile(int pileSocket) {
//    return;
//}

//void PileController::turnOffPile(int pileSocket) {
//    return;
//}

Request PileController::call(QString pileNo) { // 为指定pileNo返回一个Request
    //return {1, "test", 0, 1, 1,0,0,0,0,0,0};  // FIXME: 冒烟测试专用
    for(QList<Request>::iterator it = Global::l_priority.begin(); it != Global::l_priority.end(); ++it) {
        if((it->chargingMode == 0 && pileNo[0] == 'T') || (it->chargingMode == 1 && pileNo[0] == 'F')) {
            Request ret = *it;
            Global::l1.erase(it);
            return ret;
        }
    }
    for(QList<Request>::iterator it = Global::l1.begin(); it != Global::l1.end(); ++it) {
        if((it->chargingMode == 0 && pileNo[0] == 'T') || (it->chargingMode == 1 && pileNo[0] == 'F')) {
            Request ret = *it;
            Global::l1.erase(it);
            return ret;
        }
    }
    throw "No Available Request for Pile " + pileNo.toStdString();
}

void PileController::malfunction(QString pileNo, int mode) {
    scheduleMode = mode;
    if(mode==1) {

    } else if(mode==2) {

    }
    return;
}

//int PileController::getScheduleMode() {
//    return scheduleMode;
//}

//void PileController::changeScheduleMode(int newMode) {
//    return;
//}

//void PileController::schedule() {
//    return;
//}

//void PileController::reschedule() {
//    return;
//}

//
// Created by Tony Zhao on 2022/5/28.
//

#include "querycontroller.h"
#include "user.h"
#include "detail.h"
#include "global.h"
#include <QString>

QString QueryController::getQueueNum(User &user) {
    return user.getNumber();
}

int QueryController::getAheadNum(User &user) {
    if(!user.isWaiting()) throw "User Not Waiting";
    int ret = 0;
    QString num = user.getNumber();
    for(auto & it : Global::l1) {
        if(QString::fromStdString(std::to_string(it.queueNum)) == num) return ret;
        ++ret;
    }
    throw "Request Not Found";
}

Detail QueryController::getDetail(User &user) {
    if(!user.isFinish()) throw "User Not Finish";
    QString num = user.getNumber();
    for(auto & it : Global::l2)
        if(QString::fromStdString(std::to_string(it.queueNum)) == num)
            return *(new Detail(it));
    throw "Request Not Found";
}

User &QueryController::getUserByID(const QString& userID) {
    for(auto & it: Global::usr)
        if(it.getID() == userID)
            return it;
    throw "User Not Found";
}

#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString ID;         // 用户名
    QString state;      // 当前状态
    QString queueNum;   // 排队号
    QString pileNo;     // 充电桩号

public:
    User(QString usrID, QString state_) { ID = usrID; state = state_; }
    bool isWaiting() { if (state == "waiting") return true; else return false; }
    bool isFinish() { if (state == "finished") return true; else return false; }
    QString getNumber() { return queueNum; }
    QString getPileNo() { return pileNo; }
    QString getID() { return ID; }
    void writeQueueNum(QString new_num) { queueNum = new_num; }
    void writePileNo(QString new_no) { pileNo = new_no; }
    void changeState(QString new_state) { state = new_state; }
};

#endif // USER_H

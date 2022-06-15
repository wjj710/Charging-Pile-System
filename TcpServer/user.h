#ifndef USER_H
#define USER_H

#include <QString>

class car{
public:
    QString state;
    int mode;
    int queueNum;
    QString pileNo;
    double capacity;   // 充电量
    double battery;    // 电池容量
    car() { state = "free"; mode = 0; capacity = 0; battery = 0; }
};

class User
{
private:
    QString ID;         // 用户名
    QString state;      // 当前状态
    int mode;           // 0表示慢充，1表示快充
    QString queueNum;   // 排队号
    QString pileNo;     // 充电桩号

public:
    car p[40]; //最多40辆车
    User(QString usrID, QString state_) { ID = usrID; state = state_; }
    bool isWaiting(int v) { if (p[v].state == "waiting") return true; else return false; }
    bool isFinish(int v) { if (p[v].state == "finished") return true; else return false; }
    QString getNumber() { return queueNum; }
    QString getPileNo() { return pileNo; }
    QString getID() { return ID; }
    int getMode(){ return mode; }
    void writeQueueNum(QString new_num) { queueNum = new_num; }
    void writeMode(int new_mode){ mode=new_mode; }
    void writePileNo(QString new_no) { pileNo = new_no; }
    void changeState(QString new_state) { state = new_state; }
};

#endif // USER_H

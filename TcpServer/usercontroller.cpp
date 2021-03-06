#include "usercontroller.h"

UserController::UserController()
{
    waiting = "waiting";
    charging = "charging";
    free = "free";
    finished = "finished";
}

QString UserController::login(int socket, QString usrID, QString password)
{
    bool need_append = true;
    User user(usrID, waiting);
    for (int i = 0; i < Global::usr.length(); i++) {
        if (Global::usr[i].getID() == usrID) {  // 用户已在用户队列中，则不添加新的用户
            need_append = false;
            break;
        }
    }
    if (need_append) {      // 用户没有不在队列中，将其添加到用户队列
        Global::usr.append(user);
    }
    Global::mint2Str[socket]=usrID.toStdString();   // 更新套接字和用户ID的映射关系
    Global::mstr2Int[usrID.toStdString()]=socket;

    QString correct_pass, ans;
    int myerrno = DBFacade::Instance().Query(usrID, correct_pass);
    if (myerrno == 0) {
        if (correct_pass == password) {         // 密码正确，登录成功
            ans = "login/yes\t";
        } else {                                // 密码错误
            ans = "login/no\t";
        }
    } else if (myerrno == 4) {                  // 未注册
        ans = "login/unknown\t";
    } else {                                    // 数据库错误
        ans = "login/error\t";
    }
    return ans;
}

QString UserController::registration(int socket, QString usrID, QString password)
{
    QString ans;
    int myerrno = DBFacade::Instance().Insert(usrID, password);
    if (myerrno == 0) {                         // 注册成功
        ans = "logon/yes\t";
    } else if (myerrno == 3) {
        ans = "logon/no\t";
    } else {
        ans = "logon/error\t";
    }
    return ans;
}

QString UserController::getState(User* usr, QString carNum)
{
    int modeCode = usr->p[carNum.toInt()].mode;
    QString state = usr->p[carNum.toInt()].state;
    QString mode = modeCode == 0 ? "slow" : "fast";
    QString capacity = QString::number(int(usr->p[carNum.toInt()].capacity));
    QString battery = QString::number(int(usr->p[carNum.toInt()].battery));
    return QString("state/%1/%2/%3/%4\t").arg(state, mode, capacity, battery);
}

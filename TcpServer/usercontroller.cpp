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
    User user(usrID, waiting);
    Global::usr.append(user);
    Global::mint2Str[socket]=usrID.toStdString();
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
    User user(usrID, waiting);
    Global::usr.append(user);
    Global::mint2Str[socket]=usrID.toStdString();
    Global::mstr2Int[usrID.toStdString()]=socket;

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

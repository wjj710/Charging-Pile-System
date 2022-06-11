#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "dbfacade.h"
#include "global.h"
#include "user.h"

class UserController
{
private:
    QString waiting;
    QString free;
    QString charging;
    QString finished;

public:
    UserController();

    /**
     * @brief login 登录
     * @param socket 用户套接字
     * @param usrID 用户名
     * @param password 密码
     * @return 回复信息
     */
    QString login(int socket, QString usrID, QString password);

    /**
     * @brief registration 登录
     * @param socket 用户套接字
     * @param usrID 用户名
     * @param password 密码
     * @return 回复信息
     */
    QString registration(int socket, QString usrID, QString password);
};

#endif // USERCONTROLLER_H

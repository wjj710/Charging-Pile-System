#ifndef DBFACADE_H
#define DBFACADE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DBFacade
{
private:
    QSqlDatabase database;
    QSqlQuery sql;
    QString table_name;

    void PrintInfo(QString str);
public:
    static DBFacade& Instance();
    DBFacade();
    ~DBFacade();

    /**
     * @brief Insert 插入一行新记录
     * @param usrID 用户名
     * @param password 密码
     * @return 0 表示成功插入记录
     * @return 1 表示数据库事务开启失败
     * @return 2 表示数据库事务提交失败
     * @return 3 表示插入记录失败，用户名重复
     */
    int Insert(QString usrID, QString password);

    /**
     * @brief Delete 删除一行记录
     * @param usrID 用户名
     * @return 0 表示成功插入记录
     * @return 1 表示数据库事务开启失败
     * @return 2 表示数据库事务提交失败
     * @return 3 表示删除记录失败
     */
    int Delete(QString usrID);

    /**
     * @brief Query 查询用户密码
     * @param usrID 用户名
     * @param password 用于接收密码的字符串
     * @return 0 表示查询成功
     * @return 1 表示数据库事务开启失败
     * @return 2 表示数据库事务提交失败
     * @return 3 表示查询记录失败
     * @return 4 表示没有查询到记录，说明用户未注册
     */
    int Query(QString usrID, QString& password);
};

#endif // DBFACADE_H

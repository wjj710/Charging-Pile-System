#include "dbfacade.h"
#include <QDebug>

void DBFacade::PrintInfo(QString str)
{
    qDebug() << str;
}

DBFacade& DBFacade::Instance()
{
    static DBFacade db;
    return db;
}

DBFacade::DBFacade()
    : table_name("usersInfo")
{
    if (QSqlDatabase::contains("userDB_connection")) { // 检测数据库连接是否存在，存在则连接
        database = QSqlDatabase::database("userDB_connection");
        PrintInfo("DB connection already exists.");
    } else {                                           // 不存在则创建连接
        database = QSqlDatabase::addDatabase("QSQLITE", "userDB_connection");
        sql = QSqlQuery(database);
        database.setDatabaseName("userDB.db");
        PrintInfo("DB connection created");
    }
    if (database.open()) {                              // 数据库打开成功
        PrintInfo("DB open succeed.");
        QString cmd = QString("create table %1 ("       // 创建表格，已存在则创建失败，失败没有影响
                              "usrID TEXT PRIMARY KEY NOT NULL,"
                              "password TEXT NOT NULL)").arg(table_name);
        if (sql.exec(cmd))                              // 创建成功
            PrintInfo("table created.");
        else                                            // 创建失败
            PrintInfo("table create failed: " + sql.lastError().databaseText());
    } else                                              // 数据库打开失败
        PrintInfo("DB open failed: " + database.lastError().databaseText());
}

DBFacade::~DBFacade()
{
    database.close();
}

int DBFacade::Insert(QString usrID, QString password)
{
    int myerrno = 0;
    if (database.transaction()) {   // 开启事务

        QString cmd = QString("insert into %1 VALUES(:usrID, :password)").arg(table_name);
        sql.prepare(cmd);
        sql.bindValue(":usrID", usrID);
        sql.bindValue(":password", password);
        if (sql.exec()) {           // 执行成功
            PrintInfo(QString("insert succeed: %1  %2").arg(usrID, password));
            myerrno = 0;
        } else {                    // 执行失败，id重复
            PrintInfo("insert failed: " + sql.lastError().databaseText());
            myerrno = 3;
        }

        if (!database.commit()) {   // 事务提交失败
            database.rollback();    // 回滚事务
            myerrno = 2;
        }
    } else {                        // 事务开启失败
        myerrno = 1;
    }
    return myerrno;
}

int DBFacade::Delete(QString usrID)
{
    int myerrno = 0;
    if (database.transaction()) {   // 开启事务

        QString cmd = QString("delete from %1 where usrID = :usrID").arg(table_name);
        sql.prepare(cmd);
        sql.bindValue(":usrID", usrID);
        if (sql.exec()) {           // 执行成功
            PrintInfo(QString("delete succeed: %1").arg(usrID));
            myerrno = 0;
        } else {                    // 执行失败
            PrintInfo("delete failed: " + sql.lastError().databaseText());
            myerrno = 3;
        }

        if (!database.commit()) {   // 事务提交失败
            database.rollback();    // 回滚事务
            myerrno = 2;
        }
    } else {                        // 事务开启失败
        myerrno = 1;
    }
    return myerrno;
}

int DBFacade::Query(QString usrID, QString&password)
{
    int myerrno = 0;
    if (database.transaction()) {   // 开启事务

        QString cmd = QString("select password from %1 where usrID = :usrID").arg(table_name);
        sql.prepare(cmd);
        sql.bindValue(":usrID", usrID);
        if (sql.exec()) {           // 执行成功
            bool empty = sql.next();
            if (empty == true) {    // 查询成功
                password = sql.value(0).toString();
                PrintInfo(QString("query %1 succeed: %2").arg(usrID, password));
                myerrno = 0;
            } else {                // 没有数据
                PrintInfo(QString("query %1 failed: %2").arg(usrID, "no data"));
                myerrno = 4;
            }
        } else {                    // 执行失败
            PrintInfo("query failed: " + sql.lastError().databaseText());
            myerrno = 3;
        }

        if (!database.commit()) {   // 事务提交失败
            database.rollback();    // 回滚事务
            myerrno = 2;
        }
    } else {                        // 事务开启失败
        myerrno = 1;
    }
    return myerrno;
}

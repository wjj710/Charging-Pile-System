#include "pileinfocontroller.h"
#include "global.h"
//目前loop还没用明白，就先注释掉了
std::string PileInfoController::generatePileInfo(std::string pileNo)    //生成某充电桩状态信息，供服务器调用
{
    //根据pileNo获取充电桩的socket，给它发请求消息select
    qintptr descriptor=Global::mstr2Int.at(pileNo);
    QString msg="select/0\t";
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toUtf8().data());
            item->flush();
            qDebug()<<"send to pile: "<<descriptor<<msg;
            break;
        }
    }
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //Global::loop.exec();
    QString ans = Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    return ans.toStdString();
}
std::string PileInfoController::generateCarInfo(std::string pileNo) //生成某充电桩排队车辆信息，供服务器调用
{
    qintptr descriptor=Global::mstr2Int.at(pileNo);
    QString msg="select/1\t";
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toUtf8().data());
            item->flush();
            qDebug()<<"send to pile: "<<descriptor<<msg;
            break;
        }
    }
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //Global::loop.exec();
    QString ans = Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    return ans.toStdString();
}
std::string PileInfoController::getReport(std::string pileNo)//生成报表，供服务器调用
{
    qintptr descriptor=Global::mstr2Int.at(pileNo);
    QString msg="select/2\t";
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toUtf8().data());
            item->flush();
            qDebug()<<"send to pile: "<<descriptor<<msg;
            break;
        }
    }
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //Global::loop.exec();
    QString ans = Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    return ans.toStdString();
}

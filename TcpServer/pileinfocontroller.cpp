#include "pileinfocontroller.h"
#include "global.h"
#include "pileinfo.h"
#include <stdlib.h>
#include <string>
void PileInfoController::generatePileInfo(std::string pileNo, int admin_socketNo)    //生成某充电桩状态信息，供服务器调用
{
    //根据pileNo获取充电桩的socket，给它发请求消息select
    qintptr descriptor;
    QString msg="select/0\t";

    descriptor=Global::mstr2Int.at(pileNo);
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
    //Global::loopNo=2;
    //loop.exec();
    Global::mutex.lock();
    Global::condition.wait(&Global::mutex);
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    //PileInfo* pileinfo=reinterpret_cast<PileInfo *>(Global::bytebuffer[1].data());
    //qDebug()<<pileinfo->workingState<<" "<<pileinfo->totalChargingNumber<<" "<<pileinfo->totalChargingTime<<" "<<pileinfo->totalChargingCapacity;
    qDebug()<<Global::bytebuffer[1];
    sendArrayMsg(Global::bytebuffer[1], admin_socketNo);
    Global::mutex.unlock();
}

void PileInfoController::generateCarInfo(std::string pileNo,int admin_socketNo) //生成某充电桩排队车辆信息，供服务器调用
{
    qintptr descriptor;
    QString msg="select/1\t";

    descriptor=Global::mstr2Int.at(pileNo);
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
    //Global::loopNo=2;
    //loop.exec();
    Global::mutex.lock();
    Global::condition.wait(&Global::mutex);
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    //PileInfo* pileinfo=reinterpret_cast<PileInfo *>(Global::bytebuffer[1].data());
    //qDebug()<<pileinfo->workingState<<" "<<pileinfo->totalChargingNumber<<" "<<pileinfo->totalChargingTime<<" "<<pileinfo->totalChargingCapacity;
    qDebug()<<Global::bytebuffer[1];
    sendArrayMsg(Global::bytebuffer[1].prepend("yes/"), admin_socketNo);
    Global::mutex.unlock();
}
void PileInfoController::getReport(std::string pileNo,int admin_socketNo)//生成报表，供服务器调用
{
    qintptr descriptor;
    QString msg="select/2\t";

    descriptor=Global::mstr2Int.at(pileNo);
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
    //Global::loopNo=2;
    //loop.exec();
    Global::mutex.lock();
    Global::condition.wait(&Global::mutex);
    //给充电桩发完请求后，阻塞等待充电桩回复
    //回复消息收到，存在buffer中
    //ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
    //PileInfo* pileinfo=reinterpret_cast<PileInfo *>(Global::bytebuffer[1].data());
    //qDebug()<<pileinfo->workingState<<" "<<pileinfo->totalChargingNumber<<" "<<pileinfo->totalChargingTime<<" "<<pileinfo->totalChargingCapacity;
    qDebug()<<Global::bytebuffer[1];
    sendArrayMsg(Global::bytebuffer[1], admin_socketNo);
    Global::mutex.unlock();
}

void PileInfoController::sendArrayMsg(QByteArray msg, int descriptor){//向指定描述符对应的socket发消息
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg, msg.size());
            item->flush();
            qDebug()<<"send to admin: "<<descriptor<<msg;
            break;
        }
    }
}


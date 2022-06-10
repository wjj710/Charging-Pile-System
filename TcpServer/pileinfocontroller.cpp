#include "pileinfocontroller.h"
#include "global.h"
#include <stdlib.h>
std::string PileInfoController::generatePileInfo(std::string pileNo)    //生成某充电桩状态信息，供服务器调用
{
    //根据pileNo获取充电桩的socket，给它发请求消息select
    qintptr descriptor;
    QString msg="select/0\t";
    QString ans="yes/";
    if(pileNo=="0")
    {
        for(int i=1; i<=Global::fastChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("F"+QString::number(i)).toStdString());
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
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
        for(int i=1; i<=Global::trickleChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("T"+QString::number(i)).toStdString());
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
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
    }
    else
    {
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
        ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
        Global::mutex.unlock();
    }
    qDebug()<<ans;
    return ans.toStdString();
}
std::string PileInfoController::generateCarInfo(std::string pileNo) //生成某充电桩排队车辆信息，供服务器调用
{
    qintptr descriptor;
    QString msg="select/1\t";
    QString ans="yes/";
    if(pileNo=="0")
    {
        for(int i=1; i<=Global::fastChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("F"+QString::number(i)).toStdString());
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
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
        for(int i=1; i<=Global::trickleChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("T"+QString::number(i)).toStdString());
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
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
    }
    else
    {
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
       // Global::loopNo=2;
        //loop.exec();
        Global::mutex.lock();
        Global::condition.wait(&Global::mutex);
        //给充电桩发完请求后，阻塞等待充电桩回复
        //回复消息收到，存在buffer中
        ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
        Global::mutex.unlock();
    }
    qDebug()<<ans;
    return ans.toStdString();
}
std::string PileInfoController::getReport(std::string pileNo)//生成报表，供服务器调用
{
    qintptr descriptor;
    QString msg="select/2\t";
    QString ans="yes/";
    if(pileNo=="0")
    {
        for(int i=1; i<=Global::fastChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("F"+QString::number(i)).toStdString());
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
           // Global::loopNo=2;
            //loop.exec();
            Global::mutex.lock();
            Global::condition.wait(&Global::mutex);
            //给充电桩发完请求后，阻塞等待充电桩回复
            //回复消息收到，存在buffer中
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
        for(int i=1; i<=Global::trickleChargingPileNum; i++)
        {
            descriptor=Global::mstr2Int.at(("T"+QString::number(i)).toStdString());
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
           // Global::loopNo=2;
            //loop.exec();
            Global::mutex.lock();
            Global::condition.wait(&Global::mutex);
            //给充电桩发完请求后，阻塞等待充电桩回复
            //回复消息收到，存在buffer中
            ans += Global::buffer[1]; //返回给服务器的消息，让服务器转发给管理员
            Global::mutex.unlock();
        }
    }
    else qDebug()<<"no/error\t";
    return ans.toStdString();
}

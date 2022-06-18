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
void PileInfoController::getWaiting(int admin_socketNo)//返回等候区的车辆信息，即l1和l_priority
{
  /*  Request r={1,"test",0,0,0,0,0,0,0,0,0};  //测试举例
    Request rr={2,"test",1,0,0,0,0,0,0,0,0};
    Global::l1.append(r);
    Global::l_priority.append(rr);
    Global::mq2v["T1"]=222;
    Global::mq2v["F2"]=666;*/
    std::string ans="yes/";
    for(int i=0; i<Global::l_priority.size(); i++){
        CarInfo car;
        QString mode="";
        if(Global::l_priority[i].chargingMode==0) mode+="T";
        else mode+="F";
        mode += QString::number(Global::l_priority[i].queueNum);
        strcpy(car.ownerID, Global::usr[0].getID().toStdString().c_str());
        car.vNum = Global::mq2v.at(mode.toStdString());
        car.batteryCapacity=Global::l_priority[i].batteryCapacity;
        car.requestChargingCapacity = Global::l_priority[i].requestChargingCapacity;
        car.queueTime = time(0)-Global::l_priority[i].requestTime;
        car.queueNum=Global::l_priority[i].queueNum;
        car.chargingMode = Global::l_priority[i].chargingMode;
        car.alreadyChargingCapacity=0;
        car.nowFee=0;
        std::string k((char *)(&car), sizeof(CarInfo));
        ans+=k;
    }
    for(int i=0; i<Global::l1.size(); i++){
        CarInfo car;
        QString mode="";
        if(Global::l1[i].chargingMode==0) mode+="T";
        else mode+="F";
        mode += QString::number(Global::l1[i].queueNum);
        strcpy(car.ownerID, Global::usr[0].getID().toStdString().c_str());
        car.vNum = Global::mq2v.at(mode.toStdString());
        car.batteryCapacity=Global::l1[i].batteryCapacity;
        car.requestChargingCapacity = Global::l1[i].requestChargingCapacity;
        car.queueTime = time(0)-Global::l1[i].requestTime;
        car.queueNum=Global::l1[i].queueNum;
        car.chargingMode = Global::l1[i].chargingMode;
        car.alreadyChargingCapacity=0;
        car.nowFee=0;
        std::string k((char *)(&car), sizeof(CarInfo));
        ans+=k;
    }
    sendArrayMsg(QString::fromStdString(ans).toLatin1(), admin_socketNo);
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


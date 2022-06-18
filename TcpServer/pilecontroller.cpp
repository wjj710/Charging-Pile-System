//
// Created by Tony Zhao on 2022/5/28.
//

#include "pilecontroller.h"
//#include "pile.h"
#include "global.h"
#include "request.h"
#include <QString>
#include <QList>
#include <queue>
#include <vector>
#include "querycontroller.h"

void PileController::call(std::string pileNo) {
    try {
        Request req = getRequest(pileNo);
        std::string req_str((char *)(&req), sizeof(req));
        std::string ret = "insertIntoPileList/" + req_str + "\t";
        sendMsg(QString::fromStdString(ret), Global::mstr2Int[pileNo]);
        //sendMsg("waitingToCharging\t",Global::mstr2Int[req.ownerID]);
        QueryController::getUserByID(Global::usr[0].getID()).changeState("charging");
    } catch (...) {
        Global::m_queue[pileNo]++;
    }
}

Request PileController::getRequest(std::string pileNo) {
    for(QList<Request>::iterator it = Global::l_priority.begin(); it != Global::l_priority.end(); ++it) {
        if(it->chargingMode == (pileNo[0] == 'F')) {
            Request ret = *it;
            Global::l_priority.erase(it);
            return ret;
        }
    }
    for(QList<Request>::iterator it = Global::l1.begin(); it != Global::l1.end(); ++it) {
        if(it->chargingMode == (pileNo[0] == 'F')) {
            Request ret = *it;
            Global::l1.erase(it);
            return ret;
        }
    }
    throw "No Available Request.";
}

bool PileController::cmpRequest(Request x, Request y) {
    return x.requestTime < y.requestTime;
}

void PileController::malfunction(std::string pileNo, int mode) {
    scheduleMode = mode == 3 ? 0 : mode;
    if(mode==1) {
        Global::m_on[pileNo] = false;
        QList<Request> reqs = getAllRequestFromPile(pileNo);
        for(const auto &req : reqs) {
            handleNewRequest(req, Global::l_priority);
        }
    }
    if(mode==2||mode==3) {
        Global::m_on[pileNo] = mode==2?false:true;
        QList<Request> l_req;
        for(const auto &it: Global::lp) {
            if(Global::m_on[it] && it[0] == pileNo[0]) {
                QList<Request> reqs = getAllRequestFromPile(it);
                for(const auto &req : reqs) {
                    Global::m_queue[it]++;
                    l_req.append(req);
                }
            }
        }
        std::sort(l_req.begin(), l_req.end(), cmpRequest);
        for(const auto &req : l_req) {
            handleNewRequest(req, Global::l_priority);
        }
    }
    return;
}

QList<Request> PileController::getAllRequestFromPile(std::string pileNo) {
    sendMsg("select/3\t",Global::mstr2Int[pileNo]);
    Global::mutex.lock();
    Global::condition.wait(&Global::mutex);
    QList<Request> ret;
    if(Global::res=="yes"&&Global::bytebuffer.size()) {
        for(Request *tmp=(Request *)(Global::bytebuffer.begin()); tmp<(Request *)(Global::bytebuffer.end()); tmp+=sizeof(Request)) {
            Global::m_queue[pileNo]++;
            ret.append(*tmp);
        }
    }
    Global::mutex.unlock();
    return ret;
}

// Handle New Request.
QString PileController::handleNewRequest(Request req, QList<Request> & fallback_list) {
    std::string pileNo = getIdlePile(req.chargingMode);
    if(pileNo!="") {
        std::string req_str((char *)(&req), sizeof(req));
        std::string ret = "insertIntoPileList/" + req_str + "\t";
        sendMsg2(QString::fromStdString(ret), Global::mstr2Int[pileNo]);
    }
    else {
        fallback_list.append(req);
        qDebug()<<"V"<<req.vNum<<" "<<req.requestChargingCapacity<<" "<<req.batteryCapacity<<"\n";
    }
    return QString::fromStdString(pileNo);
}

// Get PileNo from m_queue. Return "" if Pile Not Found.
std::string PileController::getIdlePile(int isFastCharge) {
    int l = 0;
    std::string ret = "";
    for(const auto &it : Global::m_queue) {
        if(isFastCharge==(it.first[0]=='F')&&it.second>l&&Global::m_on[it.first]) {
            ret=it.first;
            l=it.second;
        }
    }
    if(l) Global::m_queue[ret]--;
    return ret;
}

void PileController::sendMsg(QString msg, int descriptor) {
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toLatin1().data(),msg.size());
            item->flush();
            qDebug()<<"send to pile: "<<descriptor<<msg;
            break;
        }
    }
}

void PileController::sendMsg2(QString msg, int descriptor) {
    for(int i = 0; i < Global::tcpclientsocketlist.count(); i++)
    {
        QTcpSocket *item = Global::tcpclientsocketlist.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            item->write(msg.toLatin1().data(),msg.size());
            item->flush();
            qDebug()<<"send to pile: "<<descriptor<<msg;
            break;
        }
    }
}

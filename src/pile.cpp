#include "../headers/Pile.h"
#include "../headers/PileInfo.h"
#include <stdlib.h>

extern HANDLE wMutex;

Pile::Pile(char *argv[])
{
    pileNo=argv[1];
    workingState=0;
    power=atof(argv[2]);
    chargingQueueLen=atoi(argv[3]);
    totalChargingTime=0;
    totalChargingNumber=0;
    totalChargingFee=0;
    totalChargingCapacity=0;
}

string Pile::turnOn()
{
    if(!workingState){ //如果已经打开了
        return "no/already turned on\t";
    }else if(workingState==2){ //如果出故障了
        return "no/pile malfunction\t";
    }else{
        workingState=0;
        return "yes\t";
    }
}

string Pile::turnOff()
{
    if(workingState==1){
        return "no/already turned off\t";
    }else if(workingState==2){
        return "no/pile malfunction\t";
    }else{
        workingState=1;
        return "yes\t";
    }
}

string Pile::insertIntoPileList(Request r1)
{
    if(workingState==1){
        return "no/already turned off\t";
    }else if(workingState==2){
        return "no/pile malfunction\t";
    }
    WaitForSingleObject(wMutex, INFINITE);
    chargingQueue.push_back(r1);
    ReleaseMutex(wMutex);
    return "yes\t";
}

string Pile::removeFromPileList(DWORD threadid, int qNum)
{
    if(workingState==1){
        return "no/already turned off\t";
    }else if(workingState==2){
        return "no/pile malfunction\t";
    }

    for(int i=0; i<chargingQueue.size(); i++){
        if(chargingQueue[i].queueNum==qNum){
            if(i==0){ //说明此时该充电请求正在被处理，不能直接删除
                PostThreadMessage(threadid,WM_TIMER,0,qNum);
            }else{
                WaitForSingleObject(wMutex, INFINITE);
                chargingQueue.erase(chargingQueue.begin()+i);
                ReleaseMutex(wMutex);
            }
            return "yes\t";
        }
    }
    return "no/request does not exist\t";
}

string Pile::isEmpty()
{
    if(chargingQueue.size()){ //队列不是空的
        return "yes/0\t";
    }else{ //队列是空的
        return "yes/1\t";
    }
}

string Pile::select(int mode)
{
    string s="yes/";
    if(mode==0){
        PileInfo p={workingState,totalChargingNumber,totalChargingTime, totalChargingCapacity};
        string k((char *)(&p), sizeof(PileInfo));
        s+=(k+"\t");
    }else if(mode==1){
        WaitForSingleObject(wMutex, INFINITE);
        for(int i=0; i<chargingQueue.size(); i++){
            CarInfo c={chargingQueue[i].ownerID, chargingQueue[i].batteryCapacity, 
                       chargingQueue[i].requestChargingCapacity, time(0)-chargingQueue[i].requestTime};
            string k((char *)(&c), sizeof(CarInfo));
            s+=k;
        }
        ReleaseMutex(wMutex);
        s+="\t";
    }else if(mode==2){
        ReportInfo r={pileNo, totalChargingNumber, totalChargingTime, totalChargingCapacity,
                      totalChargingFee, 0.8*totalChargingCapacity, totalChargingFee+0.8*totalChargingCapacity};
        string k((char *)(&r), sizeof(ReportInfo));
        s+=(k+"\t");
    }else if(mode==3){
        vector<Request> v;
        if(workingState==0){
            if(chargingQueue.size()>1){
                WaitForSingleObject(wMutex, INFINITE);
                v.assign(chargingQueue.begin()+1,chargingQueue.end());
                chargingQueue.erase(chargingQueue.begin()+1,chargingQueue.end());
                ReleaseMutex(wMutex);
            }
        }else{
            if(chargingQueue.size()>0){
                WaitForSingleObject(wMutex, INFINITE);
                v.assign(chargingQueue.begin(),chargingQueue.end());
                chargingQueue.erase(chargingQueue.begin(),chargingQueue.end());
                ReleaseMutex(wMutex);
            }
        }
        string k((char *)(&v), v.size()*sizeof(Request));
        s+=(k+"\t");
    }else{
        s="no/mode error\t";
    }
    return s;
}

double Pile::calculateFee(time_t start, time_t end)
{
    double fee[24]={0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.7,0.7,0.7,1.0,1.0,
                    1.0,1.0,1.0,0.7,0.7,0.7,1.0,1.0,1.0,0.7,0.7,0.4}; //记录24个小时的电价
    int hs=(start/3600)%24; //开始时的小时值
    int he=(end/3600)%24;   //终止时的小时值
    int k=hs;
    double r=0; //记录费用
    while(k!=he){
        r+=fee[k]*power;
        k=(k+1)%24;
    }
    double fs=(start%3600)*power*fee[hs]/3600;
    double fe=(end%3600)*power*fee[he]/3600;
    if(he==hs){
        r=fe-fs;
    }else{
        r+=(fe-fs);
    }
    return r;
}
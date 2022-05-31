#include "../headers/Request.h"

Request::Request(int qNum, string oID, int cMode, double rChargingCapacity, double bCapacity)
{
    queueNum=qNum;
    ownerID=oID;
    chargingMode=cMode;
    requestChargingCapacity=rChargingCapacity;
    batteryCapacity=bCapacity;
}
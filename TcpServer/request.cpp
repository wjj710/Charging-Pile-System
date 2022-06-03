#include "request.h"

Request::Request(int qNum, std::string oID, int cMode, double rChargingCapacity, double bCapacity)
{
    queueNum=qNum;
    ownerID=oID;
    chargingMode=cMode;
    requestChargingCapacity=rChargingCapacity;
    batteryCapacity=bCapacity;
}

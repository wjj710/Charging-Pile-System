#ifndef GLOBAL_H
#define GLOBAL_H
#include <QList>
#include <map>
#include <string>
#include <QEventLoop>
#include "request.h"
#include "user.h"
#include"tcpclientsocket.h"
//using namespace std;

//全局变量类，使用全局变量时在变量名前加"Global::"，如Global::waitingAreaSize，同时要include "global.h"
class Global
{
public:
    Global();
    static std::map<std::string,int> mstr2Int; //用来把用户的ID和对应的socket标识符联系起来，也把充电桩的pileNo和对应的socket标识符联系起来
    static std::map<int,std::string> mint2Str; //与上面的map功能相反
    static QList<QStringList> handleList; //处理队列
    static QStringList buffer;  //存放充电桩给服务器的返回消息
    static QList<TcpClientSocket*> tcpclientsocketlist; //服务器中的socket队列
    static int waitingAreaSize; //等候区大小
    static int chargingQueueLen; //充电桩队列长度
    static int fastChargingPileNum; //快充电桩数目
    static int trickleChargingPileNum; //慢充电桩数目
    static QList<Request> l1; //等候区队列
    static QList<Request> l2; //完成队列, 寻找详单信息的时候从后向前根据ownerID找到用户最近完成的请求
    static QList<User> usr;   //用户队列，根据用户的ID找到对应的用户类
    static QList<std::string> lp;   //充电桩队列(包括快充和慢充），里面的string是pileNo，需要充电桩类的时候直接用pileNo找到socket描述符，给充电桩进程发消息
    static QList<Request> l_priority; //优先级队列，因为是全局变量，初始化之前要先清空
    static QList<Request> l_time; //时间顺序队列，初始化之前要先清空
    static std::string malfunctionPileNo; //故障充电桩的编号，如果此时没有故障充电桩就是空字符串
    static QEventLoop loop; //事件循环，用于同步通信
};

#endif // GLOBAL_H

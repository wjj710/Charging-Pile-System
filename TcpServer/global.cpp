#include "global.h"

Global::Global()
{

}

std::map<std::string,int> Global::mstr2Int; //用来把用户的ID和对应的socket标识符联系起来，也把充电桩的pileNo和对应的socket标识符联系起来
std::map<int,std::string> Global::mint2Str; //与上面的map功能相反
std::map<std::string,int> Global::mq2v;
time_t Global::t; //测试基准时间
time_t Global::t1; //样例基准时间
QList<QStringList> Global::handleList; //处理队列
QStringList Global::buffer;  //存放充电桩给服务器的返回消息
QList<QByteArray> Global::bytebuffer;//存放充电桩返回消息，保留原格式
QList<TcpClientSocket*> Global::tcpclientsocketlist; //服务器中的socket队列
int Global::waitingAreaSize=0; //等候区大小
int Global::chargingQueueLen=0; //充电桩队列长度
int Global::fastChargingPileNum=0; //快充电桩数目
int Global::trickleChargingPileNum=0; //慢充电桩数目
QList<Request> Global::l1; //等候区队列
QList<Request> Global::l2; //完成队列
QList<User> Global::usr;   //用户队列
QList<std::string> Global::lp;   //充电桩队列，里面的int是socket的描述符，需要充电桩类的时候直接用此描述符给充电桩进程发消息
QList<Request> Global::l_priority; //优先级队列，因为是全局变量，初始化之前要先清空
QList<Request> Global::l_time; //时间顺序队列，初始化之前要先清空
std::string Global::malfunctionPileNo; //故障充电桩的编号，如果此时没有故障充电桩就是空字符串
//int Global::loopNo; //事件循环编号，用于同步通信
QList<QProcess *> Global::proclist; //充电桩进程队列
QWaitCondition Global::condition;
QMutex Global::mutex;
QList<std::string> Global::l_call; // 存放未得到反馈的充电桩编号

std::map<std::string,int> Global::m_queue;
std::map<std::string,bool> Global::m_on;

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include "headers/Pile.h"
#define MAXLEN 1000
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

Pile *p; //充电桩指针
SOCKET sock; //通信套接字
HANDLE hMutex = CreateMutex(NULL,FALSE,NULL); //互斥锁，用来保证两个线程对sock的互斥访问
HANDLE wMutex = CreateMutex(NULL,FALSE,NULL); //互斥锁，用来保证两个线程对chargingQueue的互斥访问

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    //刚开始队列中都是空的，所以发送多个叫号请求
    for(int i=0; i<p->chargingQueueLen; i++){
        char *s="call\t";
        WaitForSingleObject(hMutex, INFINITE);
        send(sock,s,strlen(s),NULL);
        ReleaseMutex(hMutex);
    }
    while(1){
        //如果充电桩队列为空，就一直等待
        while(!p->chargingQueue.size());
        //如果队列不为空，就依次处理
        for(int i=0; i<p->chargingQueue.size(); i++){
            //如果是关机或故障状态，则一直等待
            while(p->workingState); 
            p->chargingQueue[i].startChargingTime=time(0); //记录开始充电的时间
            UINT timesec=(p->chargingQueue[i].requestChargingCapacity)*3600/p->power; //记录充电预计秒数
            BOOL bRet = FALSE;
            MSG msg = {0};
            UINT timerid = SetTimer(NULL, 0, timesec*1000, NULL);
            while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){
                if (bRet == -1){ 
                    cout<<"timer error"<<endl; 
                    return -1;
                }else{
                    //timer消息到达
                    if (msg.message == WM_TIMER){
                        if (msg.wParam == timerid){
                            cout<<"timer reaching"<<endl;
                        }else{
                            if(msg.lParam == p->chargingQueue[i].queueNum){
                                cout<<"charging interupt"<<endl;
                            }else{
                                continue;
                            }
                        }
                        break;
                    }else{
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
            }
            KillTimer(NULL, timerid); //关计时器
            p->chargingQueue[i].endChargingTime=time(0); //记录结束充电的时间
            time_t start = p->chargingQueue[i].startChargingTime;
            time_t end = p->chargingQueue[i].endChargingTime;
            p->chargingQueue[i].chargingCapacity = (end-start)*(p->power)/3600; //记录实际充电量
            p->chargingQueue[i].serviceFee = 0.8 * p->chargingQueue[i].chargingCapacity; //计算服务费用
            p->chargingQueue[i].chargingFee = p->calculateFee(start, end); //计算充电费用
            p->totalChargingFee += p->chargingQueue[i].chargingFee; //累加充电费用
            p->totalChargingNumber++; //累加充电次数
            p->totalChargingTime += (end-start); //累加充电时间
            p->totalChargingCapacity += p->chargingQueue[i].chargingCapacity; //累加充电量
            //发送叫号请求并将此请求从队列中删除
            char *k="call/";
            char s[6+sizeof(Request)];
            memcpy(s,k,5);
            memcpy(s+5,(&p->chargingQueue[i]),sizeof(Request));
            s[5+sizeof(Request)]='\t';
            WaitForSingleObject(hMutex, INFINITE);
            send(sock,s,strlen(s)+1,NULL);
            ReleaseMutex(hMutex);
            WaitForSingleObject(wMutex, INFINITE);
            p->chargingQueue.erase(p->chargingQueue.begin()+i);
            ReleaseMutex(wMutex);
            i--;
        }
    }
}

//创建充电桩进程时需要传入三个参数：充电桩编号、充电桩功率和队列长度
int main(int argc, char *argv[])
{
    /*初始化*/
    if(argc!=4){
        cout<<"args format error"<<endl;
        return 1;
    }
    //创建充电桩对象
    p=new Pile(argv);

    /*与服务器建立连接*/

    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起连接请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(8080);  //端口
    int ret=connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    if(ret){
        cout<<"connect error"<<endl;
        exit(1);
    }
    string info="pileLogon/"+p->pileNo+"\t";
    WaitForSingleObject(hMutex, INFINITE);
    send(sock,info.c_str(),info.size(),NULL);
    ReleaseMutex(hMutex);

    /*开始处理流程*/

    //首先创建一个线程用来对等候队列中的车充电，同时发送叫号请求
    DWORD threadid;
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &threadid);
    CloseHandle(thread);

    //主线程用来接收服务器发来的请求，并进行相应的处理
    char sz[MAXLEN] = {0};
    while(ret=recv(sock, sz, MAXLEN, NULL)){
        if(ret<0){
            cout<<"receive information error"<<endl;
            exit(1);
        }else{
            int i=0; //字符串指针
            while(i<MAXLEN){
                if(!isalpha(sz[i])) break; //不是有效报文头，退出循环
                int sth=i; //记录报文的起始位置
                while(sz[i]!='/'&&sz[i]!='\t') i++;
                char h[50]; //用来存放报头
                char b[500]; //用来存放参数
                char c=sz[i];
                sz[i++]='\0'; //移动指针并且将报头末尾置为\0
                strcpy(h,sz+sth);
                if(c=='/'){ //说明有参数
                    int count=0; //记录参数占多少个字节
                    int stb=i; //记录参数的起始位置
                    while(sz[i]!='\t'){
                        i++;
                        count++;
                    }
                    i++; //指针移到下一个报文的开始处
                    memcpy(b,sz+stb,count);
                }
                string s=h;
                string ret=""; //用来存放调用方法的返回值
                if(s=="turnOn"){
                    ret=p->turnOn();
                }else if(s=="turnOff"){
                    ret=p->turnOff();
                }else if(s=="insertIntoPileList"){
                    ret=p->insertIntoPileList(*(Request *)b);
                }else if(s=="removeFromPileList"){
                    ret=p->removeFromPileList(threadid, atoi(b));
                }else if(s=="isEmpty"){
                    ret=p->isEmpty();
                }else if(s=="select"){
                    ret=p->select(atoi(b));
                }else if(s=="malfunction"){
                    if(p->workingState==2){ 
                        ret="no/malfunction already happened\t";
                    }else{
                        if(p->chargingQueue.size()){
                            PostThreadMessage(threadid,WM_TIMER,0,p->chargingQueue[0].queueNum);
                        }
                        ret="yes\t";
                        p->workingState=2;
                    }
                }else if(s=="malfunctionRecover"){
                    if(p->workingState!=2){ 
                        ret="no/malfunction already recovered\t";
                    }else{
                        p->workingState=0;
                        ret="yes\t";
                    }
                }else{
                    ret="no/no such request:"+to_string(i)+"\t";
                    cout<<"request error"<<endl;
                }
                //向服务器发响应
                char bf[ret.size()];
                memcpy(bf,ret.c_str(),ret.size());
                WaitForSingleObject(hMutex, INFINITE);
                send(sock,bf,ret.size(),NULL);
                ReleaseMutex(hMutex);
            }
            memset(sz,0,MAXLEN);
        }
    }
    return 0;
}
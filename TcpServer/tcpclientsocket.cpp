#include "tcpclientsocket.h"
#include <QDebug>
#include <ctype.h>

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    // 接收到数据包时调用receivedata函数
    connect(this, &TcpClientSocket::readyRead, this, &TcpClientSocket::receivedata);
    // 断开连接时，调用slotclientdisconnected函数，该函数发送clientdisconnected信号。
    connect(this, &TcpClientSocket::disconnected, this, &TcpClientSocket::slotclientdisconnected);
}
/**
 * @brief TcpClientSocket::receivedata 客户端发来数据包时被调用；在该函数中，根据发来的数据包包头信息，调用不同控制器的不同方法。
 */
void TcpClientSocket::receivedata()
{
    QByteArray array = readAll();
    qDebug()<<"receive bytearray"<<array<<Qt::endl;
    QList<QByteArray> arrayList = array.split('\t'); //分解各个报文
    QList<QByteArray> aList;
    int flag=0; //如果是第一个包，则flag为0
    QByteArray res="";
    for(int i=0; i<arrayList.size();i++){
        QByteArray q1=arrayList.at(i);
        int k=0; //记录前面0字节的个数
        for(int i=0;i<q1.size();i++){
            if(q1[i]!='\0'){
                k=i;
                break;
            }
        }
        QByteArray q;
        if(q1.size()-k>2&&isalpha(q1.data()[k])&&isalpha(q1.data()[k+1])){
            q=q1.right(q1.size()-k); //如果是下一个包，去掉前面的0字节
        }else{
            q=q1;
        }
        if(q=="") continue;
        if(q.size()>=2&&isalpha(q.data()[0])&&isalpha(q.data()[1])){ //如果前两个字符都是字母，说明是包头
            if(flag){ //说明不是第一个包，要把前一个包加入aList
                aList.append(res);
            }else{
                flag=1;
            }
            res=q;
        }else{ //否则是包体，加入res中
            res+=('\t'+q);
        }
    }
    aList.append(res);

//    QString msg = array;
//    QStringList msgList = msg.split("\t"); //分解各个报文
    for(int i=0; i<aList.size(); i++){
        QByteArray pre_ans=aList.at(i);
        if(pre_ans.length()!=0){
            emit showserver(pre_ans, peerAddress(), peerPort(),0);// 从客户端接受，末尾设为0
            emit sendserver(pre_ans,this->socketDescriptor()); //将报文发送给服务器
        }
    }
}

void TcpClientSocket::slotclientdisconnected()
{
    emit clientdisconnected(this->socketDescriptor());
}

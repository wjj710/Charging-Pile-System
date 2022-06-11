#include "tcpclientsocket.h"
#include <QDebug>

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
    QString msg = array;
    QStringList msgList = msg.split("\t"); //分解各个报文
    for(int i=0; i<msgList.size(); i++){
        QString ans=msgList.at(i);
        if(ans.length()!=0){
            emit showserver(ans, peerAddress(), peerPort(),0);// 从客户端接受，末尾设为0
            emit sendserver(ans,this->socketDescriptor()); //将报文发送给服务器
        }
    }
}

void TcpClientSocket::slotclientdisconnected()
{
    emit clientdisconnected(this->socketDescriptor());
}

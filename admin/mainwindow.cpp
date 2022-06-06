#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Fmode->setChecked(1);
    ui->Finfo->setChecked(1);
    btnGroup1 = new QButtonGroup;
    btnGroup1->addButton(ui->Fmode,0);
    btnGroup1->addButton(ui->Tmode,1);
    btnGroup2 = new QButtonGroup;
    btnGroup2->addButton(ui->Finfo,0);
    btnGroup2->addButton(ui->Tinfo,1);
}

MainWindow::~MainWindow()
{
    allOver=0;
    delete this->socket;
    delete ui;
}
void MainWindow::self_init()
{
    allButtonOff();
    socket = new QTcpSocket();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    QString IP = "127.0.0.1";
    int port = 8080;
    //取消已有的连接
    socket->abort();
    //连接服务器
    socket->connectToHost(IP, port);
    //等待连接成功
    if(!socket->waitForConnected(30000))
    {
        ui->IPLab->setText("IP: 连接失败");
        ui->PortLab->setText("Port: 连接失败");
        qDebug() << "Connection failed!";
        return;
    }
    qDebug() << "Connect successfully!";
    ui->IPLab->setText("IP: "+IP);
    ui->PortLab->setText("Port: "+QString::number(port));
    //发出初始化设置消息
    QString message="adminLogon\t";
    socket->write(message.toLatin1());
    socket->flush();
    loop.exec(); //阻塞，等待收到回复消息解除
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        FastCharingPileNum=list[1].toInt();
        TrickleChargingPileNum=list[2].toInt();
        WaitingAreaSize=list[3].toInt();
        ChargingQueueLen=list[4].toInt();
        ui->FastNum->setText(list[1]);
        ui->SlowNum->setText(list[2]);
        ui->WaitingNum->setText(list[3]);
        ui->ChargingNum->setText(list[4]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取充电桩配置信息失败！\n原因：%1").arg(list[1]));
    }
    allButtonOn();
}
void MainWindow::on_On_clicked()
{
    QString message="turnOnPile/";
    QString pileNo=QString::number(ui->OnNo->value());
    QString mode;
    if(btnGroup1->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("开启充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("开启充电桩失败！\n原因：%1").arg(list[1]));
    }

}
void MainWindow::on_Off_clicked()
{
    QString message="turnOffPile/";
    QString pileNo=QString::number(ui->OnNo->value());
    QString mode;
    if(btnGroup1->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("关闭充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("关闭充电桩失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_AllOn_clicked()
{
    QString message="turnOnPile/";
    QString pileNo=QString::number(0);
    message=message+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("开启全部充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("开启全部充电桩失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_AllOff_clicked()
{
    QString message="turnOffPile/";
    QString pileNo=QString::number(0);
    message=message+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("关闭全部充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("关闭全部充电桩失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_GetState_clicked()
{
    QString message="getPileInfo/";
    QString pileNo=QString::number(ui->infoNo->value());
    QString mode;
    if(btnGroup2->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        ui->State->setText(list[1]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取充电桩状态失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_GetAllState_clicked()
{
    QString message="getPileInfo/";
    QString pileNo=QString::number(0);
    message=message+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        ui->State->setText(list[1]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取全部充电桩状态失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_GetInfo_clicked()
{
    QString message="getCarInfo/";
    QString pileNo=QString::number(ui->CarNo->value());
    QString mode;
    if(btnGroup2->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        ui->Info->setText(list[1]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取等候车辆信息失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_GetAllInfo_clicked()
{
    QString message="getCarInfo/";
    QString pileNo=QString::number(0);
    message=message+pileNo+"\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        ui->Info->setText(list[1]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取全部等候车辆信息失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::on_GetReport_clicked()
{
    QString message="getReport\t";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QString msg = socket->readAll();
    msg.chop(1);
    qDebug()<<msg;
    QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        ui->Report->setText(list[1]);
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("获取报表失败！\n原因：%1").arg(list[1]));
    }
}
void MainWindow::socket_Read_Data()
{
    qDebug()<<"receive";
    loop.exit();
    allButtonOn();
}
void MainWindow::socket_Disconnected()
{
    if(allOver != 0)
    {
        QMessageBox::critical(this, tr("出错"),  tr("与服务器断开连接！！！\n请关闭重新连接。"));
        ui->IPLab->setText("IP: 连接中断");
        ui->PortLab->setText("Port: 连接中断");
        qDebug() << "Connection break!";
    }
}
void MainWindow::allButtonOff()
{
    ui->errorLab->setText("状态：等待服务器响应中。。。");
    ui->On->setEnabled(false);
    ui->Off->setEnabled(false);
    ui->AllOn->setEnabled(false);
    ui->AllOff->setEnabled(false);
    ui->GetState->setEnabled(false);
    ui->GetAllState->setEnabled(false);
    ui->GetInfo->setEnabled(false);
    ui->GetAllInfo->setEnabled(false);
    ui->GetReport->setEnabled(false);
}
void MainWindow::allButtonOn()
{
    ui->errorLab->setText("状态：正常");
    ui->On->setEnabled(true);
    ui->Off->setEnabled(true);
    ui->AllOn->setEnabled(true);
    ui->AllOff->setEnabled(true);
    ui->GetState->setEnabled(true);
    ui->GetAllState->setEnabled(true);
    ui->GetInfo->setEnabled(true);
    ui->GetAllInfo->setEnabled(true);
    ui->GetReport->setEnabled(true);
}

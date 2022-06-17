#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pileinfo.h"
#include "time.h"
#include "QDir"
//#include <unistd.h>

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
        ui->Fmode->setText("快充 max:"+list[1]);
        ui->Tmode->setText("慢充 max:"+list[2]);
        ui->Finfo->setText("快充 max:"+list[1]);
        ui->Tinfo->setText("慢充 max:"+list[2]);
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
    QString pileNo=QString::number(ui->OffNo->value());
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
    /*if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("开启全部充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("开启全部充电桩失败！\n原因：%1").arg(list[1]));
    }*/
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
   /* QStringList list = msg.split("/");
    if(list[0]=="yes")
    {
        QMessageBox::information(this, tr("提示"),  tr("关闭全部充电桩成功！"));
    }
    else
    {
        QMessageBox::critical(this, tr("出错"),  tr("关闭全部充电桩失败！\n原因：%1").arg(list[1]));
    }*/
}
void MainWindow::on_GetState_clicked()
{
    ui->State->clear();
    QString message="getPileInfo/";
    QString pileNo=QString::number(ui->infoNo->value());
    QString mode;
    if(btnGroup2->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    QString text = "充电桩："+mode+pileNo+"\n";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QByteArray msg = socket->readAll();
    qDebug()<<msg;
    PileInfo* pileinfo=reinterpret_cast<PileInfo *>(msg.data());
    int state=pileinfo->workingState;
    if(state==0)
        text+="    充电桩状态：开启\n";
    else if(state==1)
        text+="    充电桩状态：关闭\n";
    else  text+="    充电桩状态：故障\n";
    int chargingnumber=pileinfo->totalChargingNumber;
    text+="    充电总次数："+QString::number(chargingnumber)+"\n";
    time_t chargingtime=pileinfo->totalChargingTime;
    //text+="    充电总时长："+timetran(chargingtime)+"\n";
    double chargingcapacity=pileinfo->totalChargingCapacity;
    text+="    充电总电量："+QString::number(chargingcapacity)+"\n";
    ui->State->setText(text);
    /*QString msg = socket->readAll();
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
    }*/
}
void MainWindow::on_GetAllState_clicked()
{
    ui->State->clear();
    QString text="";
    for(int i=1; i<=FastCharingPileNum; i++)
    {
        QString message="getPileInfo/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="F";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        qDebug()<<msg;
        PileInfo* pileinfo=reinterpret_cast<PileInfo *>(msg.data());
        int state=pileinfo->workingState;
        if(state==0)
            text+="    充电桩状态：开启\n";
        else if(state==1)
            text+="    充电桩状态：关闭\n";
        else  text+="    充电桩状态：故障\n";
        int chargingnumber=pileinfo->totalChargingNumber;
        text+="    充电总次数："+QString::number(chargingnumber)+"\n";
        time_t chargingtime=pileinfo->totalChargingTime;
        //text+="    充电总时长："+timetran(chargingtime)+"\n";
        double chargingcapacity=pileinfo->totalChargingCapacity;
        text+="    充电总电量："+QString::number(chargingcapacity)+"\n";
    }
    for(int i=1; i<=TrickleChargingPileNum; i++)
    {
        QString message="getPileInfo/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="T";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        qDebug()<<msg;
        PileInfo* pileinfo=reinterpret_cast<PileInfo *>(msg.data());
        int state=pileinfo->workingState;
        if(state==0)
            text+="    充电桩状态：开启\n";
        else if(state==1)
            text+="    充电桩状态：关闭\n";
        else  text+="    充电桩状态：故障\n";
        int chargingnumber=pileinfo->totalChargingNumber;
        text+="    充电总次数："+QString::number(chargingnumber)+"\n";
        time_t chargingtime=pileinfo->totalChargingTime;
        //text+="    充电总时长："+timetran(chargingtime)+"\n";
        double chargingcapacity=pileinfo->totalChargingCapacity;
        text+="    充电总电量："+QString::number(chargingcapacity)+"\n";
    }
    ui->State->setText(text);
}
void MainWindow::on_GetInfo_clicked()
{
    ui->Info->clear();
    QString message="getCarInfo/";
    QString pileNo=QString::number(ui->infoNo->value());
    QString mode;
    if(btnGroup2->checkedId() == 0)
        mode="F";
    else
        mode="T";
    message=message+mode+pileNo+"\t";
    QString text = "充电桩："+mode+pileNo+"\n";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QByteArray msg = socket->readAll();
    qDebug()<<msg;
    msg=msg.remove(0, 4);
    qDebug()<<msg;
    int usernum=msg.size()/sizeof (CarInfo);
    QByteArray carinfo;
    for(int i=0;i<usernum;i++)
    {
        carinfo=msg.left(sizeof(CarInfo));
        msg.remove(0, sizeof(CarInfo));
        CarInfo* c=reinterpret_cast<CarInfo *>(carinfo.data());
        int vnum=c->Vnum;
        text+="    *车号："+QString::number(vnum)+"\n";
        char ownID[8];
        strcpy(ownID, c->ownerID);
        std::string ownid=ownID;
        text+="    用户ID："+QString::fromStdString(ownID)+"\n";
        double batterycapacity=c->batteryCapacity;
        text+="    电池容量："+QString::number(batterycapacity)+"\n";
        double request=c->requestChargingCapacity;
        text+="    请求充电量："+QString::number(request)+"\n";
        time_t time=c->queueTime;
        //text+="    排队时长："+timetran(time)+"\n";
        int queuenum=c->queueNum;
        text+="    排队号："+QString::number(queuenum)+"\n";
        int mode=c->chargingMode;
        if(mode==0)
            text+="    充电模式：T\n";
        else
            text+="    充电模式：F\n";
        double alreadycapacity=c->alreadyChargingCapacity;
        text+="    *已冲电量："+QString::number(alreadycapacity,'g',5)+"\n";
        double nowfee=c->nowFee;
        text+="    *当前费用："+QString::number(nowfee,'g',5)+"\n";
    }
    ui->Info->setText(text);
}
void MainWindow::on_GetAllInfo_clicked()
{
    ui->Info->clear();
    QString text="";
    for(int i=1; i<=FastCharingPileNum; i++)
    {
        QString message="getCarInfo/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="F";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        msg=msg.remove(0, 4);
        qDebug()<<msg;
        int usernum=msg.size()/sizeof (CarInfo);
        QByteArray carinfo;
        for(int j=0;j<usernum;j++)
        {
            carinfo=msg.left(sizeof(CarInfo));
            msg.remove(0, sizeof(CarInfo));
            CarInfo* c=reinterpret_cast<CarInfo *>(carinfo.data());
            int vnum=c->Vnum;
            text+="    *车号："+QString::number(vnum)+"\n";
            char ownID[8];
            strcpy(ownID, c->ownerID);
            std::string ownid=ownID;
          //  text+="    用户ID："+QString::fromStdString(ownID)+"\n";
            double batterycapacity=c->batteryCapacity;
           // text+="    电池容量："+QString::number(batterycapacity)+"\n";
            double request=c->requestChargingCapacity;
          //  text+="    请求充电量："+QString::number(request)+"\n";
            time_t time=c->queueTime;
            //text+="    排队时长："+timetran(time)+"\n";
            int queuenum=c->queueNum;
           // text+="    排队号："+QString::number(queuenum)+"\n";
            int mode=c->chargingMode;
          //  if(mode==0)
          //      text+="    充电模式：T\n";
          //  else
         //       text+="    充电模式：F\n";
            double alreadycapacity=c->alreadyChargingCapacity;
            text+="    *已冲电量："+QString::number(alreadycapacity,'g',5)+"\n";
            double nowfee=c->nowFee;
            text+="    *当前费用："+QString::number(nowfee,'g',5)+"\n";
        }
        text+="\n";
    }
    for(int i=1; i<=TrickleChargingPileNum; i++)
    {
        QString message="getCarInfo/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="T";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        msg=msg.remove(0, 4);
        qDebug()<<msg;
        int usernum=msg.size()/sizeof (CarInfo);
        QByteArray carinfo;
        for(int j=0;j<usernum;j++)
        {
            carinfo=msg.left(sizeof(CarInfo));
            msg.remove(0, sizeof(CarInfo));
            CarInfo* c=reinterpret_cast<CarInfo *>(carinfo.data());
            int vnum=c->Vnum;
            text+="    *车号："+QString::number(vnum)+"\n";
            char ownID[8];
            strcpy(ownID, c->ownerID);
            std::string ownid=ownID;
           // text+="    用户ID："+QString::fromStdString(ownID)+"\n";
            double batterycapacity=c->batteryCapacity;
           // text+="    电池容量："+QString::number(batterycapacity)+"\n";
            double request=c->requestChargingCapacity;
            //text+="    请求充电量："+QString::number(request)+"\n";
            time_t time=c->queueTime;
            //text+="    排队时长："+timetran(time)+"\n";
            int queuenum=c->queueNum;
            //text+="    排队号："+QString::number(queuenum)+"\n";
            int mode=c->chargingMode;
            //if(mode==0)
             //   text+="    充电模式：T\n";
           // else
            //    text+="    充电模式：F\n";
            double alreadycapacity=c->alreadyChargingCapacity;
            text+="    *已冲电量："+QString::number(alreadycapacity,'g',5)+"\n";
            double nowfee=c->nowFee;
            text+="    *当前费用："+QString::number(nowfee,'g',5)+"\n";
        }
        text+="\n";
    }
    ui->Info->setText(text);
    static int times = 0;
    QString filepath = QString::number(++times)+"_chargingcarinfo.txt";
    QFile data(filepath);
    data.open(QIODevice::WriteOnly);
    QString str = ui->Info->toPlainText();
    QByteArray context = str.toUtf8();
    data.write(context);
    data.close();
}
void MainWindow::on_GetReport_clicked()
{
    ui->Report->clear();
    QString text="";
    for(int i=1; i<=FastCharingPileNum; i++)
    {
        QString message="getReport/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="F";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        qDebug()<<msg;
        ReportInfo* report=reinterpret_cast<ReportInfo *>(msg.data());
        char pileno[8];
        strcpy(pileno, report->pileNo);
        text+="    充电桩编号："+QString::fromStdString(pileno)+"\n";
        int chargingnumber=report->totalChargingNumber;
        text+="    累计充电次数："+QString::number(chargingnumber)+"\n";
        time_t chargingtime=report->totalChargingTime;
        //text+="    累计充电时长："+timetran(chargingtime)+"\n";
        double chargingcapacity=report->totalChargingCapacity;
        text+="    累计充电量："+QString::number(chargingcapacity)+"\n";
        double chargingfee=report->totalChargingFee;
        text+="    累计充电费用："+QString::number(chargingfee)+"\n";
        double servicefee=report->totalServiceFee;
        text+="    累计服务费用："+QString::number(servicefee)+"\n";
        double allfee=report->totalAllFee;
        text+="    累计总费用："+QString::number(allfee)+"\n";
    }
    for(int i=1; i<=TrickleChargingPileNum; i++)
    {
        QString message="getReport/";
        QString pileNo=QString::number(i);
        QString mode;
        mode="T";
        message=message+mode+pileNo+"\t";
        text += "充电桩："+mode+pileNo+"\n";
        socket->write(message.toLatin1());
        socket->flush();
        allButtonOff();
        loop.exec();
        QByteArray msg = socket->readAll();
        qDebug()<<msg;
        ReportInfo* report=reinterpret_cast<ReportInfo *>(msg.data());
        char pileno[8];
        strcpy(pileno, report->pileNo);
        text+="    充电桩编号："+QString::fromStdString(pileno)+"\n";
        int chargingnumber=report->totalChargingNumber;
        text+="    累计充电次数："+QString::number(chargingnumber)+"\n";
        time_t chargingtime=report->totalChargingTime;
        //text+="    累计充电时长："+timetran(chargingtime)+"\n";
        double chargingcapacity=report->totalChargingCapacity;
        text+="    累计充电量："+QString::number(chargingcapacity)+"\n";
        double chargingfee=report->totalChargingFee;
        text+="    累计充电费用："+QString::number(chargingfee)+"\n";
        double servicefee=report->totalServiceFee;
        text+="    累计服务费用："+QString::number(servicefee)+"\n";
        double allfee=report->totalAllFee;
        text+="    累计总费用："+QString::number(allfee)+"\n";
    }
    ui->Report->setText(text);
}
void MainWindow::on_GetWaiting_clicked()
{
    ui->waiting->clear();
    QString message="getWaiting\t";
    QString text = "";
    socket->write(message.toLatin1());
    socket->flush();
    allButtonOff();
    loop.exec();
    QByteArray msg = socket->readAll();
    qDebug()<<msg;
    msg=msg.remove(0, 4);
    qDebug()<<msg;
    int usernum=msg.size()/sizeof (CarInfo);
    QByteArray carinfo;
    for(int i=0;i<usernum;i++)
    {
        carinfo=msg.left(sizeof(CarInfo));
        msg.remove(0, sizeof(CarInfo));
        CarInfo* c=reinterpret_cast<CarInfo *>(carinfo.data());
        int vnum=c->Vnum;
        text+="    *车号："+QString::number(vnum)+"\n";
        char ownID[8];
        strcpy(ownID, c->ownerID);
        std::string ownid=ownID;
        text+="    用户ID："+QString::fromStdString(ownID)+"\n";
        double batterycapacity=c->batteryCapacity;
        text+="    电池容量："+QString::number(batterycapacity)+"\n";
        double request=c->requestChargingCapacity;
        text+="    *请求充电量："+QString::number(request)+"\n";
        time_t time=c->queueTime;
        //text+="    排队时长："+timetran(time)+"\n";
        int queuenum=c->queueNum;
        text+="    排队号："+QString::number(queuenum)+"\n";
        int mode=c->chargingMode;
        if(mode==0)
            text+="    *充电模式：T\n";
        else
            text+="    *充电模式：F\n";
        double alreadycapacity=c->alreadyChargingCapacity;
        text+="    已冲电量："+QString::number(alreadycapacity,'g',5)+"\n";
        double nowfee=c->nowFee;
        text+="    当前费用："+QString::number(nowfee,'g',5)+"\n";
        text+="\n";
    }
    ui->waiting->setText(text);
    static int times = 0;
    QString filepath = QString::number(++times)+"_waitingcarinfo.txt";
    QFile data(filepath);
    data.open(QIODevice::WriteOnly);
    QString str = ui->waiting->toPlainText();
    QByteArray context = str.toUtf8();
    data.write(context);
    data.close();
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
    ui->GetWaiting->setEnabled(false);
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
    ui->GetWaiting->setEnabled(true);
}

QString MainWindow::timetran(int s)
{
    //将秒数转化为时分秒格式
    int H = s / (60*60);
    int M = (s- (H * 60 * 60)) / 60;
    int S = (s - (H * 60 * 60)) - M * 60;
    QString hour = QString::number(H);
    if (hour.length() == 1) hour = "0" + hour;
    QString min = QString::number(M);
    if (min.length() == 1) min = "0" + min;
    QString sec = QString::number(S);
    if (sec.length() == 1) sec = "0" + sec;
    QString qTime = hour + ":" + min + ":" + sec;
    return qTime;
}

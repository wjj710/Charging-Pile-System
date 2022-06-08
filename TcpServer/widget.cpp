#include "widget.h"
#include "ui_widget.h"
#include "global.h"
using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    port = 8080;
    server = new Server(this,port);
    workerThread = new WorkerThread(this);
    connect(this,&Widget::sendinit,server,&Server::init);
    //设置充电桩类型下拉框
    QStringList strlist1;
    strlist1<<"快充"<<"慢充";
    ui->pileType->addItems(strlist1);
    //设置调度方式下拉框
    QStringList strlist2;
    strlist1<<"优先级调度"<<"时间顺序调度";
    ui->pileType->addItems(strlist2);
    //恢复按钮刚开始无效
    ui->recover->setDisabled(true);

    connect(server, &Server::loopquit, workerThread, &WorkerThread::loopquit,Qt::DirectConnection);
    connect(server, &Server::showserver, this, &Widget::slotshow);
    connect(workerThread, &WorkerThread::showserver, this, &Widget::slotshow);
}

Widget::~Widget()
{
    workerThread->quit();
    for(int i=0; i<Global::proclist.size(); i++){
        QProcess *p=Global::proclist.at(i);
        p->kill();
        free(p);
    }
    delete ui;
}
void Widget::slotshow(QString msg, QHostAddress addr,int port,bool send)
{
    if(send == 1)
    {
        ui->textBrowser->append("<font color=\"#B22222\">"+addr.toString()+"/"+QString::number(port)+" -- "+msg+"</font> ");
    }else {
        ui->textBrowser->append("<font color=\"#228B22\">"+addr.toString()+"/"+QString::number(port)+" -- "+msg+"</font> ");
    }

}

//服务器初始化，即InitController的功能
void Widget::on_initialize_clicked()
{
    emit sendinit(ui->fastPileNum->value(), ui->tricklePileNum->value(), ui->waitingSize->value(), ui->chargingQueueLen->value());
    ui->initialize->setDisabled(true);
    if (!workerThread->isRunning()) workerThread->start(); //初始化后启动线程
}

//充电桩故障处理
void Widget::on_malfunction_clicked()
{
    string s="malfunction/";
    string t=ui->pileType->currentIndex()?"T":"F";
    string u=to_string(ui->pileNumber->value());
    string k=ui->dispatchMethod->currentIndex()?"2":"1";
    s+=(k+"\t");
    int d=Global::mstr2Int[t+u];
    emit sendserver(QString::fromStdString(s),d);
    ui->malfunction->setDisabled(true);
    ui->recover->setEnabled(true);
    ui->pileType->setDisabled(true);
    ui->pileNumber->setDisabled(true);
    ui->dispatchMethod->setDisabled(true);
}

//充电桩故障恢复
void Widget::on_recover_clicked()
{
    string s="recover\t";
    int d=Global::mstr2Int[Global::malfunctionPileNo];
    Global::malfunctionPileNo="";
    emit sendserver(QString::fromStdString(s),d);
    ui->malfunction->setEnabled(true);
    ui->recover->setDisabled(true);
    ui->pileType->setEnabled(true);
    ui->pileNumber->setEnabled(true);
    ui->dispatchMethod->setEnabled(true);
}


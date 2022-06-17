#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include "header.h"

namespace Ui {
class WorkWindow;
}

class WorkWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WorkWindow(QWidget *parent = nullptr);
    ~WorkWindow();

public slots:
    void get_usrID(QString UID);
    void get_state_recv(QString responce);
    void get_ahead_recv(QString responce);
    void get_queue_recv(QString responce);
    void get_detail_recv(QString responce);
    void get_bill_recv(QString responce);
    void socket_disconnect();

private slots:
    void on_submitRequest_clicked();

    void on_changeMode_clicked();

    void on_changeCapacity_clicked();

    void on_submitChange_clicked();

    void on_endORpay_clicked();

    void on_quit_clicked();

    void on_viewWaitNum_clicked();

    void on_viewQueueNum_clicked();

    void on_viewDetail_clicked();

    void on_carButton_clicked();

    void on_carBox_valueChanged(int arg1);

private:
    Ui::WorkWindow *ui;
    QString usrID;              // 用户ID
    QString carNum;             // 车辆号
    QString usr_state;          // 用户当前状态
    QString usr_mode;           // 用户当前充电模式
    QString usr_capacity;       // 用户当前充电量
    QString usr_battery;        // 用户电池容量

    void InitWindow();          // 初始化窗口布局
    void SetStateFree();        // 未提交申请时的布局
    void SetStateWaiting();     // 处于等待状态下的布局
    void SetStateCharging();    // 处于充电中状态下的布局
    void SetStateFinished();    // 处于充电完成状态下的布局
    void SetAllFalse();         // 禁用出切换车辆外的全部按钮
    void EndCharging();         // 结束充电
    void PayBills();            // 支付账单
    void EnterCarNum();         // 输入车辆号
    void CutoverCar(int car);   // 切换车辆

    int test_model[40] = {-1,0,0,1,1,0,0,1,0,1,
                           0,1,1,1,1,0,1,0,0,1,
                           1,1,1,1,0,0,0,0,1,0,
                           0};
    double test_amount[40]={0 ,40 ,30,100,120,20,20,110,20 ,105,
                            10,110,90,110,95 ,10,60,10 ,7.5,75 ,
                            95,95 ,70,80 ,5  ,15,20,25 ,30 ,30 ,
                            10};
};

#endif // WORKWINDOW_H

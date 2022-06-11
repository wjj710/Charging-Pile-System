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

private:
    Ui::WorkWindow *ui;
    QString usrID;              // 用户ID
    QString usr_state;          // 用户当前状态
    QString usr_mode;           // 用户当前充电模式
    QString usr_capacity;       // 用户当前充电量
    QString usr_battery;        // 用户电池容量

    void InitWindow();          // 初始化窗口布局
    void SetStateFree();        // 未提交申请时的布局
    void SetStateWaiting();     // 处于等待状态下的布局
    void SetStateCharging();    // 处于充电中状态下的布局
    void SetStateFinished();    // 处于充电完成状态下的布局
    void EndCharging();         // 结束充电
    void PayBills();            // 支付账单
};

#endif // WORKWINDOW_H

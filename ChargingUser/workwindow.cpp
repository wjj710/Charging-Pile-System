#include "workwindow.h"
#include "ui_workwindow.h"

WorkWindow::WorkWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkWindow),
    carNum("0"), usr_battery("1000")
{
    ui->setupUi(this);
    setWindowTitle("Intelligent Charging System"); // 设置标题
    ui->welcome->setAlignment(Qt::AlignCenter); // 欢迎字样居中
    setFixedSize(800, 600); // 设置窗口大小

    ui->capacityEdit->setMaxLength(8); // 充电量不允许超过8位数
    ui->batteryEdit->setMaxLength(8); // 电池容量不允许超过8位数

    //ui->carEdit->setEnabled(false); // 禁止更改车辆号
    //ui->carEdit->setText(carNum);   // 默认车辆号为1
    ui->stateText->setText("服务器已连接");
    ui->carButton->setEnabled(false);
    ui->modeBox->setCurrentIndex(0);        // 默认快充
    ui->infoBrowser->append(QString("欢迎光临！您正在查看%1号车辆。").arg(carNum));
}

WorkWindow::~WorkWindow()
{
    delete ui;
}

void WorkWindow::socket_disconnect()
{
    ui->stateText->setText("服务器已断开");
}

void WorkWindow::get_usrID(QString UID)
{
    usrID = UID;
    InitWindow();
    this->show();
}

void WorkWindow::InitWindow()
{
    ui->usrText->setText(usrID); // 设置用户名
    Socket::Instance().SendRequest("state/" + carNum);
}

void WorkWindow::get_state_recv(QString responce) // 更新状态
{
    QStringList resp_list = responce.split('/');
    usr_state = resp_list.at(0);
    usr_mode = resp_list.at(1) == "slow" ? "0" : "1";
    usr_capacity = resp_list.at(2);
    usr_battery = resp_list.at(3);
    if (usr_state == "free") {
        SetStateFree();
    } else if (usr_state == "charging") {
        SetStateCharging();
    } else if (usr_state == "waiting") {
        SetStateWaiting();
    } else if (usr_state == "finished") {
        SetStateFinished();
    }
    ui->modeBox->setCurrentIndex(usr_mode == "1" ? 0 : 1);
    ui->capacityEdit->setText(usr_capacity);
    ui->batteryEdit->setText(usr_battery);
    // 自动测试
    if (usr_state == "free") {
        QString capacity = QString::number(test_amount[ui->carBox->value()],'g',5);
        qDebug()<<"capacity:"<<capacity<<"   Index:"<<test_model[ui->carBox->value()];
        ui->capacityEdit->setText(capacity);
        ui->modeBox->setCurrentIndex(test_model[ui->carBox->value()]);
    }
}

void WorkWindow::SetStateFree()
{
    //ui->stateText->setText("尚未提交申请");
    ui->submitRequest->setEnabled(true);
    ui->modeBox->setEnabled(true);
    ui->capacityEdit->setEnabled(true);
    ui->batteryEdit->setEnabled(true);
    ui->submitChange->setEnabled(false);
    ui->changeMode->setEnabled(false);
    ui->changeCapacity->setEnabled(false);
    ui->viewWaitNum->setEnabled(false);
    ui->viewQueueNum->setEnabled(false);
    ui->viewDetail->setEnabled(false);
    ui->endORpay->setEnabled(false);
    ui->endORpay->setText("结束充电");
    ui->quit->setEnabled(true);
    usr_state = "free";
}

void WorkWindow::SetStateWaiting()
{
    //ui->stateText->setText("等待中");
    ui->submitRequest->setEnabled(false);
    ui->modeBox->setEnabled(false);
    ui->capacityEdit->setEnabled(false);
    ui->batteryEdit->setEnabled(false);
    ui->submitChange->setEnabled(false);
    ui->changeMode->setEnabled(true);
    ui->changeCapacity->setEnabled(true);
    ui->viewWaitNum->setEnabled(true);
    ui->viewQueueNum->setEnabled(true);
    ui->viewDetail->setEnabled(false);
    ui->endORpay->setEnabled(true);
    ui->endORpay->setText("结束充电");
    ui->quit->setEnabled(true);
    usr_state = "waiting";
}

void WorkWindow::SetStateCharging()
{
    //ui->stateText->setText("充电中");
    ui->submitRequest->setEnabled(false);
    ui->modeBox->setEnabled(false);
    ui->capacityEdit->setEnabled(false);
    ui->batteryEdit->setEnabled(false);
    ui->submitChange->setEnabled(false);
    ui->changeMode->setEnabled(false);
    ui->changeCapacity->setEnabled(false);
    ui->viewWaitNum->setEnabled(true);
    ui->viewQueueNum->setEnabled(true);
    ui->viewDetail->setEnabled(false);
    ui->endORpay->setEnabled(true);
    ui->endORpay->setText("结束充电");
    ui->quit->setEnabled(true);
    usr_state = "charging";
}

void WorkWindow::SetStateFinished()
{
    //ui->stateText->setText("充电完成");
    ui->submitRequest->setEnabled(false);
    ui->modeBox->setEnabled(false);
    ui->capacityEdit->setEnabled(false);
    ui->batteryEdit->setEnabled(false);
    ui->submitChange->setEnabled(false);
    ui->changeMode->setEnabled(false);
    ui->changeCapacity->setEnabled(false);
    ui->viewWaitNum->setEnabled(false);
    ui->viewQueueNum->setEnabled(true);
    ui->viewDetail->setEnabled(true);
    ui->endORpay->setEnabled(true);
    ui->endORpay->setText("支付账单");
    ui->quit->setEnabled(true);
    usr_state = "finished";
}

void WorkWindow::SetAllFalse()
{
    ui->submitRequest->setEnabled(false);
    ui->modeBox->setEnabled(false);
    ui->capacityEdit->setEnabled(false);
    ui->batteryEdit->setEnabled(false);
    ui->submitChange->setEnabled(false);
    ui->changeMode->setEnabled(false);
    ui->changeCapacity->setEnabled(false);
    ui->viewWaitNum->setEnabled(false);
    ui->viewQueueNum->setEnabled(false);
    ui->viewDetail->setEnabled(false);
    ui->endORpay->setEnabled(false);
    ui->quit->setEnabled(true);
}

void WorkWindow::on_submitRequest_clicked() // 提交充电请求
{
    // 获取用户输入
    usr_mode = ui->modeBox->currentText() == "slow" ? "0" : "1";
    usr_capacity = ui->capacityEdit->text();
    usr_battery = ui->batteryEdit->text();

    // 判断输入是否合法
    if (usr_capacity.isEmpty() || IsNumber(usr_capacity) == false) { // 判断充电量输入是否合法
        MessageWindow(this, "充电量必须是整数！");
        return;
    }
    if (usr_battery.isEmpty() || IsNumber(usr_battery) == false) { // 判断电池容量输入是否合法
        MessageWindow(this, "电池容量必须是整数！");
        return;
    }
    if (usr_capacity.toInt() > usr_battery.toInt()) { // 判断充电量是否大于电池容量
        MessageWindow(this, "充电量不能大于电池容量");
        return;
    }

    QString request = QString("startRequest/%1/%2/%3/%4").arg(carNum, usr_mode, usr_capacity, usr_battery);
    Socket::Instance().SendRequest(request); // 发送充电请求
    SetStateWaiting(); // 转入等待状态
}

void WorkWindow::on_changeMode_clicked()
{
    ui->modeBox->setEnabled(true); // 开启充电模式选择框
    ui->changeMode->setEnabled(false); // 关闭修改充电模式按钮
    ui->changeCapacity->setEnabled(false); // 关闭修改充电量按钮
    ui->submitChange->setEnabled(true); // 开启提交修改按钮
}

void WorkWindow::on_changeCapacity_clicked()
{
    ui->capacityEdit->setEnabled(true); // 开启充电量输入框
    ui->changeMode->setEnabled(false); // 关闭修改充电模式按钮
    ui->changeCapacity->setEnabled(false); // 关闭修改充电量按钮
    ui->submitChange->setEnabled(true); // 开启提交修改按钮
}

void WorkWindow::on_submitChange_clicked()                                // 提交修改申请
{
    QString temp_capacity = ui->capacityEdit->text();
    QString temp_mode = ui->modeBox->currentText() == "slow" ? "0" : "1";

    // 判断输入是否合法
    if (temp_capacity.isEmpty() || IsNumber(temp_capacity) == false) {    // 判断充电量输入是否合法
        MessageWindow(this, "充电量必须是整数！");
        Socket::Instance().SendRequest("state/" + carNum);
        return;
    }
    if (temp_capacity.toInt() > usr_battery.toInt()) {                   // 判断充电量是否大于电池容量
        MessageWindow(this, "充电量不能大于电池容量");
        Socket::Instance().SendRequest("state/" + carNum);
        return;
    }

    // 提交修改申请
    if (usr_capacity == temp_capacity && usr_mode == temp_mode) {       // 充电量和充电模式没有变化
        MessageWindow(this, "充电请求没有变化，该请求不会被提交");
    } else if (usr_mode != temp_mode) {                                 // 充电模式改变，提交修改申请
        Socket::Instance().SendRequest(QString("changeRequest/%1/mode/%2").arg(carNum, temp_mode));
    } else if (usr_capacity != temp_capacity) {                         // 充电量改变，提交申请
        Socket::Instance().SendRequest(QString("changeRequest/%1/capacity/%2").arg(carNum, temp_capacity));
    }

    // 更新目前状态
    Socket::Instance().SendRequest("state/" + carNum);
}

void WorkWindow::on_viewWaitNum_clicked()                               // 查看前车等待数量
{
    Socket::Instance().SendRequest("getAheadNum/" + carNum);
}

void WorkWindow::get_ahead_recv(QString responce)
{
    ui->infoBrowser->append(QString("您前面还有%1辆车正在排队，请您耐心等待。").arg(responce));
}

void WorkWindow::on_viewQueueNum_clicked()                              // 查看排队号
{
    Socket::Instance().SendRequest("getQueueNum/" + carNum);
}

void WorkWindow::get_queue_recv(QString responce)
{
    ui->infoBrowser->append(QString("您的排队号为：%1").arg(responce));
}

void WorkWindow::on_viewDetail_clicked()                                //  查看详单
{
    Socket::Instance().SendRequest("getDetail/" + carNum);
}

void WorkWindow::get_detail_recv(QString responce)
{
    ui->infoBrowser->append(QString("本次充电的详细账单如下："));
    ui->infoBrowser->append(responce);
}

void WorkWindow::on_endORpay_clicked()
{
    if (ui->endORpay->text() == "结束充电")
        EndCharging();
    else if (ui->endORpay->text() == "支付账单")
        PayBills();
}

void WorkWindow::EndCharging()                                          // 结束充电
{
    Socket::Instance().SendRequest("endRequest/" + carNum);
    SetStateFinished();
}

void WorkWindow::PayBills()                                             // 支付账单
{
    Socket::Instance().SendRequest("bill/" + carNum);
}

void WorkWindow::get_bill_recv(QString responce)
{
    QString info = QString("您的账单金额为：%1元\n是否确认支付？").arg(responce);
    QMessageBox box(QMessageBox::Question, "支付确认", info);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, "支付");
    box.setButtonText(QMessageBox::Cancel, "取消");
    box.setDefaultButton(QMessageBox::Cancel);
    int ret = box.exec();
    switch(ret) {
    case QMessageBox::Ok:
        Socket::Instance().SendRequest("pay/" + carNum);
        MessageWindow(this, "支付成功");
        SetStateFree();
        ui->infoBrowser->append("支付成功，感谢您的使用！");
        break;
    case QMessageBox::Cancel:
        break;
    }
}

void WorkWindow::on_quit_clicked()
{
    this->close();
}

void WorkWindow::on_carButton_clicked()                             // 切换车辆
{
//    if (ui->carButton->text() == "切换车辆")
//        EnterCarNum();
//    else if (ui->carButton->text() == "确认切换")
//        CutoverCar();
}

//void WorkWindow::EnterCarNum()
//{
//    SetAllFalse();                      // 禁用其他按钮
//    ui->carButton->setText("确认切换");  // 修改按钮功能
//    //ui->carEdit->setEnabled(true);      // 开启车辆号输入
//}

void WorkWindow::CutoverCar(int car)
{
    if (car >= 40) {
        MessageWindow(this, "车辆号必须小于40！");
        return;
    }
    carNum = QString::number(car);
    //ui->carEdit->setEnabled(false);
    ui->carButton->setText("切换车辆");
    Socket::Instance().SendRequest(QString("state/%1").arg(carNum));
    ui->infoBrowser->clear();
    ui->infoBrowser->append(QString("欢迎光临！您正在查看%1号车辆。").arg(carNum));
}

void WorkWindow::on_carBox_valueChanged(int arg1)
{
    CutoverCar(arg1);
}


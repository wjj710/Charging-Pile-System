#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 设置信号与槽
    // 窗口间传递信息
    connect(&start_window, &StartWindow::pass_usrID, &work_window, &WorkWindow::get_usrID);
    // socket传递回复
    connect(&Socket::Instance(), &Socket::pass_login_recv, &start_window, &StartWindow::get_login_recv); // 传递登录回复
    connect(&Socket::Instance(), &Socket::pass_logon_recv, &start_window, &StartWindow::get_logon_recv); // 传递注册回复
    connect(&Socket::Instance(), &Socket::pass_state_recv, &work_window, &WorkWindow::get_state_recv); // 传递状态回复
    connect(&Socket::Instance(), &Socket::pass_ahead_recv, &work_window, &WorkWindow::get_ahead_recv); // 传递前车数量回复
    connect(&Socket::Instance(), &Socket::pass_queue_recv, &work_window, &WorkWindow::get_queue_recv); // 传递排队号回复
    connect(&Socket::Instance(), &Socket::pass_detail_recv, &work_window, &WorkWindow::get_detail_recv); // 传递详单回复
    connect(&Socket::Instance(), &Socket::pass_bill_recv, &work_window, &WorkWindow::get_bill_recv); // 传递付款结果回复

    start_window.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


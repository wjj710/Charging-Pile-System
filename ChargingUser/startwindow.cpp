#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setWindowTitle("Intelligent Charging System"); // 设置标题
    ui->welcome->setAlignment(Qt::AlignCenter); // 欢迎字样居中
    setFixedSize(800, 600); // 设置窗口大小

    // 设置信号与槽
    // connect(this, &StartWindow::pass_usrID, &ww, &WorkWindow::get_usrID); // 传递UID

    // 用户名与密码输入设置
    ui->username->setPlaceholderText("请先输入用户名和密码");
    ui->password->setPlaceholderText("然后再点击登录或注册");
    ui->username->setMaxLength(7); // 用户名最大长度为7
    ui->password->setMaxLength(20); // 密码最大长度为20
    ui->password->setEchoMode(QLineEdit::Password); //密码隐藏
}

StartWindow::~StartWindow()
{
    delete ui;
}


void StartWindow::on_login_clicked() // 点击登录
{
    usrID = ui->username->text();
    password = ui->password->text();
    if (HaveIllegalChar(usrID) || HaveIllegalChar(password)) { // 检查非法字符
        MessageWindow(this, "错误！\n含有非法字符 \\ / : * ? \" < > | 或内容为空！");
    } else { // 输入合法，连接服务器，验证登录信息
        if (Socket::Instance().connected == false) { // 服务器连接失败
            MessageWindow(this, "服务器连接失败");
            return;
        }
        QString request = QString("%1/%2/%3").arg("login", usrID, password); // 请求信息
        Socket::Instance().SendRequest(request);
    }
}

void StartWindow::get_login_recv(QString responce) // 处理登录回复
{
    if (responce == "yes") { // 登录成功
        emit pass_usrID(usrID); // 传递用户ID
        this->close(); // 关闭本窗口
    } else if (responce == "no") { // 登录失败，密码错误
        MessageWindow(this, "登录失败！\n密码错误。");
    } else if (responce == "unknown") { // 登录失败，尚未注册
        MessageWindow(this, "登录失败！\n用户名未注册。");
    } else {
        MessageWindow(this, "登录失败！\n服务器发生错误。");
    }
}

void StartWindow::on_logon_clicked() // 点击注册
{
    usrID = ui->username->text();
    password = ui->password->text();
    if (HaveIllegalChar(usrID) || HaveIllegalChar(password)) { // 检查非法字符
        MessageWindow(this, "错误！含有非法字符 \\ / : * ? \" < > | 或内容为空！");
    } else { // 输入合法，进行注册
        if (Socket::Instance().connected == false) { // 服务器连接失败
            MessageWindow(this, "服务器连接失败");
            return;
        }
        QString request = QString("%1/%2/%3").arg("logon", usrID, password); // 请求信息
        Socket::Instance().SendRequest(request);
    }
}

void StartWindow::get_logon_recv(QString responce) // 处理注册回复
{
    if (responce == "yes") { // 注册成功
        MessageWindow(this, "注册成功！");
    } else if (responce == "no"){ // 注册失败，用户名重复
        MessageWindow(this, "注册失败！\n用户名已注册。");
    } else {                      // 注册失败，服务器数据库错误
        MessageWindow(this, "注册失败！\n服务器发生错误。");
    }
}

void StartWindow::on_quit_clicked() // 点击退出
{
    this->close();
}


#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "server.h"
#include "workerthread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    int port;
    Server* server;
    WorkerThread *workerThread;
protected slots:
    void slotshow(QString, QHostAddress, int,bool);//接收到server发过来的信号就更新界面的信息
private slots:
    void on_initialize_clicked();
    void on_malfunction_clicked();
    void on_recover_clicked();
    void on_Print_clicked();

signals:
    void sendinit(int, int, int, int);
    void sendserver(QByteArray,int);
};
#endif // WIDGET_H

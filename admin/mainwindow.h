#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QButtonGroup>
#include <QEventLoop>
#include "admin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void self_init();
    void allButtonOn();
    void allButtonOff();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_On_clicked();
    void on_Off_clicked();
    void on_AllOn_clicked();
    void on_AllOff_clicked();
    void on_GetState_clicked();
    void on_GetAllState_clicked();
    void on_GetInfo_clicked();
    void on_GetAllInfo_clicked();
    void on_GetReport_clicked();

    void socket_Read_Data();
    void socket_Disconnected();
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QEventLoop loop;
    QButtonGroup *btnGroup1;
    QButtonGroup *btnGroup2;
public:
    int FastCharingPileNum=0;
    int TrickleChargingPileNum=0;
    int WaitingAreaSize=0;
    int ChargingQueueLen=0;
    int allOver=1;//当前页面是否关闭，如果关闭，所有操作将不再进行
};
#endif // MAINWINDOW_H

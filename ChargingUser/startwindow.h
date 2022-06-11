#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include "header.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

public slots:
    void get_login_recv(QString responced);
    void get_logon_recv(QString responced);

signals:
    void show_work_window();
    void pass_usrID(QString UID);

private:
    Ui::StartWindow *ui;
    QString usrID, password;

private slots:
    void on_login_clicked();

    void on_quit_clicked();

    void on_logon_clicked();
};

#endif // STARTWINDOW_H

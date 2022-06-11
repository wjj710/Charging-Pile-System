#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "header.h"
#include "startwindow.h"
#include "workwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    StartWindow start_window;
    WorkWindow work_window;
};
#endif // MAINWINDOW_H

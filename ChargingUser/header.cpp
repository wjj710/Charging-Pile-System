#include "header.h"

QString illegal_char = "<>\\/|:?*";

bool HaveIllegalChar(QString str)
{
    if (str.length() == 0) return true;
    for (int i = 0; i < str.length(); i++) {
        if (illegal_char.indexOf(str[i]) != -1)
            return true;
    }
    return false;
}

bool IsNumber(QString str)
{
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

void MessageWindow(QMainWindow* w, QString str)
{
    QString title = "Info";
    QMessageBox::about(w, title, str);
}

void MessageWindow(QWidget* w, QString str)
{
    QString title = "Info";
    QMessageBox::about(w, title, str);
}

#ifndef HEADER_H
#define HEADER_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QString>
#include <QDebug>
#include "socket.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

extern QString illegal_char;

bool HaveIllegalChar(QString str);
bool IsNumber(QString str);
void MessageWindow(QMainWindow* w, QString str);
void MessageWindow(QWidget* w, QString str);

#endif // HEADER_H

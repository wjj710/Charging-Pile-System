/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QTextBrowser *textBrowser;
    QLabel *label_2;
    QGridLayout *gridLayout_3;
    QPushButton *recover;
    QComboBox *dispatchMethod;
    QPushButton *malfunction;
    QSpinBox *pileNumber;
    QComboBox *pileType;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QSpinBox *fastPileNum;
    QSpinBox *tricklePileNum;
    QSpinBox *waitingSize;
    QSpinBox *chargingQueueLen;
    QPushButton *initialize;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setPointSize(12);
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);

        gridLayout->addWidget(textBrowser, 5, 0, 1, 1);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 6, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        recover = new QPushButton(Widget);
        recover->setObjectName(QString::fromUtf8("recover"));

        gridLayout_3->addWidget(recover, 1, 3, 1, 1);

        dispatchMethod = new QComboBox(Widget);
        dispatchMethod->setObjectName(QString::fromUtf8("dispatchMethod"));

        gridLayout_3->addWidget(dispatchMethod, 1, 2, 1, 1);

        malfunction = new QPushButton(Widget);
        malfunction->setObjectName(QString::fromUtf8("malfunction"));

        gridLayout_3->addWidget(malfunction, 0, 3, 1, 1);

        pileNumber = new QSpinBox(Widget);
        pileNumber->setObjectName(QString::fromUtf8("pileNumber"));

        gridLayout_3->addWidget(pileNumber, 1, 1, 1, 1);

        pileType = new QComboBox(Widget);
        pileType->setObjectName(QString::fromUtf8("pileType"));

        gridLayout_3->addWidget(pileType, 1, 0, 1, 1);

        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);

        label_9 = new QLabel(Widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 0, 1, 1, 1);

        label_10 = new QLabel(Widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 0, 2, 1, 1);


        gridLayout->addLayout(gridLayout_3, 7, 0, 1, 1);

        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font);

        gridLayout->addWidget(label, 4, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 1, 1, 1);

        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 3, 1, 1);

        label_7 = new QLabel(Widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        fastPileNum = new QSpinBox(Widget);
        fastPileNum->setObjectName(QString::fromUtf8("fastPileNum"));

        gridLayout_2->addWidget(fastPileNum, 1, 0, 1, 1);

        tricklePileNum = new QSpinBox(Widget);
        tricklePileNum->setObjectName(QString::fromUtf8("tricklePileNum"));

        gridLayout_2->addWidget(tricklePileNum, 1, 1, 1, 1);

        waitingSize = new QSpinBox(Widget);
        waitingSize->setObjectName(QString::fromUtf8("waitingSize"));

        gridLayout_2->addWidget(waitingSize, 1, 2, 1, 1);

        chargingQueueLen = new QSpinBox(Widget);
        chargingQueueLen->setObjectName(QString::fromUtf8("chargingQueueLen"));

        gridLayout_2->addWidget(chargingQueueLen, 1, 3, 1, 1);

        initialize = new QPushButton(Widget);
        initialize->setObjectName(QString::fromUtf8("initialize"));

        gridLayout_2->addWidget(initialize, 0, 4, 2, 1);


        gridLayout->addLayout(gridLayout_2, 3, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250\345\210\235\345\247\213\345\214\226\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\345\205\205\347\224\265\346\241\251\346\225\205\351\232\234\357\274\232", nullptr));
        recover->setText(QCoreApplication::translate("Widget", "\346\201\242\345\244\215", nullptr));
        malfunction->setText(QCoreApplication::translate("Widget", "\346\225\205\351\232\234", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "\345\205\205\347\224\265\346\241\251\347\261\273\345\236\213", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "\345\205\205\347\224\265\346\241\251\345\217\267", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "\350\260\203\345\272\246\346\226\271\345\274\217", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250\346\216\245\345\217\227/\345\217\221\351\200\201\344\277\241\346\201\257\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\205\242\345\205\205\347\224\265\346\241\251\346\225\260", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "\347\255\211\345\200\231\345\214\272\350\275\246\344\275\215\345\256\271\351\207\217", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\345\205\205\347\224\265\346\241\251\346\216\222\351\230\237\351\230\237\345\210\227\351\225\277\345\272\246", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "\345\277\253\345\205\205\347\224\265\346\241\251\346\225\260", nullptr));
        initialize->setText(QCoreApplication::translate("Widget", "\345\210\235\345\247\213\345\214\226", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

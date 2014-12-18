/********************************************************************************
** Form generated from reading UI file 'log.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_H
#define UI_LOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_log
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QFrame *frame;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *textEdit_2;
    QPlainTextEdit *plainTextEdit;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *log)
    {
        if (log->objectName().isEmpty())
            log->setObjectName(QStringLiteral("log"));
        log->resize(505, 422);
        pushButton = new QPushButton(log);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(110, 170, 81, 23));
        pushButton->setAutoDefault(false);
        textEdit = new QTextEdit(log);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(270, 220, 104, 70));
        pushButton_2 = new QPushButton(log);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(350, 290, 81, 23));
        pushButton_3 = new QPushButton(log);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(320, 300, 81, 23));
        pushButton_4 = new QPushButton(log);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 170, 81, 23));
        pushButton_5 = new QPushButton(log);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(340, 300, 81, 23));
        pushButton_6 = new QPushButton(log);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(290, 310, 81, 23));
        pushButton_7 = new QPushButton(log);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(300, 320, 81, 23));
        pushButton_8 = new QPushButton(log);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(310, 330, 81, 23));
        pushButton_9 = new QPushButton(log);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(320, 340, 81, 23));
        pushButton_10 = new QPushButton(log);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(330, 350, 81, 23));
        pushButton_11 = new QPushButton(log);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(340, 360, 81, 23));
        frame = new QFrame(log);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(50, 40, 120, 80));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        scrollArea = new QScrollArea(log);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(180, 10, 120, 80));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 118, 78));
        scrollArea->setWidget(scrollAreaWidgetContents);
        textEdit_2 = new QTextEdit(log);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setGeometry(QRect(350, 30, 104, 70));
        plainTextEdit = new QPlainTextEdit(log);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(380, 140, 104, 70));
        textBrowser = new QTextBrowser(log);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 220, 256, 192));

        retranslateUi(log);
        QObject::connect(pushButton, SIGNAL(clicked()), textEdit, SLOT(clear()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), pushButton, SLOT(animateClick()));

        QMetaObject::connectSlotsByName(log);
    } // setupUi

    void retranslateUi(QWidget *log)
    {
        log->setWindowTitle(QApplication::translate("log", "log", 0));
        pushButton->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_2->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_3->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_4->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_5->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_6->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_7->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_8->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_9->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_10->setText(QApplication::translate("log", "PushButton", 0));
        pushButton_11->setText(QApplication::translate("log", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class Log: public Ui_log {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_H

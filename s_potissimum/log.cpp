// #ifdef __QT__
#include "log.h"
#include "ui_log.h"
#include <iostream>

Log::Log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log)
{
  ui->setupUi(this);
}

void Log::receive_message(QString new_message)
{
//   findChild<QTextEdit *>("textEdit")->append(QString::fromStdString(new_message));
  findChild<QTextEdit *>("textEdit")->append(new_message);
//   std::cout << "Log::receive_message initiated. message:" << new_message.toUtf8().constData() << std::endl;
}

/*
//void Log::createUi(QWidget *parent)
//{
    //this->setFixedSize(800,600);
    //this->setWindowTitle("Log");


    //полоска апдейта
    up_date = new QDateTime(QDateTime::currentDateTime());
    QString str = up_date->toString(Qt::LocalDate);

    update_button = new QPushButton("last update");
    update_label = new QLabel(str);
    update_button->setFlat(true);

    //полоски вертикальные
    frame = new QFrame();
    frame -> setFrameStyle(QFrame::VLine | QFrame::Raised);

//}
//void Log::findChildren("*button")
//{

//}*/


Log::~Log()
{
    delete ui;
}

// #endif
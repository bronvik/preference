#ifndef LOG_H
#define LOG_H

#include "ui_Log.h"
#include <QWidget>

namespace Ui {
class Log;
}

class Log : public QWidget//, public Ui::Log
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = 0);
    ~Log();

signals:

public slots:
  void receive_message(QString new_message);

private:
    Ui::Log *ui;
};

#endif // LOG_H



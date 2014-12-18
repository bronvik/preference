#ifndef UPDATER_H
#define UPDATER_H

#include "include.h"

class Updater : public QObject
{
    Q_OBJECT

public:
    Updater() { QString message_to_update; }

    QString message() const { return message_to_update; }

public slots:
    void send_message_slot(QString new_message);

signals:
    void send_message_signal(QString message);
//     void receive_message(QString message);

private:
    QString message_to_update;
};
#endif // UPDATER_H

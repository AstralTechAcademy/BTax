//
// Created by gabridc on 21/9/22.
//

#ifndef BROKER_NOTIFICATIONMANAGER_H
#define BROKER_NOTIFICATIONMANAGER_H

#include <QObject>

class NotificationManager : public QObject{
Q_OBJECT

public:
    NotificationManager (QObject *parent = 0) {};

signals:
    void dbNotOpened(void);


};

#endif //BROKER_NOTIFICATIONMANAGER_H

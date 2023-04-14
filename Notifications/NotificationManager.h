//
// Created by gabridc on 21/9/22.
//

#ifndef BROKER_NOTIFICATIONMANAGER_H
#define BROKER_NOTIFICATIONMANAGER_H

#include <QObject>

class NotificationManager : public QObject{
Q_OBJECT

public:
    static NotificationManager* getInstance(void) {
        if (!instance_)
            instance_ = new NotificationManager();
        return instance_;
    }

private:
    NotificationManager (QObject *parent = 0) {};
    inline static NotificationManager* instance_;

signals:
    void dbNotOpened(void);
    void newOperationError(QString message);


};

#endif //BROKER_NOTIFICATIONMANAGER_H

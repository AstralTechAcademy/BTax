//
// Created by gabridc on 26/8/21.
//

#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H
#include <QObject>
#include <QVariant>

class Broker : public QObject
{
    Q_OBJECT

public:
        explicit Broker(const QString& server, const QString& version, QObject *parent = nullptr);


    enum class HOSTS{
        WIN = 0,
        MACOSX = 1,
        LINUX = 2,
        ANDROIDD = 3,
        IOS = 4
    };

public slots:
    int getHost(void) const;
    QString getServer(void) const;
    QString getVersion(void) const;

private:
    static HOSTS host_;
    static QString server_;
    static QString version_;
};


#endif //BROKER_BROKER_H

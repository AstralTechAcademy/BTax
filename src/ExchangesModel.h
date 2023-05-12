//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_EXCHANGEMODEL_H
#define BROKER_EXCHANGEMODEL_H
#include <QAbstractListModel>
#include <QList>

class Exchange : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString timezone READ timezone)
public:
    Exchange(void) = default;
    Exchange(int id, const QString& name, const QString& timezone) :
        id_(id),
        name_(name),
        timezone_(timezone){}

    int id(void) const noexcept {return id_;};
    QString name(void) const noexcept {return name_;};
    QString timezone(void) const noexcept {return timezone_;};

    void setTimezone(const QString timezone) noexcept {timezone_ = timezone;};

private:
    int id_;
    QString name_;
    QString timezone_;
};


class ExchangesModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QList<Exchange* > exchanges READ exchanges)
public:
    enum RoleNames {
        ExchangeID = 1,
        ExchangeName,
        TimeZone
    };

    ExchangesModel(QObject* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;

    QList<Exchange*> exchanges(void);
    void add(Exchange* exch);
    QString getTimeZone(const QString& name) const;
    void clear(void) noexcept;
public slots:
    int getID(const int index);

private:
    QList<Exchange* > exchanges_;

};


#endif //BROKER_WALLETSMODEL_H

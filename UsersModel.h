//
// Created by gabridc on 25/12/21.
//

#ifndef BROKER_USERSMODEL_H
#define BROKER_USERSMODEL_H
#include <QAbstractListModel>


class UsersModel : public QAbstractListModel{

public:

    enum Roles {
        ID,
        Username,
        DisplayRole = Qt::DisplayRole
    };

    UsersModel(const QObject* o = 0){};
    QHash<int,QByteArray> roleNames() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Username) const;
    static QList<std::tuple<uint32_t , QString>> getUsers(void);
    void setUsers(void);
private:
    static QList<std::tuple<uint32_t , QString>> users ;


};


#endif //BROKER_USERSMODEL_H

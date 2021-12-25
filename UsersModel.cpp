//
// Created by gabridc on 25/12/21.
//

#include "UsersModel.h"
#include "DBLocal.h"


QList<std::tuple<uint32_t , QString>> UsersModel::users;


QList<std::tuple<uint32_t , QString>>  UsersModel::getUsers(void)
{
    return users;
}
void UsersModel::setUsers(void)
{
    users = DBLocal::GetInstance()->getUsers();
}

QVariant UsersModel::data(const QModelIndex & index, int role) const {
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return {};

    const auto user = users.at(index.row());
    if (role == ID) return std::get<0>(user); // ID
    if (role == Username) return std::get<1>(user); // Username


    return {};
}

int UsersModel::rowCount(const QModelIndex & parent) const {
    if (parent.isValid())
        return 0;
    return users.size();
}

QHash<int,QByteArray> UsersModel::roleNames() const {
return { { ID, "ID" },
{ Username, "Username" } ,
         {DisplayRole, "Display"}
};
}
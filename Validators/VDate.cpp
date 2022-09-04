//
// Created by gabridc on 4/9/22.
//

#include "VDate.h"
#include <QDebug>


bool VDate::validate(void) const
{
    auto current = QDateTime::currentDateTime();

    if (dateTime_.date().year() == 1900)
    {
        qDebug() << "Validation Error: Year has not been modified is 1900";
        return false;
    }

    if (dateTime_ > current)
    {
        qDebug() << "Validation Error: datetime is over current time";
        return false;
    }

    if (dateTime_.date().year() != 1900 && dateTime_ <= current)
        return true;

    qDebug() << "Validation Error: Datetime unknown error";
    return false;
}
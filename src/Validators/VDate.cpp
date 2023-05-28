//
// Created by gabridc on 4/9/22.
//

#include "VDate.h"
#include "Logger.h"
#include <QDebug>


bool VDate::validate(void) const
{
    auto current = QDateTime::currentDateTime();

    if (dateTime_.date().year() == 1900)
    {
        LOG_ERROR("Year has not been modified is 1900");
        return false;
    }

    if (dateTime_ > current)
    {
        LOG_ERROR("Datetime is over current time");
        return false;
    }

    if (dateTime_.date().year() != 1900 && dateTime_ <= current)
        return true;

    LOG_ERROR("Datetime unknown error");
    return false;
}
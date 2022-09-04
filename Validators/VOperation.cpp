//
// Created by gabridc on 4/9/22.
//

#include "VOperation.h"
#include "VDate.h"
#include <QDebug>


bool VOperation::validate(void) const
{
    VDate vdate(dateTime_);

    if(vdate.validate() == false)
        return false;

    if(data_.pair1Amount == -0.1 || data_.pair1AmountFiat == -0.1 || data_.pair2Amount == -0.1 || data_.pair2AmountFiat == -0.1 ||
            data_.comision == -0.1 || data_.comisionFiat == -0.1)
    {
        qDebug() << "[VALIDATION ERROR] Amounts are empty";
        return false;
    }

    if(data_.pair1Amount < 0)
    {
        qDebug() << "[VALIDATION ERROR] Pair1 is negative";
        return false;
    }

    if(data_.pair1AmountFiat < 0)
    {
        qDebug() << "[VALIDATION ERROR] Pair1Fiat is negative";
        return false;
    }

    if(data_.pair2Amount < 0)
    {
        qDebug() << "[VALIDATION ERROR] Pair2 is negative";
        return false;
    }

    if(data_.pair2AmountFiat < 0)
    {
        qDebug() << "[VALIDATION ERROR] Pair2Fiat is negative";
        return false;
    }


    return true;
}

//
// Created by gabridc on 4/9/22.
//

#ifndef BROKER_VDATE_H
#define BROKER_VDATE_H

#include "Validator.h"
#include <QDateTime>

class VDate : public Validator{

public:
    VDate() = delete;
    VDate(QDateTime dateTime) : dateTime_(dateTime){};
    virtual bool validate(void) const;

    QDateTime dateTime_;
};


#endif //BROKER_VDATE_H

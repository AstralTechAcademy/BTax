//
// Created by gabridc on 4/9/22.
//

#ifndef BROKER_VOPERATION_H
#define BROKER_VOPERATION_H

#include "Validator.h"
#include "WalletOperation.h"
#include <QDateTime>

class VOperation : public Validator {

public:
    VOperation() = delete;
    VOperation(WalletOperation::OperationData data, QDateTime dateTime) : data_(data), dateTime_(dateTime){};
    virtual bool validate(void) const;

    QDateTime dateTime_;
    WalletOperation::OperationData data_;

};


#endif //BROKER_VOPERATION_H

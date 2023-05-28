//
// Created by gabridc on 4/9/22.
//

#ifndef BROKER_VALIDATOR_H
#define BROKER_VALIDATOR_H


class Validator {
protected:
    virtual bool validate() const {return false;}
    template<class T>
    bool validate(T data) const {return false;}
};


#endif //BROKER_VALIDATOR_H
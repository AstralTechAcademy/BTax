//
// Created by gabridc on 11/11/21.
//

#ifndef BROKER_REPORTER_H
#define BROKER_REPORTER_H
#include <QObject>

class Reporter : public QObject {

Q_OBJECT

public:
    void generate(const int year);

signals: 
    void finished(void);


};


#endif //BROKER_REPORTER_H
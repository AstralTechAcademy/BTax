//
// Created by gabridc on 24/10/21.
//

#ifndef BROKER_OPERATIONSMODEL_H
#define BROKER_OPERATIONSMODEL_H
#include <QAbstractListModel>
#include "Operation.h"


class OperationsModel : public QAbstractListModel{

    Q_OBJECT
    //Q_PROPERTY(QList<Operation*> operations READ operations WRITE setOperations)

    Q_PROPERTY(double totalEarnings READ totalEarnings NOTIFY totalEarningsChanged)
public:
    enum OperationRoles {
        id = Qt::UserRole + 1,
        pair1,
        pair2,
        pair1Amount,
        pair1AmountFiat,
        pair2Amount,
        pair2AmountFiat,
        comision,
        comisionFiat,
        status,
        date,
        comments,
        type,
        ganancia,
    };
    OperationsModel(QObject *parent = 0)
    {
        totalEarnings_ = 0.0;
    };

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;
    void add(Operation* operation);
    void clear(void);
    QList<Operation*> operations(void);
    void setOperations(QList<Operation*> operations);

    // Esta función permite modificar los parametros de la clase Operation desde QML
    bool setData(const QModelIndex &index, const QVariant &value, int role);


public slots:
    double totalEarnings(void) const;


signals:
    void countChanged();
    void totalEarningsChanged(double);
private:
    QList<Operation*> operations_;
    double totalEarnings_;

    void calculateEarnings(void);



};


#endif //BROKER_OPERATIONSMODEL_H

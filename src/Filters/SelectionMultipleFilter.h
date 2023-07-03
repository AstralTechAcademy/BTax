#ifndef MULTIPLE_FILTER_H
#define MULTIPLE_FILTER_H
#include <QString>
#include <QList>

class SelectionMultipleFilter
{
public:
    SelectionMultipleFilter() = default;
    SelectionMultipleFilter(QString name) : filterName_(name){};

public:
    void update(QString element);
    const QList<QString>& get() const noexcept;

private:
    QList<QString> elements_;
    QString filterName_;
};

#endif
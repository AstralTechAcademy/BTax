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
    void clear();
    void update(QString element);
    void remove(QString element);
    const QList<QString>& get() const noexcept;

private:
    QList<QString> elements_;
    QString filterName_;
};

#endif
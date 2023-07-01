#include "SelectionMultipleFilter.h"
#include "Logger.h"

void SelectionMultipleFilter::update(QString element)
{
    auto index = elements_.indexOf(element);
    if(index != -1)
        elements_.removeAt(index);
    else
        elements_.push_back(element);
    
    LOG_DEBUG("%s %s %d", qPrintable(filterName_), qPrintable(element), elements_.size());
}
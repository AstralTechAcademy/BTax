#include "Reporter.h"
#include "SQLManager.h"
#include "BrokerManager.h"


void Reporter::generate(const int year)
{

    auto res = SQLManager::GetInstance()->getOperations(BrokerManager::userID, year);

    if(res == std::nullopt)
        LOG_INFO("There are not operations retrieved in this year");

    LOG_DEBUG("Operations retrieved count: %d"  + res->size());

    auto operations = res.value();

    for(auto a : operations)
    {
        std::cout << a->getGanancia() << std::endl;
    }

}
//
// Created by gabridc on 11/11/21.
//

#include "BrokerManager.h"
#include "Statistics.h"
#include "SQLManager.h"

double Statistics::getCurrentRevenue(void)
{
    SQLManager::GetInstance()->getWallets(BrokerManager::userID);
}
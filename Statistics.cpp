//
// Created by gabridc on 11/11/21.
//

#include "BrokerManager.h"
#include "Statistics.h"
#include "DBLocal.h"

double Statistics::getCurrentRevenue(void)
{
    DBLocal::GetInstance()->getWallets(BrokerManager::userID);
}
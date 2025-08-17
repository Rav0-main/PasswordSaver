#pragma once

#include "../Log2Database/main.h"
#include "accountdata.h"

/*
* Show and run DELETE_ACCOUNT_SCREEN
*/
void runDeleteAccountScreen(Database* accountDatabase, AccountData* account, const Database* userDatabase, int* currentScreen);
#pragma once

#include "accountdata.h"
#include "../Log2Database/main.h"
#include <stdbool.h>

/*
* Returns true if user success signed else false
*/
bool runSignInScreen(const Database* userDatabase, int* currentScreen, AccountData* account);
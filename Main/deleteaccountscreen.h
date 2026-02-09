#pragma once

#include "../Log2Database/main.h"
#include "accountdata.h"
#include "screencodes.h"

/*
* Show and run DeleteAccountScreen
*/
void runDeleteAccountScreen(
	Database* const restrict accountDatabase, AccountData* const restrict account,
	Database* const restrict userDatabase, Screen* const restrict currentScreen
);
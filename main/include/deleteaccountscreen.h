#pragma once

#include "log2database.h"
#include "accountdata.h"
#include "screencodes.h"

/*
* Show and run DeleteAccountScreen
*/
void runDeleteAccountScreen(
	Database* const restrict accountDatabase, AccountData* const restrict account,
	Database* const restrict users, Screen* const restrict currentScreen
);
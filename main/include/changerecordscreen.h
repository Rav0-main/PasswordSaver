#pragma once

#include "log2database.h"
#include "screencodes.h"

/*
* Show and run ChangeRecordScreen
*/
void runChangeRecordScreen(
	Database* const restrict accountDatabase, Screen* const restrict currentScreen,
	const char* const restrict password
);
#pragma once

#include "log2database.h"
#include "screencodes.h"

/*
* Show and run CreateRecordScreen
*/
void runCreateRecordScreen(
	Database* const restrict accountDatabase, Screen* const restrict currentScreen,
	const char* const restrict password
);
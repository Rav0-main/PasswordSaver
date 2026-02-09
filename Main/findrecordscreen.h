#pragma once

#include "../Log2Database/main.h"
#include "screencodes.h"

/*
* Show and run FindRecordScreen
*/
void runFindRecordScreen(
	const Database* const restrict accountDatabase, Screen* const restrict currentScreen,
	const char* const restrict password
);
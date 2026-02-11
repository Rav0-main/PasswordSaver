#pragma once

#include "accountdata.h"
#include "screencodes.h"
#include "log2database.h"

/*
* Show and run RegisterScreen
*/
void runRegisterScreen(
	Screen* const restrict currentScreen, Database* const restrict users
);
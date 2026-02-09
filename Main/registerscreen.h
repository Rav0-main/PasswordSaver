#pragma once

#include "accountdata.h"
#include "screencodes.h"
#include "../Log2Database/main.h"

/*
* Show and run RegisterScreen
*/
void runRegisterScreen(
	Screen* const restrict currentScreen, Database* const restrict userDatabase
);